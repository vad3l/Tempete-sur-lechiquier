#include "MenuScene.hpp"
#include "../GameHub.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include <gf/Log.h>
#include <gf/Coordinates.h>




PlaySelectScene::PlaySelectScene(GameHub& game)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_upAction("UpAction")
, m_downAction("DownAction")
, m_rightAction("RightAction")
, m_leftAction("LeftAction")
, m_triggerAction("TriggerAction")
, m_quitAction("Quit")
, m_fullscreenAction("Fullscreen")
, m_PlayTitleEntity(game.resources)
, m_ipWidget("127.0.0.1", game.resources.getFont("Trajan-Color-Concept.otf"))
, m_index(0)
, m_leftWidget("<", game.resources.getFont("DejaVuSans.ttf"))
, m_rightWidget(">", game.resources.getFont("DejaVuSans.ttf"))
{
    setClearColor(gf::Color::Black);
    addHudEntity(m_PlayTitleEntity);

    m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
    addAction(m_fullscreenAction);
    
    m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_quitAction);

    m_upAction.addScancodeKeyControl(gf::Scancode::Up);
    addAction(m_upAction);

    m_downAction.addScancodeKeyControl(gf::Scancode::Down);
    addAction(m_downAction);

    m_leftAction.addScancodeKeyControl(gf::Scancode::Left);
    addAction(m_leftAction);

    m_rightAction.addScancodeKeyControl(gf::Scancode::Right);
    addAction(m_rightAction);

    m_triggerAction.addScancodeKeyControl(gf::Scancode::Return);
    m_triggerAction.addMouseButtonControl(gf::MouseButton::Left);
    addAction(m_triggerAction);


    auto setupButton = [&] (gf::TextButtonWidget& button, auto callback) {
        button.setDefaultTextColor(gf::Color::Black);
        button.setDefaultBackgroundColor(gf::Color::Gray(0.7f));
        button.setSelectedTextColor(gf::Color::Black);
        button.setSelectedBackgroundColor(gf::Color::fromRgba32(212,30,27,255));
        button.setDisabledTextColor(gf::Color::Black);
        button.setDisabledBackgroundColor(gf::Color::Red);
        button.setAnchor(gf::Anchor::TopLeft);
        button.setAlignment(gf::Alignment::Center);
        button.setCallback(callback);
        m_widgets.addWidget(button);
    };

    std::ifstream file(std::string(GAME_CONFIGDIR)+"IpList.txt");

    if (!file) {
        std::cerr << "Error: Unable to open file." << std::endl;
    }else{
        std::string line;
        while (std::getline(file, line)) {
            m_listIp.insert(m_listIp.begin(), line);
        }

        file.close();
    }

    setupButton(m_ipWidget, [&] () {
        m_game.setIp(std::string(m_ipWidget.getString()));
        m_game.replaceAllScenes(m_game.game);
    });

    setupButton(m_rightWidget, [&] () {
        changeRightLeft(true);
    });

    setupButton(m_leftWidget, [&] () {
        changeRightLeft(false);
    });
  
}

void PlaySelectScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (!isActive()) {
        return;
    }

    if(m_leftAction.isActive()) {
        changeRightLeft(false);
    }

    if(m_rightAction.isActive()) {
        changeRightLeft(true);
    }

    if (m_fullscreenAction.isActive()) {
        window.toggleFullscreen();
    }

    if (m_upAction.isActive()) {
        m_widgets.selectPreviousWidget();
    }

    if (m_downAction.isActive()) {
        m_widgets.selectNextWidget();
    }

    if (m_triggerAction.isActive()) {
        m_widgets.triggerAction();
    }

    if (m_quitAction.isActive()) {
        m_game.replaceScene(m_game.start);
    }
}

void PlaySelectScene::doProcessEvent(gf::Event& event) {
    switch (event.type){
        case gf::EventType::MouseMoved:
            m_widgets.pointTo(m_game.computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));
            break;
    }
}

void PlaySelectScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
    constexpr float characterSize = 0.075f;
    constexpr gf::Vector2f backgroundSize(0.5f, 0.3f);
    constexpr gf::Vector2f backgroundSizeArrow(0.2f, 0.3f);

    target.setView(getHudView());
    gf::Coordinates coords(target);

    const float paragraphWidth = coords.getRelativeSize(backgroundSize - 0.05f).x;
    const float paragraphWidthArrow = coords.getRelativeSize(backgroundSizeArrow - 0.05f).x;
    const float paddingSize = coords.getRelativeSize({0.01f, 0.f}).x;
    const unsigned resumeCharacterSize = coords.getRelativeCharacterSize(characterSize);
    
    m_ipWidget.setCharacterSize(resumeCharacterSize);
    m_ipWidget.setPosition(coords.getRelativePoint({0.275f, 0.425f + characterSize}));
    m_ipWidget.setParagraphWidth(paragraphWidth);
    m_ipWidget.setPadding(paddingSize);

    m_leftWidget.setCharacterSize(resumeCharacterSize);
    m_leftWidget.setPosition(coords.getRelativePoint({0.075f, 0.425f + characterSize }));
    m_leftWidget.setParagraphWidth(paragraphWidthArrow);
    m_leftWidget.setPadding(paddingSize);

    m_rightWidget.setCharacterSize(resumeCharacterSize);
    m_rightWidget.setPosition(coords.getRelativePoint({0.775, 0.425f + characterSize }));
    m_rightWidget.setParagraphWidth(paragraphWidthArrow);
    m_rightWidget.setPadding(paddingSize);

    m_widgets.render(target, states);
    m_PlayTitleEntity.render(target,states);
}

void PlaySelectScene::doShow() {
    m_widgets.clear();

    m_ipWidget.setDefault();
    m_leftWidget.setDefault();
    m_rightWidget.setDefault();
    
    m_widgets.addWidget(m_ipWidget);
    m_widgets.addWidget(m_leftWidget);
    m_widgets.addWidget(m_rightWidget);
    /*m_join.setDefault();
    m_widgets.addWidget(m_join);*/
    m_widgets.selectNextWidget();
}

void PlaySelectScene::changeRightLeft(bool value) {
    //true right
    if(value){
        if(m_index+1 < m_listIp.size()){
            m_ipWidget.setString(m_listIp[m_index+1]);
            m_index++;
            m_rightWidget.setSelected();
            m_leftWidget.setDefault();
            m_ipWidget.setDefault();
        }
    }else{
        if(m_index-1 >= 0){
            m_ipWidget.setString(m_listIp[m_index-1]);
            m_index--;
            m_leftWidget.setSelected();
            m_rightWidget.setDefault();
            m_ipWidget.setDefault();
        }
    }
}