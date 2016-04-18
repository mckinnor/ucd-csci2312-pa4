//
// Created by ryanm on 4/12/2016.
//

#include "Game.h"
#include "Piece.h"

namespace Gaming {
    unsigned int Piece::__idGen = 1000;

    Piece::Piece(const Game &g, const Position &p):__game(g),__position(p) {
        __id = __idGen++;
        __finished = false;
        __turned = false;
        setPosition(p);
    }

    Piece::~Piece() {
        __idGen--;
    }

    std::ostream &operator<<(std::ostream &os, const Piece &piece) {
        piece.print(os);
        return os;
    }
}