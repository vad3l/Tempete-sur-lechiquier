#ifndef DECK_H
#define DECK_H

#include <vector>
#include <fstream>

#include "Card.hpp"

#define CARDS_DESCRIPTIONDIR "../data/"

class Deck{
	public:
		Deck(std::vector<int> numCards = {});

		void displayDeck();

		Card getFirst();
		int getNbCardInDeck();
		void emptyDiscard ();
		void drop (Card c);

		std::pair<std::vector<Card>,std::vector<Card>> distribute();

		std::vector<Card> m_deck;
		std::vector<Card> m_discard;

	private:
		void swap(Card &a, Card &b);
		void shuffle();
};

#endif // DECK_H
