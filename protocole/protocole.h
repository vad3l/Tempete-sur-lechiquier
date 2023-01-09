#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <gf/Id.h>
#include <gf/SerializationOps.h>
#include <gf/Vector.h>

#include "../model/chess.hpp"

using namespace gf::literals;


/* Types d'erreur */
typedef enum {
    NONE,     /* Validation de la requete */
    COUP_NO_VALIDE,      
    GAME_START,
    GAME_END
} CodeRep;

/* struct pos d'une pièce*/

struct Pos {
 	static constexpr gf::Id type = "TCase"_id;
	int y;           /* Ligne de la position d'une piece */
	int x;           /* Colonne de la position d'une piece */
};

template<typename Archive>
  Archive& operator|(Archive& ar, Pos& data) {
    return ar | data.y | data.x;
}


struct PartieRep {
    static constexpr gf::Id type = "PartieRep"_id;
    CodeRep err;                   /* Code de retour */
    ChessColor coulPion;
    ChessStatus status;
};

template<typename Archive>
Archive operator|(Archive& ar, PartieRep& data) {
    return ar | data.err | data.coulPion | data.status;
}


/***********************
**********COUP**********
***********************/
struct CoupReq {
    static constexpr gf::Id type = "CoupReq"_id;
    Pos posStart;
    Pos posEnd;
};

template<typename Archive>
Archive operator|(Archive& ar, CoupReq& data) {
    return ar | data.posStart | data.posEnd;
}

struct CoupRep {
    static constexpr gf::Id type = "CoupRep"_id;
    CodeRep err;                   /* Code de retour */
    Pos posStart;
    Pos posEnd;
};

template<typename Archive>
Archive operator|(Archive& ar, CoupRep& data) {
    return ar | data.err | data.posStart | data.posEnd;
}


/***********************
**********PROMOTION**********
***********************/
struct PromotionReq {
    static constexpr gf::Id type = "PromotionReq"_id;
    Pos pos;
    ChessPiece choix;
};

template<typename Archive>
Archive operator|(Archive& ar, PromotionReq& data) {
    return ar | data.pos | data.choix;
}

struct PromotionRep {
    static constexpr gf::Id type = "PromotionRep"_id;
    CodeRep err;                   /* Code de retour */
    Pos pos;
    ChessPiece choix;
};

template<typename Archive>
Archive operator|(Archive& ar, PromotionRep& data) {
    return ar | data.err | data.pos | data.choix;
}
#endif // PROTOCOLE_H
