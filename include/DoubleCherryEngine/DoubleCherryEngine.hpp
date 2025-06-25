#pragma once

// Libretro core API
#include <DoubleCherryEngine/libretro/libretroVariables.h>

// Core engine components
#include <DoubleCherryEngine/CoreConfigurator/CoreConfigurator.hpp>
#include <DoubleCherryEngine/EngineCoreManager/EngineCoreManager.hpp>
#include <DoubleCherryEngine/EngineEventManager/EngineEventManager.hpp>
#include <DoubleCherryEngine/EngineEventManager/IEventListener.hpp>
#include <DoubleCherryEngine/EnviromentManager/EnviromentManager.hpp>
#include <DoubleCherryEngine/CoreOptionsManager/CoreOptionsManager.hpp>
#include <DoubleCherryEngine/InputManager/InputManager.hpp>
#include <DoubleCherryEngine/Renderer/AudioRenderer/AudioRenderer.hpp>
#include <DoubleCherryEngine/Renderer/VideoRenderer/VideoRenderer.hpp>
#include <DoubleCherryEngine/Renderer/VideoRenderer/VideoLayoutManager.hpp>


// Utility components


CoreConfigurator& coreConfigurator = CoreConfigurator::getInstance();
EngineCoreManager& coreManager = EngineCoreManager::getInstance();
VideoLayoutManager& videoLayoutManager = VideoLayoutManager::getInstance();
InputManager& inputManager = InputManager::getInstance();
EnvironmentManager& environmentManager = EnvironmentManager::getInstance();


class DoubleCherryEngine {
public:

	static bool loadGame(const struct retro_game_info* info) {
		if (!info) return false;
		coreManager.setCore(coreConfigurator.getCore(info));
		return coreManager.loadGame(info);
	}
	static void unloadGame() {
		coreManager.unloadGame();
	}
	static void reset() {
		coreManager.reset();
	}	

	static void run() {
		coreManager.run();
	}

};