#pragma once
#include <DoubleCherryEngine/common/types/types.hpp>
#include <string.h>





class IMultiCore {
public:
    virtual ~IMultiCore() = default;


    SystemType getSystemType() const {
        return systemType;
	}
    virtual int getMaxSystemsCount() = 0;
    virtual int getActiveSystemsCount() = 0;


    // Initialize the core
    virtual void init() = 0;

    // Deinitialize and clean up the core
    virtual void deinit() = 0;

    // Load a standard game
    virtual bool loadGame(const struct retro_game_info* info) = 0;

    // Load a game with special parameters or behavior
    virtual bool loadGameSpecial() = 0;

    // Unload the currently loaded game
    virtual void unloadGame() = 0;

    // Reset the core state (e.g., like a soft reset)
    virtual void reset() = 0;

    // Run the core's main loop (e.g., for one frame)
    virtual void run() = 0;

    //SystemType systemType;
    //std::String gameTitle;
private:
	SystemType systemType = SystemType::GB; // Default to GB
};
