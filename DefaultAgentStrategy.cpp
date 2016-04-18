//
// Created by ryanm on 4/12/2016.
//


#include <chrono>
#include "DefaultAgentStrategy.h"

namespace Gaming {
    Gaming::DefaultAgentStrategy::DefaultAgentStrategy() {

    }

    Gaming::DefaultAgentStrategy::~DefaultAgentStrategy() {

    }

    ActionType Gaming::DefaultAgentStrategy::operator()(const Surroundings &s) const {
        std::vector<int> positions;
        unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine rnd(seed);

        for (int i = 0; i < 9; ++i) {
            if (s.array[i] == PieceType::ADVANTAGE) {
                positions.push_back(i);
            }
        }
        //  Check for Food
        if (positions.size() == 0) {
            for (int i = 0; i < 9; ++i) {
                if (s.array[i] == PieceType::FOOD) {
                    positions.push_back(i);
                }
            }
        }
        //  Check for Empty
        if (positions.size() == 0) {
            for (int i = 0; i < 9; ++i) {
                if (s.array[i] == PieceType::EMPTY) {
                    positions.push_back(i);
                }
            }
        }
        //  Check for Simple
        if (positions.size() == 0) {
            for (int i = 0; i < 9; ++i) {
                if (s.array[i] == PieceType::SIMPLE) {
                    positions.push_back(i);
                }
            }
        }

        if (positions.size() > 0) {
            int posIndex = positions[rnd() % positions.size()];
            if (positions.size() == 1) posIndex = positions[0];

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
            return (ac);
        }

        return ActionType::STAY;
    }
}