//
// Created by ryanm on 4/12/2016.
//

#include <chrono>
#include "Game.h"
#include "Gaming.h"
#include "Simple.h"

namespace Gaming {
    const char Simple::SIMPLE_ID = 'S';

    Simple::Simple(const Game &g, const Position &p, double energy) : Agent(g, p, energy) {

    }

    Simple::~Simple() {
    }

    void Simple::print(std::ostream &os) const {
        os << SIMPLE_ID << __id;
    }

    ActionType Simple::takeTurn(const Surroundings &s) const {
        std::vector<int> positions;
        unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine rnd(seed);

        for (int i = 0; i < 9; ++i) {
            //std::cout << "Surr of " << __id << ": i: " << i << ": " << s.array[i] << std::endl;
            if (s.array[i] == PieceType::ADVANTAGE || s.array[i] == PieceType::FOOD) {
                //std::cout << "Pushing advantage" << std::endl;
                positions.push_back(i);
                //std::cout << "size of positions: " << positions.size() << std::endl;
            }
        }
        //Check for Empty
        if (positions.size() == 0) {
            for (int i = 0; i < 9; ++i) {
                //std::cout << "Surrounding[i]: " << s.array[i] << std::endl;
                if (s.array[i] == PieceType::EMPTY) {
                    positions.push_back(i);
                    //std::cout << "Available position: " << i << std::endl;
                }
            }
        }

        if (positions.size() > 0) {
            int posIndex = positions[rnd() % positions.size()];
            if (positions.size() == 1) posIndex = positions[0];

            //std::cout << "Chosen Position: " << posIndex << std::endl;
            ActionType ac;
            switch (posIndex) {
                case 0: ac = NW; break;
                case 1: ac = N; break;
                case 2: ac = NE; break;
                case 3: ac = W; break;
                case 4: ac = STAY; break;
                case 5: ac = E; break;
                case 6: ac = SW; break;
                case 7: ac = S; break;
                case 8: ac = SE; break;
                default: ac = STAY;
            }
            //std::cout << "Chosen Action: " << ac << std::endl;
            return (ac);
        }

        return ActionType::STAY;
    }
}