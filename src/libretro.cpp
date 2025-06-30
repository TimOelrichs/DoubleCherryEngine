#include <DoubleCherryEngine/DoubleCherryEngine.hpp>


void retro_get_system_av_info(struct retro_system_av_info* info)
{
     videoLayoutManager.updateAVInfo(info);
}

void retro_init(void) {
    DoubleCherryEngine::init();
}

void retro_deinit(void)
{
    libretro_supports_bitmasks = false;
}

bool retro_load_game(const struct retro_game_info* info)
{
	return DoubleCherryEngine::loadGame(info);
}

bool retro_load_game_special(unsigned type, const struct retro_game_info* info, size_t num_info)
{
    return false;
}

void retro_unload_game(void)
{
    DoubleCherryEngine::unloadGame();
}

void retro_reset(void)
{
    DoubleCherryEngine::reset();
}

void retro_run(void)
{
	DoubleCherryEngine::run();
}

void retro_get_system_info(struct retro_system_info* info)
{
    info->library_name = "DoubleCherryEngine";
#ifndef GIT_VERSION
#define GIT_VERSION ""
#endif
    info->library_version = "v0.1.0" GIT_VERSION;
    info->need_fullpath = false;
    info->valid_extensions = "gb|dmg|gbc|cgb|sgb";
}
