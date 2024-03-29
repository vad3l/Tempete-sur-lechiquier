#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/Sprite.h>
#include <gf/Texture.h>
#include <gf/Font.h>
#include <gf/Text.h>
#include <gf/Sleep.h>
#include <gf/Color.h>
#include <gf/Log.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/AnimatedSprite.h>
#include <gf/Animation.h>

#include <map>
#include <SFML/Audio.hpp>

#include "../../model/Plateau.hpp"
#include "../../protocole/Protocole.hpp"

#include "../Entity/BoardEntity.hpp"
#include "../Entity/TableBoardEntity.hpp"
#include "../Entity/MainEntity.hpp"
#include "../Entity/PoseEntity.hpp"

#include "../GameData.hpp"
#include "../Network.hpp"

struct GameHub;
struct GameData;

class GameScene : public gf::Scene {
	public:
		GameScene(GameHub &game, Network &network, GameData &gameData);
	private:
		void doHandleActions(gf::Window& window) override;
		void doProcessEvent(gf::Event& event) override;
		void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
		void doUpdate(gf::Time time) override;
		void doShow() override;

		void onActivityChange(bool active)  override;
	private:

		GameHub &m_game;
		Network &m_network;
		GameData &m_gameData;

		gf::Action m_quitAction;
		gf::Action m_fullscreenAction;
		gf::Action m_endTurnAction;
		gf::Action m_playCardAction;
		gf::Action m_triggerAction;
		gf::Action m_poseActionRight;
		gf::Action m_poseActionLeft;
		gf::Action m_discardActionRight;

		bool m_poseRight;
		bool m_poseLeft;
		bool m_discardRight;

		gf::Packet m_packet;

		gf::ExtendView m_boardView;
		gf::ExtendView m_tableBoardView;
		gf::ExtendView m_cardsView;
		gf::ExtendView m_principalView;
		gf::ViewContainer m_views;

		BoardEntity m_boardEntity;
		MainEntity m_mainEntity;
		TableBoardEntity m_tableBoardEntity;
		PoseEntity m_poseEntity;
	
		gf::TextButtonWidget m_endTurn;
		gf::TextButtonWidget m_playCard;

		gf::WidgetContainer m_widgets;

		gf::Texture &m_loading;
		gf::Font &m_font;	
		gf::Animation m_animation;
		gf::AnimatedSprite m_animatedSprite;
		
		gf::Texture &m_background;

		float startTime = 0.f;

		sf::Sound clickEndturn;
		sf::Sound clickplayCard;
		sf::Sound clock;
		sf::Sound takePiece;
		sf::Sound movePiece;
		sf::Sound checkPiece;
		sf::Sound drawCard;
		sf::Sound explosion;
};  



#endif // GAME_SCENE_H
