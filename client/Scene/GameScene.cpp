#include "GameScene.hpp"
#include "../GameHub.hpp"

#include <string>

#include "../Singletons.hpp"

GameScene::GameScene(GameHub& game, Network &network, GameData &gameData)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_network(network)
, m_gameData(gameData)
, m_quitAction("quit")
, m_fullscreenAction("Fullscreen")
, m_endTurnAction("EndTurnAction")
, m_playCardAction("playCardAction")
, m_triggerAction("TriggerAction")
, m_poseActionRight("PoseActionRight")
, m_poseActionLeft("PoseActionLeft")
, m_discardActionRight("DiscardActionRight")
, m_boardEntity(game.resources, m_gameData)
, m_mainEntity(game.resources, m_gameData)
, m_tableBoardEntity(game.resources, m_gameData)
, m_poseEntity(game.resources, m_gameData)
, m_endTurn("Fin tour", game.resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_playCard("Activer carte", game.resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_loading(game.resources.getTexture("images/Loading.png"))
, m_font(game.resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_background(game.resources.getTexture("images/BackgroundGame.png"))
{

	
	setClearColor(gf::Color::Black);
	m_loading.setSmooth(true);
	m_background.setSmooth(true);

	m_animation.addTileset(m_loading, gf::vec(12, 1), gf::milliseconds(400), 12);
	m_animatedSprite.setAnimation(m_animation);
  	m_animatedSprite.setOrigin({ 84.f / 2.0f, 84.f / 2.0f });
	m_animatedSprite.setAnchor(gf::Anchor::Center);

	m_poseRight =false;
	m_poseLeft = false;
	m_discardRight =false;
	
	m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
	addAction(m_quitAction);

	m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
	addAction(m_fullscreenAction);

	m_endTurnAction.addScancodeKeyControl(gf::Scancode::Return);
	addAction(m_endTurnAction);

	m_playCardAction.addScancodeKeyControl(gf::Scancode::P);
	addAction(m_playCardAction);

	m_triggerAction.addMouseButtonControl(gf::MouseButton::Left);
	addAction(m_triggerAction);

	m_poseActionRight.addKeycodeKeyControl(gf::Keycode::A);
	addAction(m_poseActionRight);
	
	m_poseActionLeft.addKeycodeKeyControl(gf::Keycode::Z);
	addAction(m_poseActionLeft);

	m_discardActionRight.addKeycodeKeyControl(gf::Keycode::E);
	addAction(m_discardActionRight);

	clickEndturn.setBuffer(gAudioManager().getSound("sounds/ClickButton.ogg"));
	clickEndturn.setVolume(FxsVolume);

	clickplayCard.setBuffer(gAudioManager().getSound("sounds/PlayCard.ogg"));
	clickplayCard.setVolume(FxsVolume);

	clock.setBuffer(gAudioManager().getSound("sounds/Clock.ogg"));
	clock.setVolume(FxsVolume);
		
	takePiece.setBuffer(gAudioManager().getSound("sounds/TakePiece.ogg"));
	takePiece.setVolume(FxsVolume);

	movePiece.setBuffer(gAudioManager().getSound("sounds/MovePiece.ogg"));
	movePiece.setVolume(FxsVolume);

	checkPiece.setBuffer(gAudioManager().getSound("sounds/CheckPiece.ogg"));
	checkPiece.setVolume(FxsVolume);

	drawCard.setBuffer(gAudioManager().getSound("sounds/DrawCard.ogg"));
	drawCard.setVolume(FxsVolume);
	
	explosion.setBuffer(gAudioManager().getSound("sounds/Explosion.ogg"));
	explosion.setVolume(FxsVolume);

	m_boardView = gf::ExtendView({ 0, 0 }, { 403, 403 });
	m_boardView.setViewport(gf::RectF::fromPositionSize({ 0.275f, 0.125f}, { 0.45f, 0.45f }));

	m_tableBoardView = gf::ExtendView({ 0, 0 }, { 500, 500 });
	m_tableBoardView.setViewport(gf::RectF::fromPositionSize({ 0.15f, 0.0f}, { 0.7f, 0.7f }));

	m_cardsView = gf::ExtendView({ 0, 0 }, { 1200, 300 });
	m_cardsView.setViewport(gf::RectF::fromPositionSize({ 0.125f, 0.7f}, { 0.75f, 0.3f }));

	m_principalView = gf::ExtendView({ 0, 0 }, { 1600, 900 });
	m_principalView.setViewport(gf::RectF::fromPositionSize({ 0., 0.f}, { 1.f, 1.f }));
	m_principalView.setCenter(game.getRenderer().getSize()/2);

	m_views.addView(m_boardView);
	m_views.addView(m_tableBoardView);
	m_views.addView(m_cardsView);
	m_views.addView(m_principalView);

	m_views.setInitialFramebufferSize({game.getRenderer().getSize()});

	gf::Coordinates coordsWidget(m_game.getRenderer().getSize());
	auto setupButton = [&] (gf::TextButtonWidget& button,  auto callback) {
		button.setDefaultTextColor(gf::Color::fromRgba32(212,30,27,255));
		button.setDefaultBackgroundColor(gf::Color::Gray(0.7f));
		button.setDefaultBackgroundOutlineColor(gf::Color::Gray(0.7f)); 

		button.setSelectedTextColor(gf::Color::fromRgba32(212,30,27,255));
		button.setSelectedBackgroundColor(gf::Color::Gray(0.7f));
		button.setSelectedBackgroundOutlineColor(gf::Color::Gray(0.5f));
		
		button.setDisabledTextColor(gf::Color::Black);
		button.setDisabledBackgroundColor(gf::Color::Gray(0.7f));
		button.setDisabledBackgroundOutlineColor(gf::Color::Red); 

		button.setCallback(callback);
		m_widgets.addWidget(button);
	};

	setupButton(m_endTurn, [&] () {
		if(m_gameData.m_phase.getCurrentPhase()==Phase::APRES_COUP) {
			gf::Log::debug("EndTurn pressed!\n");
			clickEndturn.play();
			m_gameData.m_phase.setCurrentPhase(Phase::PAS_MON_TOUR);
			CardRep cardRep;
			cardRep.err = CodeRep::NO_CARD;
			m_network.send(cardRep);
			m_gameData.m_plateau.turnTo = !m_gameData.m_plateau.turnTo;
			m_poseEntity.returnCardHand();
			m_gameData.m_phase.setCurrentSubPhase(SubPhase::NONE);
			m_gameData.m_plateau.m_casesClicked.clear();
		}
	});

	setupButton(m_playCard, [&] () {
		if(m_poseEntity.m_cardPose.m_num == -1) {
			return;
		}

		CardRep cardRep;
		cardRep.err = CodeRep::CARD;
		cardRep.card = -1; 
		
		cardRep.poses = m_gameData.m_plateau.m_casesClicked;

		for(std::size_t i=0; i < m_gameData.m_main.size(); i++) {
			if(m_gameData.m_main[i].m_num == -1) {
				cardRep.card = i; 
				break;
			}
		}
		
		clickplayCard.play();
		gf::Log::debug("envoie au serveur la card %li\n", cardRep.card);
		m_network.send(cardRep);
	});
}

void GameScene::doHandleActions([[maybe_unused]] gf::Window& window) {
	if (!isActive()) {
		return;
	}

	if (m_quitAction.isActive()) {
		m_game.pushScene(*m_game.quit);
		pause();
	}

	if (m_fullscreenAction.isActive()) {
		window.toggleFullscreen();
	}

	if (m_triggerAction.isActive()) {
		m_widgets.triggerAction();
	}

	if (m_endTurnAction.isActive()){
		gf::Log::debug("pushed endturn\n");
		m_endTurn.triggerCallback();
	}

	if(m_playCardAction.isActive()) {
		gf::Log::debug("pushed playCard\n");
		m_playCard.triggerCallback();
	}

	if(m_poseEntity.m_cardPose.m_num != -1 && m_poseActionRight.isActive()){
		gf::Log::debug("pushed poseRight\n");
		m_poseRight = !m_poseRight;
	}
	
	if(m_poseEntity.m_cardPose.m_num != -1 && m_poseActionLeft.isActive()){
		gf::Log::debug("pushed poseLeft\n");
		m_poseLeft = !m_poseLeft;
	}
	
	if(m_poseEntity.m_cardDiscard.m_num != -1 && m_discardActionRight.isActive()){
		gf::Log::debug("pushed discardRight\n");
		m_discardRight = !m_discardRight;
	}
}

void GameScene::doProcessEvent(gf::Event& event) {
	if(!isActive()) {
		return;
	}
	
	bool clickLeft = false;
	bool clickRight = false;
	bool mouseMoved = false;

	m_views.processEvent(event);
	Card c = Card();
	if(m_poseEntity.m_cardPose.m_num != -1 && m_poseLeft) {
		c = m_poseEntity.m_cardPose;
		m_poseLeft = !m_poseLeft;
	}

	if(m_poseEntity.m_cardDiscard.m_num != -1  && m_discardRight) {
		c = m_poseEntity.m_cardDiscard;
		m_discardRight = !m_discardRight;
	}
	
	if(c.m_num !=-1) {
		m_game.zoomCard->m_cardZoom = c;
		m_game.pushScene(*m_game.zoomCard);
		pause();	
	}


	if(m_poseEntity.m_cardPose.m_num != -1 && m_poseRight) {
		m_poseEntity.returnCardHand();
		m_gameData.m_phase.setCurrentSubPhase(SubPhase::NONE);
		m_gameData.m_plateau.m_casesClicked.clear();
		m_poseRight = !m_poseRight;
		return;
	}

	if (m_gameData.m_gameStatus != ChessStatus::ON_GOING) { return; }
	
	switch (event.type) {
		case gf::EventType::MouseMoved:
			m_widgets.pointTo(m_game.computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));
			mouseMoved = true;
			break;

		case gf::EventType::MouseButtonPressed:
			if(event.mouseButton.button == gf::MouseButton::Left) {
				clickLeft = true;
			}else {
				clickRight = true;
			}
			break;
		default :
			break;
	}

	if(mouseMoved) {
		//gf::Log::error("mousemoved : \n\n /%i/ /%i/", event.mouseButton.coords.x, event.mouseButton.coords.y);
		int numCarte = m_mainEntity.getCardSelected(m_cardsView.getSize(), m_game.getRenderer().mapPixelToCoords(event.mouseCursor.coords, m_cardsView));
		m_mainEntity.m_hoverCard = -1;

		if(numCarte!=-1) {
			//gf::Log::error("mousemoved 2222: \n\n");
			m_mainEntity.m_hoverCard = numCarte;
		}
		return;
	}else {
		m_mainEntity.m_hoverCard = -1;
	}

	if(clickRight) {
		int numCarte = m_mainEntity.getCardSelected(m_cardsView.getSize(), m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords, m_cardsView));

		if(numCarte!=-1) {
			c = m_gameData.m_main[numCarte];
		}
		if(c.m_num !=-1) {
			m_game.zoomCard->m_cardZoom = c;
			m_game.pushScene(*m_game.zoomCard);
			pause();	
		}
	}

	if(!clickLeft) {
		return;
	}


	Phase currentPhase = m_gameData.m_phase.getCurrentPhase();
	

	if(m_gameData.m_phase.getCurrentSubPhase() == SubPhase::CLIQUER_CASES) {
		gf::Vector2i v = m_boardEntity.getCaseSelected(m_boardView.getSize(), m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords, m_boardView));
		if(v.x == -1 || v.y == -1) {
			return;
		}
		
		auto it = std::find(m_gameData.m_plateau.m_casesClicked.begin(), m_gameData.m_plateau.m_casesClicked.end(), v);
		if(it != m_gameData.m_plateau.m_casesClicked.end()) {
			m_gameData.m_plateau.m_casesClicked.erase(it);
			return;
		}
		
		if(m_gameData.m_plateau.m_casesClicked.size() < m_poseEntity.m_cardPose.m_nbClickPossible) {
			m_gameData.m_plateau.m_casesClicked.push_back(v);
		}
		return;
	}

	if((currentPhase == Phase::APRES_COUP || currentPhase == Phase::AVANT_COUP) && !m_gameData.m_plateau.m_promotion) {
		int numCarte = m_mainEntity.getCardSelected(m_cardsView.getSize(), m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords, m_cardsView));

		if(numCarte!=-1) {

			bool playable = m_gameData.m_main[numCarte].m_isPlayable(m_gameData.m_plateau, currentPhase);

			gf::Log::info("carte %i est jouable %i \n", numCarte, playable);

			if(playable && m_poseEntity.m_cardPose.m_num == -1) {
				m_poseEntity.m_cardPose = m_gameData.m_main[numCarte];
				if(m_gameData.m_main[numCarte].m_action == Action::CHOOSE_CASES) {
					m_gameData.m_phase.setCurrentSubPhase(SubPhase::CLIQUER_CASES);
				}
				m_gameData.m_main[numCarte] = Card();
				m_gameData.m_plateau.coordCaseSelected = gf::Vector2i(-1);
				m_gameData.m_plateau.moveAvailable.clear();
			}
		}
	}
	
	if(currentPhase==Phase::COUP || (currentPhase==Phase::AVANT_COUP  && m_poseEntity.m_cardPose.m_num==-1)) { 
		gf::Vector2i v = m_boardEntity.getCaseSelected(m_boardView.getSize(), m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords, m_boardView));
		if(v.x == -1 || v.y == -1) {
			gf::Log::info("clique en dehors plateau\n");
			return;
		}

		if(m_gameData.m_plateau.m_promotion) {
			
			PromotionRep promo;

			promo.pos.x = v.x;
			promo.pos.y = (m_gameData.m_myColor == ChessColor::WHITE ? 0: 7);

			ChessPiece choice = m_boardEntity.getChoice(m_boardView.getSize(),v);
			promo.choice = choice;
			gf::Log::debug("envoie au serveur la promotion du pion en %i,%i avec comme choice %i\n",v.y,v.x,(int)promo.choice);
			m_network.send(promo);
		   
		}else {
			
			bool coupPionEnded = m_gameData.m_plateau.setMovement(v);
			if(m_gameData.m_plateau.coordCaseSelected != gf::Vector2i(-1) && !coupPionEnded) {
				takePiece.play();
			}

			if(coupPionEnded) {
				CoupRep coup;
				coup.posStart.x = m_gameData.m_plateau.coordCaseSelected.x;
				coup.posStart.y = m_gameData.m_plateau.coordCaseSelected.y;
				coup.posEnd.x = v.x;
				coup.posEnd.y = v.y;
				
				m_gameData.m_plateau.coordCaseSelected = gf::Vector2i(-1,-1);
				m_gameData.m_plateau.moveAvailable.clear();	
					
				m_network.send(coup);
			}
		}
	}
}

void GameScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
	gf::Coordinates coord(target);
	target.setView(getHudView());

	float backgroundHeight = coord.getRelativeSize(gf::vec(0.0f, 1.0f)).height;
	float backgroundScale = backgroundHeight / m_background.getSize().height;

	gf::Sprite background(m_background);
	background.setPosition(coord.getCenter());
	background.setAnchor(gf::Anchor::Center);
	background.setScale(backgroundScale);
	//target.draw(background, states);

	
	
	
	if(m_gameData.m_gameStatus == ChessStatus::NO_STARTED) {
		gf::Text text("En attente d un autre joueur", m_font, 50);
		text.setPosition(coord.getRelativePoint({0.5f, 0.5f }));	
		text.setAnchor(gf::Anchor::Center);
		text.setColor(gf::Color::White);
		m_animatedSprite.setPosition(coord.getRelativePoint({0.5f, 0.6f }));
		target.draw(text, states);
		target.draw(m_animatedSprite, states);
		return;
	}
	
	if(m_gameData.m_gameStatus == ChessStatus::STOP_WATCH) {
		std::string indaction = "La partie commence dans " + std::to_string((int)startTime) + " secondes ...\n Vous jouez les " + ((m_gameData.m_myColor==ChessColor::WHITE) ? "Blancs" : "Noirs");
		gf::Text text(indaction, m_font, 50);
		text.setColor(gf::Color::White);
		text.setPosition(coord.getRelativePoint({0.5f, 0.5f }));	
		text.setAlignment(gf::Alignment::Center);
		text.setParagraphWidth(1600.f);
		text.setLineSpacing(2.f);
		text.setAnchor(gf::Anchor::Center);
		
		target.draw(text, states);
		return;
	}
	
	m_endTurn.setPosition( coord.getRelativePoint({0.4f, 0.025f}) );
	m_playCard.setPosition( coord.getRelativePoint({0.6f, 0.025f}) );


	m_endTurn.setRadius(5.f);
	m_endTurn.setBackgroundOutlineThickness(4.f);
	m_endTurn.setAnchor(gf::Anchor::TopCenter);

	m_playCard.setRadius(5.f);
	m_playCard.setBackgroundOutlineThickness(4.f);
	m_playCard.setAnchor(gf::Anchor::TopCenter);

	target.setView(m_tableBoardView);
	m_tableBoardEntity.render(target, states);

	target.setView(m_boardView);
	m_boardEntity.render(target, states);

	target.setView(m_cardsView);
	m_mainEntity.render(target, states);

	target.setView(getHudView());
	m_poseEntity.render(target, states);

	m_widgets.render(target, states);
}

