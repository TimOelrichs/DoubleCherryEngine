#pragma once
#include <libretro_variables.h>
#include <CoreEngine/common/interfaces/ISingleton.hpp>
#include <CoreEngine/common/types/types.hpp>
#include <CoreEngine/CoreEventManager/ICoreEventListener.hpp>



class NetpacketManager final : public ISingleton<NetpacketManager> {
    friend class ISingleton<NetpacketManager>;

public:
    bool isActivated();
    void setActive(bool enable);
    int getMyID();
    bool isHost() { return numClients > 0 };
    int getNumberOfClients();
    void send();
    void broadcast();

private:

    int maxClients = 1;
    int numClients = 0

 
    NetpacketManager() {

        auto& events = CoreEventsManager::getInstance();

        events->registerOnBeforeRun(this);
        //events->registerOnOptionsUpdate(this);
        //events->registerOnLoadGame(this);
    };
    void setInputDescription(retro_input_descriptor* input_desc) {
        environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, input_desc);
    };



protected:
    onBeforeRun() {
        inputPoll();
    }

    void netpacket_poll_receive() {
        if (netpacket_pollrcv_fn_ptr)
            netpacket_pollrcv_fn_ptr();
    }

    void netpacket_send(uint16_t client_id, const void* buf, size_t len) {
        // Force all packets to be flushed ASAP, to minimize latency.
        if (netpacket_send_fn_ptr)
            netpacket_send_fn_ptr(RETRO_NETPACKET_RELIABLE | RETRO_NETPACKET_FLUSH_HINT, buf, len, client_id);
    }

    static void netpacket_start(uint16_t client_id, retro_netpacket_send_t send_fn, retro_netpacket_poll_receive_t poll_receive_fn) {
        netpacket_send_fn_ptr = send_fn;
        netpacket_pollrcv_fn_ptr = poll_receive_fn;
        num_clients = 0;
        my_client_id = client_id;
    }

    // Netplay session ends.
    static void netpacket_stop() {
        netpacket_send_fn_ptr = NULL;
        netpacket_pollrcv_fn_ptr = NULL;
    }

    static void netpacket_receive(const void* buf, size_t len, unsigned short client_id) {

        const byte* data = (const byte*)buf;


    }

    // Ensure we do not have too many clients for the type of connection used.
    static bool netpacket_connected(unsigned short client_id) {
        
        if (numClients >= maxClients)
            return false;
        num_clients++;

        return true;
    }
};



// Netplay (Netpacket) interface









static void netpacket_receive(const void* buf, size_t len, unsigned short client_id) {

    const byte* data = (const byte*)buf;

    bool isMaster = (v_gb[0]->get_regs()->SC & 0x01) == 01;
    if (!isMaster)
    {
        byte retbuf[1] = { v_gb[0]->receive_from_linkcable(data[0]) };

        netpacket_send(client_id, retbuf, 1);
        return;
    }

    v_gb[0]->get_regs()->SB = data[0];
    v_gb[0]->get_regs()->SC &= 3;
    v_gb[0]->get_cpu()->irq(INT_SERIAL);
}

const struct retro_netpacket_callback netpacket_iface_null = {
  NULL,          // start
  NULL,        // receive
  NULL,           // stop
  NULL,                     // poll
  NULL,      // connected
  NULL,   // disconnected
  "DoubleCherryGB netpack V1.0",   // core version char* 
};



static void netpacket_disconnected(unsigned short client_id) {
    num_clients--;
}

const struct retro_netpacket_callback netpacket_iface = {
  netpacket_start,          // start
  netpacket_receive,        // receive
  netpacket_stop,           // stop
  NULL,                     // poll
  netpacket_connected,      // connected
  netpacket_disconnected,   // disconnected
  "DoubleCherryGB netpack V1.0",   // core version char* 
};
