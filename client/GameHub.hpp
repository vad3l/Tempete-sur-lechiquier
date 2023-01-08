#ifndef GAME_HUB_H
#define GAME_HUB_H

#include <gf/GameManager.h>

#include "Scene/StartScene.hpp"
#include "Scene/MenuScene.hpp"
#include "Scene/PlaySelectScene.hpp"
#include "Scene/GameScene.hpp"
#include "Scene/RulesScene.hpp"

/*#include "QuitScene.hpp"
#include "MenuScene.hpp"
#include "CreditScene.hpp"
*/

struct GameHub : gf::GameManager {
    GameHub();


    StartScene start;
    MenuScene menu;
    PlaySelectScene play;
    GameScene game;
    RulesScene rules;

    void setIp(std::string ip);

    /*CreditScene credits;
    QuitScene quit;
*/

};





#endif // GAME_HUB_H
