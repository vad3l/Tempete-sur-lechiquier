#include "QuitScene.hpp"
#include "../GameHub.hpp"

#include "../Singletons.hpp"

QuitScene::QuitScene(GameHub& game)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_upAction("UpAction")
, m_downAction("DownAction")
, m_triggerAction("TriggerAction")
, m_resumeButton("Reprendre", game.resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_quitButton("Menu", game.resources.getFont("fonts/Trajan-Color-Concept.otf"))
{
	setClearColor(gf::Color::Black);

	m_upAction.addScancodeKeyControl(gf::Scancode::Up);
	addAction(m_upAction);

	m_downAction.addScancodeKeyControl(gf::Scancode::Down);
	addAction(m_downAction);

	m_triggerAction.addScancodeKeyControl(gf::Scancode::Return);
	m_triggerAction.addMouseButtonControl(gf::MouseButton::Left);
	addAction(m_triggerAction);

	btnClicked.setBuffer(gAudioManager().getSound("sounds/ClickButton.ogg"));
	btnClicked.setVolume(FxsVolume);

	auto setupButton = [&] (gf::TextButtonWidget& button, auto callback) {
		button.setDefaultTextColor(gf::Color::Black);
		button.setDefaultBackgroundColor(gf::Color::Gray(0.7f));
		button.setSelectedTextColor(gf::Color::Black);
		button.setSelectedBackgroundColor(gf::Color::Red);
		button.setDisabledTextColor(gf::Color::Black);
		button.setDisabledBackgroundColor(gf::Color::Red);
		button.setAnchor(gf::Anchor::TopLeft);
		button.setAlignment(gf::Alignment::Center);
		button.setCallback(callback);
		m_widgets.addWidget(button);
	};

	setupButton(m_resumeButton, [&] () {
		gf::Log::debug("Resume button pressed!\n");
		btnClicked.play();
		m_game.popScene(); 
	});

	setupButton(m_quitButton, [&] () {
		gf::Log::debug("Quit button pressed!\n");
		btnClicked.play();
		m_game.replaceAllScenes(*m_game.menu);
		gBackgroundMusic.play();
	});
}

void QuitScene::doProcessEvent(gf::Event& event) {
	switch (event.type) {
		case gf::EventType::MouseMoved:
			m_widgets.pointTo(m_game.computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));
			break;
		default :
			break;
	}
}

void QuitScene::doHandleActions([[maybe_unused]] gf::Window& window) {
	if (!isActive()) {
		return;
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
}

void QuitScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
	constexpr float characterSize = 0.075f;
	constexpr float spaceBetweenButton = 0.05f;
	constexpr gf::Vector2f backgroundSize(0.5f, 0.3f);

	target.setView(getHudView());
	
	gf::RoundedRectangleShape backgroundT(target.getSize());
	backgroundT.setColor(gf::Color::fromRgba32(0,0,0,200));

	gf::Coordinates coords(target);

	const gf::Vector2f backgroundRelativeSize = coords.getRelativeSize(backgroundSize);
	const float paragraphWidth = coords.getRelativeSize(backgroundSize - 0.05f).x;
	const float paddingSize = coords.getRelativeSize({0.01f, 0.f}).x;
	const unsigned resumeCharacterSize = coords.getRelativeCharacterSize(characterSize);

	gf::RectangleShape background;
	background.setPosition(coords.getRelativePoint({0.5f, 0.5f}));
	background.setSize(backgroundRelativeSize);
	background.setAnchor(gf::Anchor::Center);
	background.setColor(gf::Color::Gray(0.2f));
	background.draw(target, states);

	m_resumeButton.setCharacterSize(resumeCharacterSize);
	m_resumeButton.setPosition(coords.getRelativePoint({0.275f, 0.44f}));
	m_resumeButton.setParagraphWidth(paragraphWidth);
	m_resumeButton.setPadding(paddingSize);

	constexpr float offset = characterSize + spaceBetweenButton;

	m_quitButton.setCharacterSize(resumeCharacterSize);
	m_quitButton.setPosition(coords.getRelativePoint({0.275f, 0.44f + offset}));
	m_quitButton.setParagraphWidth(paragraphWidth);
	m_quitButton.setPadding(paddingSize);
	
	target.draw(backgroundT, states);
	m_widgets.render(target, states);
}

void QuitScene::doShow() {
	m_widgets.clear();

	m_resumeButton.setDefault();
	m_widgets.addWidget(m_resumeButton);


	m_quitButton.setDefault();
	m_widgets.addWidget(m_quitButton);

	m_widgets.selectNextWidget();
}

void QuitScene::onActivityChange(bool active){
	if(active) {
		btnClicked.setVolume(FxsVolume);
	}
}