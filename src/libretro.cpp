#include <DoubleCherryEngine/DoubleCherryEngine.h>


CoreConfigurator& coreConfigurator = CoreConfigurator::getInstance();
CoreManager& coreManager = CoreManager::getInstance();
VideoLayoutManager& videoLayoutManager = VideoLayoutManager::getInstance();


void retro_get_system_av_info(struct retro_system_av_info* info)
{
    //videoLayoutManager.getAvInfo(info);
}

void retro_init(void) {
    coreConfigurator.init();
}

void retro_deinit(void)
{
    libretro_supports_bitmasks = false;
}

bool retro_load_game(const struct retro_game_info* info)
{
    if (!info) return false;
    coreManager.setCore(coreConfigurator.getCore(info));
    return coreManager.loadGame(info);
}

bool retro_load_game_special(unsigned type, const struct retro_game_info* info, size_t num_info)
{
    return false;
}

void retro_unload_game(void)
{
    coreManager.unloadGame();
}

void retro_reset(void)
{
    coreManager.reset();
}

void retro_run(void)
{
    coreManager.run();
}

void retro_get_system_info(struct retro_system_info* info)
{
    info->library_name = "DoubleCherryGB";
#ifndef GIT_VERSION
#define GIT_VERSION ""
#endif
    info->library_version = "v0.17.0" GIT_VERSION;
    info->need_fullpath = false;
    info->valid_extensions = "gb|dmg|gbc|cgb|sgb";
}
