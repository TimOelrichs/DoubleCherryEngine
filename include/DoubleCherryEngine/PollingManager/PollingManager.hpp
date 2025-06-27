#pragma once
#include <chrono>
#include <functional>
#include <vector>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/EngineCoreManager/CoreOptionsManager.hpp>
#include <DoubleCherryEngine/EnviromentManager/EnviromentManager.hpp>

class PollingManager final : public ISingleton<PollingManager> {
    friend class ISingleton<PollingManager>;

public:
    using Clock = std::chrono::steady_clock;
    using TaskFunc = std::function<void()>;

    struct PollingTask {
        TaskFunc func;
        std::chrono::milliseconds interval;
        Clock::time_point lastRun;
    };

    void start(bool use_thread = true) {
        if (running_.exchange(true)) return;

        auto& options = CoreOptionsManager::getInstance();
        pollingEnabled_ = options.getBool("enable_polling");

        auto& env = EnvironmentManager::getInstance();

        if (!pollingEnabled_) {
            if (env.log.log)
                env.log.log(RETRO_LOG_INFO, "[PollingManager] Polling disabled by core option.\n");
            use_thread_ = false;
            return;
        }

        unsigned int cpu_cores = std::thread::hardware_concurrency();

        if (!use_thread) {
            if (env.log.log)
                env.log.log(RETRO_LOG_INFO, "[PollingManager] Polling started without thread (use_thread=false).\n");
            use_thread_ = false;
        }
        else if (cpu_cores <= 1) {
            if (env.log.log)
                env.log.log(RETRO_LOG_INFO, "[PollingManager] Single-core CPU detected (%u core). Polling without thread.\n", cpu_cores);
            use_thread_ = false;
        }
        else {
            if (env.log.log)
                env.log.log(RETRO_LOG_INFO, "[PollingManager] Multi-core CPU detected (%u cores). Polling with thread.\n", cpu_cores);
            use_thread_ = true;
            thread_ = std::thread([this]() { run(); });
        }
    }

    void stop() {
        running_ = false;
        cv_.notify_one();
        if (thread_.joinable())
            thread_.join();
    }

    ~PollingManager() {
        stop();
    }

    void addTask(TaskFunc func, std::chrono::milliseconds interval) {
        std::lock_guard lock(mutex_);
        tasks_.push_back({ std::move(func), interval, Clock::now() });
        cv_.notify_one();
    }

    void performExtraPoll() {
        if (!pollingEnabled_ || use_thread_) return;

        std::lock_guard lock(mutex_);
        auto now = Clock::now();

        for (auto& task : tasks_) {
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - task.lastRun);
            if (elapsed >= task.interval) {
                task.func();
                task.lastRun = Clock::now();
            }
        }
    }

    bool isPollingEnabled() const { return pollingEnabled_; }
    bool isUsingThread() const { return use_thread_; }

private:
    PollingManager() : running_(false), use_thread_(false), pollingEnabled_(true) {}

    void run() {
        std::unique_lock lock(mutex_);
        auto& env = EnvironmentManager::getInstance();

        while (running_ && pollingEnabled_) {
            auto now = Clock::now();
            std::chrono::milliseconds sleepDuration = std::chrono::hours(1);

            for (auto& task : tasks_) {
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - task.lastRun);
                if (elapsed >= task.interval) {
                    lock.unlock();
                    task.func();
                    lock.lock();
                    task.lastRun = Clock::now();
                    elapsed = std::chrono::milliseconds(0);
                }

                auto timeToNextRun = task.interval - elapsed;
                if (timeToNextRun < sleepDuration)
                    sleepDuration = timeToNextRun;
            }

            if (sleepDuration > std::chrono::milliseconds(0)) {
                cv_.wait_for(lock, sleepDuration, [this]() { return !running_ || !pollingEnabled_; });
            }
        }

        if (env.log.log)
            env.log.log(RETRO_LOG_INFO, "[PollingManager] Polling thread exited.\n");
    }

    std::vector<PollingTask> tasks_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::thread thread_;
    std::atomic<bool> running_;
    bool use_thread_;
    bool pollingEnabled_;
};
