#pragma once
#include <libretro.h>
#include "inputDescriptors/inputDescriptors.h"

#include <cstdlib> 


static retro_netpacket_send_t netpacket_send_fn_ptr = NULL;
static retro_netpacket_poll_receive_t netpacket_pollrcv_fn_ptr = NULL;



retro_set_rumble_state_t rumble_state_cb;
retro_set_led_state_t led_state_cb;
retro_log_printf_t log_cb;
retro_video_refresh_t video_cb;
retro_audio_sample_batch_t audio_batch_cb;

retro_input_poll_t input_poll_cb;
retro_input_state_t input_state_cb;

bool libretro_supports_persistent_buffer = false;
bool libretro_supports_bitmasks = false;
struct retro_system_av_info* my_av_info = (retro_system_av_info*)malloc(sizeof(*my_av_info));

unsigned libretro_msg_interface_version = 0;
