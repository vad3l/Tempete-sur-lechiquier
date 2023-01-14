#ifndef CARD_H
#define CARD_H
#include <string>
#include <iostream>
#include <set>
#include <algorithm>
#include <functional>
#include "Plateau.hpp"

enum class Turn{
    BEFORE,
    AFTER,
    REPLACE,
    BOTH,
    DURING
};

enum class Effect{
    CANCELCARD,
    CANCELMOVE,
    NONE
};

class Card{
    private:
        std::string m_name;
        std::string m_description;
        Turn m_turn;
        Effect m_effect;
    public:
	std::function<bool(Plateau&, gf::Vector2i, gf::Vector2i)> m_execute;
        Card(std::string name,std::string m_description,Turn turn, Effect effect);
        Turn getTurn() const; //return when we can use the card b for beforeTurn, a for after
        Effect getEffect() const;//return return r for replace,cm for CancelMove and cc for CancelCard
        std::string getName() const; // return the name of the card
};

#endif // CARD_H
