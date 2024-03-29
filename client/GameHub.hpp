#ifndef GAME_HUB_H
#define GAME_HUB_H

#include <gf/GameManager.h>
#include <gf/SegueEffects.h>

#include "Scene/DownloadScene.hpp"
#include "Scene/StartScene.hpp"
#include "Scene/MenuScene.hpp"
#include "Scene/PlaySelectScene.hpp"
#include "Scene/GameScene.hpp"
#include "Scene/RulesScene.hpp"
#include "Scene/QuitScene.hpp"
#include "Scene/SettingsScene.hpp"
#include "Scene/ZoomCardScene.hpp"

#include "AudioManager.hpp"

struct GameHub : gf::GameManager {
	GameHub();

	std::unique_ptr<DownloadScene> download;
	std::unique_ptr<StartScene> start;
	std::unique_ptr<MenuScene> menu;
	std::unique_ptr<PlaySelectScene> play;
	std::unique_ptr<GameScene> game;
	std::unique_ptr<RulesScene> rules;
	std::unique_ptr<QuitScene> quit;
	std::unique_ptr<SettingsScene> settings;
	std::unique_ptr<ZoomCardScene> zoomCard;
	
	gf::BlackoutSegueEffect blackoutEffect;
	bool m_loadingFinish = false;

	void loadingDownloadAssets();
	void loadingAssets(Network &network, GameData &data);
	bool loadingFinish();
};





#endif // GAME_HUB_H
