//
// Created by ryanm on 4/12/2016.
//


#include "Game.h"
#include "Strategy.h"
#include "Gaming.h"
#include "Strategic.h"

namespace Gaming {

    const char Strategic::STRATEGIC_ID = 'T';

    Gaming::Strategic::Strategic(const Game &g, const Position &p, double energy, Strategy *s) : Agent(g, p, energy) {
        __strategy = s;
        __energy = energy;
    }

    Strategic::~Strategic() {
        delete __strategy;
    }

    void Strategic::print(std::ostream &os) const {
        os << STRATEGIC_ID << __id;
    }

    ActionType Strategic::takeTurn(const Surroundings &s) const {
        //return (*__strategy)(s);
    }
}