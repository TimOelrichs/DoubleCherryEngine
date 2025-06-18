#pragma once
#include <rtc/rtc.hpp>
#include <string>
#include <memory>
#include <functional>
#include <vector>

class WebRTCManager {
public:
    static WebRTCManager& get(); 

    void init(); 
    void setRemoteDescription(const std::string& sdp, const std::string& type);
    void addIceCandidate(const std::string& candidate, const std::string& mid);
    void sendCameraFrame(const uint8_t* rgbData, int width, int height);

    void onLocalDescription(std::function<void(std::string, std::string)> cb);
    void onLocalCandidate(std::function<void(std::string, std::string)> cb);

private:
    WebRTCManager(); 
    WebRTCManager(const WebRTCManager&) = delete;
    WebRTCManager& operator=(const WebRTCManager&) = delete;

    std::shared_ptr<rtc::PeerConnection> pc;
    std::shared_ptr<rtc::DataChannel> dataChannel;

    std::function<void(std::string, std::string)> localDescCallback;
    std::function<void(std::string, std::string)> localCandidateCallback;

    void createPeerConnection();
    void encodeRGBToJPEG(const uint8_t* rgb, int width, int height, std::vector<uint8_t>& out);
};
