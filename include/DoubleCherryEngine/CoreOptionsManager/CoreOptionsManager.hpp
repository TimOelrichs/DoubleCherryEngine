#pragma once

#include <DoubleCherryEngine/libretro/libretroVariables.h>
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include <DoubleCherryEngine/CoreEventManager/CoreEventManager.hpp>
#include <DoubleCherryEngine/CoreEventManager/IEventListener.hpp>
#include <DoubleCherryEngine/Util/StringUtil.h>

typedef struct {
    const char* name;
    int value;
} OptionEntry;

class CoreOptionsManager : public ISingleton<CoreOptionsManager>, public IEventListener {
    friend class ISingleton<CoreOptionsManager>;
 
public:

    void init() {
        eventManager.registerOnBeforeRun(this);
        eventManager.registerOnOptionsUpdate(this);
    }

    void addOptionDefinitions(retro_core_option_v2_definition* def) {

    }
    void addOptionCategories(retro_core_option_v2_definition* def) {

    }

    int getEnumOption(const char* key, const OptionEntry* table, int default_value)
    {
        struct retro_variable var = { key, NULL };
        if (!environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) || !var.value)
            return default_value;

        for (int i = 0; table[i].name != NULL; ++i) {
            if (StringUtil::equals(var.value, table[i].name))
                return table[i].value;
        };

        return default_value;
    }

    bool getBoolOption(const char* key, bool default_value)
    {
        struct retro_variable var = { key, NULL };
        if (!environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) || !var.value)
            return default_value;

        if (StringUtil::equalsIgnoreCase(var.value, "true") ||
            StringUtil::equalsIgnoreCase(var.value, "on") ||
            StringUtil::equalsIgnoreCase(var.value, "enabled") ||
            StringUtil::equalsIgnoreCase(var.value, "1"))
        {
            return true;
        }

        if (StringUtil::equalsIgnoreCase(var.value, "false") ||
            StringUtil::equalsIgnoreCase(var.value, "off")||
            StringUtil::equalsIgnoreCase(var.value, "disabled")  ||
            StringUtil::equalsIgnoreCase(var.value, "0"))
        {
            return false;
        }

        return default_value;
    }

    int getIntOption(const char* key, int default_value)
    {
        struct retro_variable var = { key, NULL };
        if (!environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) || !var.value)
            return default_value;

        char* endptr = NULL;
        long val = strtol(var.value, &endptr, 10);
        if (endptr != var.value && *endptr == '\0')
            return (int)val;

        return default_value;
    }


private:
    CoreEventManager& eventManager = CoreEventManager::getInstance();

    CoreOptionsManager() {
     
    }


protected:

    bool updated = false;

    void onBeforeRun() override {
        if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
            eventManager.onOptionsUpdate();
    }

};
