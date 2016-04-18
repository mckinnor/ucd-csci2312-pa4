//
// Created by ryanm on 4/12/2016.
//


#include "Game.h"
#include "Gaming.h"
#include "Advantage.h"

namespace  Gaming {

    const double Advantage::ADVANTAGE_MULT_FACTOR = 2.0;
    //const char ADVANTAGE_ID = 'D';

    Advantage::Advantage(const Game &g, const Position &p, double capacity) : Resource(g, p, capacity) {

    }

    Advantage::~Advantage() {

    }

    void Advantage::print(std::ostream &os) const {
        os << ADVANTAGE_ID << __id;
    }

    double Advantage::getCapacity() const {
        return __capacity * ADVANTAGE_MULT_FACTOR;
    }

    double Advantage::consume() {
        double ret = getCapacity();
        __capacity = -1;
        finish();
        //std::cout << "Consuming: D" << __id << std::endl;
        return ret;
    }
}