void GameScene::doUpdate(gf::Time time) {
	if(m_gameData.m_gameStatus == ChessStatus::NO_STARTED) {
		m_animatedSprite.update(time);
	}

	if(m_gameData.m_gameStatus == ChessStatus::STOP_WATCH) {
		startTime -= time.asSeconds();
		if(startTime < 0) {
			m_gameData.m_gameStatus = ChessStatus::ON_GOING;
			clock.stop();
		}
	}
	
	if(!m_network.m_queue.poll(m_packet)) {
		return;
	}
	
	//Phase currentPhase = m_gameData.m_phase.getCurrentPhase();

	if (m_packet.getType() == PartieRep::type) {

		auto repPartie = m_packet.as<PartieRep>();
		if (repPartie.err == CodeRep::GAME_START) {
			gf::Log::info("Jeux commence\n");
			m_gameData.m_gameStatus = ChessStatus::STOP_WATCH;
			startTime = 3;
			
			gBackgroundMusic.stop();
			clock.play();
		} else if (repPartie.err == CodeRep::GAME_END) {
			gf::Log::info("Jeux Fini\n");

			if(repPartie.status == ChessStatus::WIN && repPartie.colorPion != m_gameData.m_myColor) {
				m_gameData.m_gameStatus = ChessStatus::LOOSE;	
			}else {
				m_gameData.m_gameStatus = repPartie.status;
			}

		} else if(repPartie.err == CodeRep::COLOR) {
			auto repPartie = m_packet.as<PartieRep>();

			m_gameData.m_myColor = repPartie.colorPion;

			if (m_gameData.m_myColor == ChessColor::WHITE) {
				m_gameData.m_phase.setCurrentPhase(Phase::AVANT_COUP);
			}
			else {
				m_gameData.m_phase.setCurrentPhase(Phase::PAS_MON_TOUR);
			}

			gf::Log::info("Vous jouez la couleur : %i\n", (int)m_gameData.m_myColor);
			m_boardView.setRotation(0.f);
			
			if(m_gameData.m_myColor == ChessColor::BLACK) {
				m_boardView.setRotation(gf::Pi);
			}
		}else if(repPartie.err == CodeRep::TURN_START) {
			gf::Log::info("mon tour commence\n");
			m_gameData.m_phase.setCurrentPhase(Phase::AVANT_COUP);
			assert(m_gameData.m_phase.getNbCardPlay()==0);
			m_gameData.m_plateau.turnTo = m_gameData.m_myColor;
		}
	}	 

	if(m_packet.getType() == CoupRep::type) {
		gf::Log::info("coup recu serveur\n");
		auto coupRep = m_packet.as<CoupRep>();
	
		// move piece
		if(coupRep.err == CodeRep::NONE) { // coup valide
			
			gf::Log::debug("------COUP CORRECT------\n");
			Piece pieceStart =  m_gameData.m_plateau.state[coupRep.posStart.y * 8 + coupRep.posStart.x].piece;
			pieceStart.isMoved= true;

			m_gameData.m_plateau.movePieces(gf::Vector2i(coupRep.posStart.x, coupRep.posStart.y), gf::Vector2i(coupRep.posEnd.x, coupRep.posEnd.y));

			bool playerInEchecB = m_gameData.m_plateau.playerInEchec;
			m_gameData.m_plateau.playerInEchec = m_gameData.m_plateau.isInEchec(!m_gameData.m_plateau.turnTo);

			if(!playerInEchecB && m_gameData.m_plateau.playerInEchec) {
				checkPiece.play();
			}else {
				movePiece.play();
			}

			m_gameData.m_plateau.prettyPrint();
			
			m_gameData.m_plateau.lastCoup.push_back(gf::Vector2i(coupRep.posStart.x,coupRep.posStart.y));
			m_gameData.m_plateau.lastCoup.push_back(gf::Vector2i(coupRep.posEnd.x,coupRep.posEnd.y));
			
			m_gameData.m_plateau.allPositions.push_back(m_gameData.m_plateau.getFen());	
			m_poseEntity.returnCardHand();

			if(pieceStart.getType() == ChessPiece::PAWN &&( coupRep.posEnd.y == 0 || coupRep.posEnd.y == 7)) {
				m_gameData.m_plateau.m_promotion = true;
			}else {
				if(m_gameData.m_phase.getCurrentPhase() != Phase::PAS_MON_TOUR) {
					
					if(m_gameData.m_phase.getNbCardPlay() >= m_gameData.m_phase.getLimNbCard()) {
						m_gameData.m_phase.setCurrentPhase(Phase::PAS_MON_TOUR);
						m_gameData.m_plateau.turnTo = !m_gameData.m_plateau.turnTo;	
					}else {
						m_gameData.m_phase.setCurrentPhase(Phase::APRES_COUP);
					}
				}
				m_gameData.m_plateau.m_promotion = false;
			}
			
			
		}else if(coupRep.err == CodeRep::COUP_NO_VALIDE) {
			gf::Log::debug("------COUP INVALIDE------\n");
		}
	}

	if(m_packet.getType() == PromotionRep::type) {
		gf::Log::info("promo recu serveur\n");
		//assert(m_gameData.m_plateau.m_promotion);

		auto promoRep = m_packet.as<PromotionRep>();
		if(promoRep.err == CodeRep::NONE) {
			gf::Log::debug("------PROMOTION CORRECT------\n");
			
			m_gameData.m_plateau.promotionPiece(gf::Vector2i(promoRep.pos.x, promoRep.pos.y), promoRep.choice);

			bool playerInEchecB = m_gameData.m_plateau.playerInEchec;
			m_gameData.m_plateau.playerInEchec = m_gameData.m_plateau.isInEchec(!m_gameData.m_plateau.turnTo);

			if(!playerInEchecB && m_gameData.m_plateau.playerInEchec) {
				checkPiece.play();
			}
			
			m_gameData.m_plateau.allPositions.push_back(m_gameData.m_plateau.getFen());
			m_poseEntity.returnCardHand();

			m_gameData.m_plateau.prettyPrint();
			if(m_gameData.m_phase.getCurrentPhase() != Phase::PAS_MON_TOUR) {
				assert(m_gameData.m_plateau.m_promotion);
				if(m_gameData.m_phase.getNbCardPlay() >= m_gameData.m_phase.getLimNbCard()) {

					m_gameData.m_phase.setCurrentPhase(Phase::PAS_MON_TOUR);
					m_gameData.m_plateau.turnTo = !m_gameData.m_plateau.turnTo;	
				}else {
					m_gameData.m_phase.setCurrentPhase(Phase::APRES_COUP);
				}
			}
			m_gameData.m_plateau.m_promotion = false;
		}else {
			gf::Log::debug("------PROMOTION INVALIDE------\n");
		}
	}

	if(m_packet.getType() == DistribRep::type) {
		gf::Log::info("deck recu serveur\n");
		
		auto deckRep = m_packet.as<DistribRep>();
		
		int j = 0;
		if(deckRep.err == CodeRep::NONE) {	
			for(int i=0; i< 5; i++) {	
				if(m_gameData.m_main[i].m_num == -1) {
					m_gameData.m_main[i] = m_gameData.m_cards[deckRep.hand[j].m_num];
					j++;
				}
			}
		}
	}

	if(m_packet.getType() == CardRep::type) {
		gf::Log::info("recu carte serveur\n");
		
		auto carteRep = m_packet.as<CardRep>();
		
		if(carteRep.err == CodeRep::NONE) {
			gf::Log::debug("------CARTE VALIDE------ %i\n", carteRep.num);

			m_gameData.m_cards[carteRep.num].m_execute(m_gameData.m_plateau, carteRep.poses);
			
			m_gameData.m_plateau.playerInEchec = m_gameData.m_plateau.isInEchec(!m_gameData.m_plateau.turnTo);

			m_gameData.m_plateau.allPositions.push_back(m_gameData.m_plateau.getFen());

			m_poseEntity.m_cardDiscard = m_gameData.m_cards[carteRep.num];
			m_gameData.m_plateau.m_casesClicked.clear();

			if(m_gameData.m_phase.getCurrentPhase() != Phase::PAS_MON_TOUR) {
				m_gameData.m_phase.nextPhaseCard(m_gameData.m_cards[carteRep.num]);
				
				m_gameData.m_phase.setCurrentSubPhase(SubPhase::NONE);

				if(m_gameData.m_plateau.hasPromotion()) {
					m_gameData.m_phase.setCurrentPhase(Phase::COUP);
					m_gameData.m_plateau.m_promotion = true;
				}

				m_poseEntity.m_cardPose = Card();
				if(m_gameData.m_phase.getCurrentPhase() == Phase::PAS_MON_TOUR) { //carte mais fin a mon tour
					m_gameData.m_plateau.turnTo = !m_gameData.m_plateau.turnTo;	
				}

				clickplayCard.stop();
				drawCard.play();
			}
			
		}else {
			gf::Log::debug("------CARTE INVALIDE------\n");
		}	
	}

	//mode debug
	if(m_packet.getType() == Debug::type) {
		gf::Log::info("\n\nrecu plateau serveur mode debug \n\n");
		auto p = m_packet.as<Debug>();
		gf::Log::info("\n\nplateau moi: %s \n\nplateau serveur: %s \n\n",m_gameData.m_plateau.getFen().c_str(), p.plateau.c_str());
		assert(p.plateau==m_gameData.m_plateau.getFen());
	}
}

void GameScene::onActivityChange(bool active) {
	gf::Log::debug("en pause = %d et active = %d\n",isPaused(), isActive());
	if(active && !isPaused()){
		gf::Log::debug("scene game active\n");
		m_views.setInitialFramebufferSize(m_game.getRenderer().getSize());
		clickEndturn.setVolume(FxsVolume);
		clickplayCard.setVolume(FxsVolume);
		clock.setVolume(FxsVolume);
		takePiece.setVolume(FxsVolume);
		movePiece.setVolume(FxsVolume);
		checkPiece.setVolume(FxsVolume);
		drawCard.setVolume(FxsVolume);
		explosion.setVolume(FxsVolume);

	}else if(!active && isPaused()){
		gf::Log::debug("scene game desactive\n");
		m_network.deconnect();
		m_boardView.setRotation(0);
		m_gameData.reset();
		m_poseEntity.m_cardPose = Card();
		m_poseEntity.m_cardDiscard = Card();
	}
}

void GameScene::doShow() {
	m_widgets.clear();

	m_endTurn.setDefault();
	m_playCard.setDefault();

	m_widgets.addWidget(m_endTurn);
	m_widgets.addWidget(m_playCard);
}