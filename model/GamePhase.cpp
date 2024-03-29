#include "GamePhase.hpp"


GamePhase::GamePhase(){
	m_currentPhase = Phase::AVANT_COUP;
	m_currentSubPhase = SubPhase::NONE;
	m_nbCardPlay = 0;
	m_limNbCard = 1;
}

Phase GamePhase::getCurrentPhase() {
	return m_currentPhase;
}

SubPhase GamePhase::getCurrentSubPhase() {
	return m_currentSubPhase;
}

std::size_t GamePhase::getNbCardPlay() {
	return m_nbCardPlay;
}

std::size_t GamePhase::getLimNbCard() {
	return m_limNbCard;
}

void GamePhase::setCurrentPhase(Phase phase) {
	if(phase == Phase::PAS_MON_TOUR || phase == Phase::AVANT_COUP) { // tour suivant
	//	gf::Log::error("\n\nremise a 0\n\n");
		m_nbCardPlay = 0;
		m_limNbCard = 1;
	}  
	m_currentPhase = phase;
}

void GamePhase::setCurrentSubPhase(SubPhase subPhase) {
	m_currentSubPhase = subPhase;
}

void GamePhase::nextPhaseCard(Card cardPlay){
	assert(m_currentPhase!=Phase::COUP);
	assert(m_currentPhase !=Phase::PAS_MON_TOUR);
	m_nbCardPlay +=1;
	
	if(cardPlay.m_action == Action::NONE) {
		if(cardPlay.m_effect == Effect::NONE) {
			if(m_nbCardPlay>=m_limNbCard) {
				m_currentPhase = (Phase)((int)m_currentPhase+1);
			}		
		}else if(cardPlay.m_effect == Effect::REPLACE_COUP) {
		//	gf::Log::error("\n\n debug 1111\n\n");
			m_currentPhase = Phase::PAS_MON_TOUR;
		}else if(cardPlay.m_effect == Effect::REGAME_OTHER_CARTE) {
		//	gf::Log::error("\n\nmise a 3\n\n");
			m_limNbCard=3;
		}
	}

	if(cardPlay.m_action == Action::CHOOSE_CASES) {
		if(cardPlay.m_effect == Effect::NONE) {
			if(m_nbCardPlay>=m_limNbCard) {
				m_currentPhase = (Phase)((int)m_currentPhase+1);
			}		
		}else if(cardPlay.m_effect == Effect::REPLACE_COUP) {
			gf::Log::error("\n\ndebugg 222222\n\n");
			m_currentPhase = Phase::PAS_MON_TOUR;
		}
	}
}
