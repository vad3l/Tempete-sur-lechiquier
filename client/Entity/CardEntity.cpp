#include "CardEntity.hpp"

CardEntity::CardEntity(gf::ResourceManager& resources,GameData &gameData)
: m_cardsFont(resources.getFont("fonts/DejaVuSans.ttf"))
,m_cardsIllustration(resources.getTexture("images/CardIlustrationSheet.png"))
,m_accessories(resources.getTexture("images/AccesoriesCards.png"))
,m_gameData(gameData)
{
	m_cardsIllustration.setSmooth(true);
	m_accessories.setSmooth(true);
}

void CardEntity::update([[maybe_unused]] gf::Time time) {
}

void CardEntity::render(gf::RenderTarget &target, const gf::RenderStates &states, Card c, float x, float y){

	int numberImageW = 10;
	int numberImageH = 14;
	gf::Coordinates coordsCard({1200,300});
	gf::Vector2i sizeCard = gf::Vector2i(200,300);
	constexpr float titleCharacterSize = 16.f;
	constexpr float instructionsCharacterSize = 14.f;

	if(c.m_num == -1) {
		return;
	}
	
	int i = c.m_num%numberImageW;
	int j = c.m_num/numberImageW;	   
	gf::Vector2f position= coordsCard.getRelativePoint({ x, y });
	gf::RoundedRectangleShape card(sizeCard);
	card.setColor(gf::Color::White);
	card.setRadius(22);
	card.setOutlineColor(gf::Color::Black);
	card.setPosition(position);

	gf::RectangleShape illustration({sizeCard.x/1.25f, sizeCard.x/1.25f});
	illustration.setTexture(m_cardsIllustration,gf::RectF::fromPositionSize({ (1.f / numberImageW) * i , (1.f/numberImageH) * j }, { 1.f / numberImageW, 1.f/numberImageH }));
	illustration.setPosition({position.x+sizeCard.x/2.f, position.y+sizeCard.y/3.5f});
	illustration.setAnchor(gf::Anchor::Center);

	gf::Text cardName(c.m_name, m_cardsFont, titleCharacterSize);
	cardName.setColor(gf::Color::Black);
	if(c.m_name.size()>15) {
		cardName.setPosition({position.x+25, position.y+sizeCard.y/1.62f});
	}else {
		cardName.setPosition({position.x+25, position.y+sizeCard.y/1.55f});
	}
	cardName.setAlignment(gf::Alignment::Center);
	cardName.setParagraphWidth(200-50);

	gf::RectangleShape rect({sizeCard.x/1.1f, sizeCard.x/4.3f});
	rect.setTexture(m_accessories,gf::RectF::fromPositionSize({ 0.f  , 0.f }, { 1.f , 1.f }));
	rect.setPosition({position.x+sizeCard.x/2.f, position.y+sizeCard.y/1.6f});
	rect.setAnchor(gf::Anchor::Center);
	switch (c.m_turn) {
		case Turn::AVANT_COUP:
			rect.setColor(gf::Color::Yellow);
			break;
		case Turn::APRES_COUP:
			rect.setColor(gf::Color::Green);
			break;
		case Turn::BOTH:
			rect.setColor(gf::Color::fromRgba32(212,234,45));
			break;
		case Turn::DURING_TOUR_ADVERSE:
			rect.setColor(gf::Color::Cyan);
			break;
	}
	/*if(m_gameData.m_phase.getCurrentPhase()==Phase::COUP) {
		rect.setColor(gf::Color::White);
	}*/
	
	gf::Text cardDescription(c.m_description.substr(0, 60)+"...", m_cardsFont, instructionsCharacterSize);
	cardDescription.setColor(gf::Color::Black);
	cardDescription.setPosition({position.x+10, position.y+sizeCard.y/1.3f});
	cardDescription.setParagraphWidth(200-20);
	cardDescription.setAlignment(gf::Alignment::Center);
		
	target.draw(card, states);
	target.draw(illustration, states);
	target.draw(rect, states);
	target.draw(cardName, states);
	target.draw(cardDescription, states);
}