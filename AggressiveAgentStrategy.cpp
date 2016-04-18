//
// Created by ryanm on 4/12/2016.
//


#include <chrono>
#include "AggressiveAgentStrategy.h"
#include "Game.h"

namespace Gaming {
    const double AggressiveAgentStrategy::DEFAULT_AGGRESSION_THRESHOLD = Game::STARTING_AGENT_ENERGY * 0.75;

    Gaming::AggressiveAgentStrategy::AggressiveAgentStrategy(double agentEnergy) {
        __agentEnergy = agentEnergy;
    }

    Gaming::AggressiveAgentStrategy::~AggressiveAgentStrategy() {

    }

    ActionType Gaming::AggressiveAgentStrategy::operator()(const Surroundings &s) const {
        std::vector<int> positions;
        unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine rnd(seed);

        // Check for Agent
        if (__agentEnergy > DEFAULT_AGGRESSION_THRESHOLD) {
            for (int i = 0; i < 9; ++i) {
                //std::cout << "Surr of " << __id << ": i: " << i << ": " << s.array[i] << std::endl;
                if (s.array[i] == PieceType::SIMPLE || s.array[i] == PieceType::STRATEGIC) {
                    //std::cout << "Pushing advantage" << std::endl;
                    positions.push_back(i);
                    //std::cout << "size of positions: " << positions.size() << std::endl;
                }
            }
        }
        //  Check for Advantage
        if (positions.size() == 0) {
            for (int i = 0; i < 9; ++i) {
                //std::cout << "Surrounding[i]: " << s.array[i] << std::endl;
                if (s.array[i] == PieceType::ADVANTAGE) {
                    positions.push_back(i);
                    //std::cout << "Available position: " << i << std::endl;
                }
            }
        }
        //  Check for Food
        if (positions.size() == 0) {
            for (int i = 0; i < 9; ++i) {
                //std::cout << "Surrounding[i]: " << s.array[i] << std::endl;
                if (s.array[i] == PieceType::FOOD) {
                    positions.push_back(i);
                    //std::cout << "Available position: " << i << std::endl;
                }
            }
        }

        //  Check for Empty
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