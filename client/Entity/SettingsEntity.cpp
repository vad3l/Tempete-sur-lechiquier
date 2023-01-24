#include "SettingsEntity.hpp"

#include "../GameData.hpp"

SettingsEntity::SettingsEntity(gf::ResourceManager& resources, GameData &gameData)
: m_font(resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_backgroundTexture(resources.getTexture("images/StartMenu.png"))
, m_gameData(gameData)
{

}

void SettingsEntity::update([[maybe_unused]] gf::Time time) {
}

void SettingsEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
	gf::Coordinates coords(target);

	float backgroundHeight = coords.getRelativeSize(gf::vec(0.0f, 1.0f)).height;
	float backgroundScale = backgroundHeight / m_backgroundTexture.getSize().height;

	gf::Sprite background(m_backgroundTexture);
	background.setColor(gf::Color::Opaque(0.20f));
	background.setPosition(coords.getCenter());
	background.setAnchor(gf::Anchor::Center);
	background.setScale(backgroundScale);
	target.draw(background, states);

	unsigned titleCharacterSize = coords.getRelativeCharacterSize(0.1f);

	gf::Text title("Parametres", m_font, titleCharacterSize);
	title.setColor(gf::Color::White);
	title.setPosition(coords.getRelativePoint({ 0.5f, 0.1f }));
	title.setAnchor(gf::Anchor::TopCenter);

	unsigned subtitleCharacterSize = coords.getRelativeCharacterSize(0.05f);

	gf::Text subtitle("Style du plateau", m_font, subtitleCharacterSize);
	subtitle.setColor(gf::Color::White);
	subtitle.setPosition(coords.getRelativePoint({ 0.5f, 0.4f }));
	subtitle.setAnchor(gf::Anchor::Center);

	gf::Text textureStyle("None", m_font, subtitleCharacterSize);
	textureStyle.setColor(gf::Color::Blue);
	textureStyle.setPosition(coords.getRelativePoint({ 0.5f, 0.5f }));
	textureStyle.setAnchor(gf::Anchor::Center);

	switch (m_gameData.m_style) {
		case 0:
			textureStyle.setString("Bois");
			break;
		case 1:
			textureStyle.setString("Chic");
			break;
	}
	
	target.draw(title, states);
	target.draw(textureStyle, states);
	target.draw(subtitle, states);
}

