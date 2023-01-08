#ifndef PLAY_SELECT_SCENE_H
#define PLAY_SELECT_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Event.h>

#include <vector>

#include "../Entity/PlayTitleEntity.hpp"

struct GameHub;

class PlaySelectScene : public gf::Scene {
    public:
        PlaySelectScene(GameHub& game);

    private:
        void doProcessEvent(gf::Event& event) override;
        void doHandleActions(gf::Window& window) override;
        void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
        void doShow() override;
        void changeRightLeft(bool value);

    private:
        GameHub& m_game;
        gf::Action m_quitAction;

        gf::Action m_upAction;
        gf::Action m_downAction;
        gf::Action m_leftAction;
        gf::Action m_rightAction;

        gf::Action m_triggerAction;
        gf::Action m_fullscreenAction;
        
        PlayTitleEntity m_PlayTitleEntity;

        gf::TextButtonWidget m_leftWidget;
        gf::TextButtonWidget m_rightWidget;

        gf::WidgetContainer m_widgets;
        gf::TextButtonWidget m_ipWidget;
        std::vector<std::string> m_listIp;
        int m_index;
        
};



#endif // PLAY_SELECT_SCENE_H
