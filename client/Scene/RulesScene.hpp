#ifndef RULES_SCENE_H
#define RULES_SCENE_H

#include "../Entity/RulesEntity.hpp"

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Event.h>
#include <gf/Text.h>
#include <gf/Log.h>
#include <gf/Coordinates.h>

#include <SFML/Audio.hpp>

struct GameHub;	

class RulesScene : public gf::Scene {
	public:
		RulesScene(GameHub& game);

	private:
		void doProcessEvent(gf::Event& event) override;
		void doHandleActions(gf::Window& window) override;
		void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
		void doShow() override;
		void changeRightLeft(bool value);
		void onActivityChange(bool active)  override;
   
	private:
		GameHub& m_game;
		
		gf::Action m_leftAction;
		gf::Action m_rightAction;
		gf::Action m_downAction;
		gf::Action m_fullscreenAction;
		gf::Action m_triggerAction;
		gf::Action m_quitAction;
		
		gf::SpriteWidget m_pageLeft;
		gf::SpriteWidget m_pageRight;
		gf::SpriteWidget m_quitButton;

		RulesEntity m_rulesEntity;

		gf::WidgetContainer m_widgets;
		
		sf::Sound btnClicked;
};

#endif // RULES_SCENE_H