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

#include <iostream>


#include "../../model/Plateau.hpp"
#include "../../protocole/Protocole.hpp"

#include "../Entity/BoardEntity.hpp"
#include "../Entity/TableBoardEntity.hpp"

//A CHANGER
#include "../Entity/CardsEntity.hpp"

#include "../GameData.hpp"
#include "../Network.hpp"

struct GameHub;
struct GameData;

class GameScene : public gf::Scene {
    public:
        GameScene(GameHub& game);
        void setIp(std::string ip);
    private:
        void doHandleActions(gf::Window& window) override;
        void doProcessEvent(gf::Event& event) override;
        void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
        void doUpdate(gf::Time time) override;

        void onActivityChange(bool active)  override;
    private:
        std::string m_ip;

        GameHub& m_game;
        GameData m_gameData;

        gf::Action m_quitAction;
        gf::Action m_fullscreenAction;
        gf::Action m_texture1Action;
        gf::Action m_texture2Action;

        bool m_promotion;

        gf::Packet m_packet;

        gf::ExtendView m_boardView;
        gf::ExtendView m_tableBoardView;
        gf::ExtendView m_cardsView;        
        gf::ViewContainer m_views;

        BoardEntity m_boardEntity;    
        TableBoardEntity m_tableBoardEntity;
        //MODIFIER
        CardsEntity m_CardsEntity;
};  



#endif // GAME_SCENE_H
