//
// Created by ryanm on 4/11/2016.
//

#include <iomanip>
#include <sstream>
#include <set>
#include "Game.h"
#include "Simple.h"
#include "Strategic.h"
#include "Food.h"
#include "Advantage.h"

namespace Gaming {

    const unsigned int Game::NUM_INIT_AGENT_FACTOR = 4;
    const unsigned int Game::NUM_INIT_RESOURCE_FACTOR = 2;
    const unsigned Game::MIN_HEIGHT = 3;
    const unsigned Game::MIN_WIDTH = 3;
    const double Game::STARTING_AGENT_ENERGY = 20;
    const double Game::STARTING_RESOURCE_CAPACITY = 10;

    PositionRandomizer Game::__posRandomizer = PositionRandomizer();

    Game::Game() {
        __width = 3;
        __height = 3;
        __numInitAgents = 0;
        __numInitResources = 0;
        __round = 0;
        int i = 0;
        while(i < 9){
            __grid.push_back(nullptr);
            i++;
        }
        __verbose = false;
    }

    Game::Game(unsigned width, unsigned height, bool manual) {
        if(width < MIN_WIDTH || height < MIN_HEIGHT){
            throw InsufficientDimensionsEx(MIN_WIDTH, MIN_HEIGHT, width, height);
        }
        else {
            __width = width;
            __height = height;
            int i = 0;
            while(i < width*height){
                __grid.push_back(nullptr);
                i++;
            }
            __round = 0;
            __status = NOT_STARTED;
        }

        if(!manual){
            populate();
        }
    }

    Game::~Game() {
        __numInitAgents = 0;
        __numInitResources = 0;
    }

    void Game::populate() {
        std::default_random_engine gen;
        std::uniform_int_distribution<int> d(0, __width * __height);

        __numInitAgents = (__width * __height) / NUM_INIT_AGENT_FACTOR;
        __numInitResources = (__width * __height) / NUM_INIT_RESOURCE_FACTOR;
        unsigned int numStrategic = __numInitAgents / 2;
        unsigned int numSimple = __numInitAgents - numStrategic;
        unsigned int numAdvantage = __numInitResources / 2;
        unsigned int numFood = __numInitResources - numAdvantage;

// populate Strategic agents
        while (numStrategic > 0) {
            int i = d(gen); // random index in the grid vector
            if (__grid[i] == nullptr) { // is position empty
                Position pos(i / __width, i % __width);
                __grid[i] = new Strategic(*this, pos, STARTING_AGENT_ENERGY, new DefaultAgentStrategy);
                numStrategic --;
            }
        }

        while (numSimple > 0) {
            int i = d(gen); // random index in the grid vector
            if (__grid[i] == nullptr) { // is position empty
                Position pos(i / __width, i % __width);
                __grid[i] = new Simple(*this, pos, STARTING_AGENT_ENERGY);
                numSimple --;
            }
        }

        while (numFood > 0){
            int i = d(gen);
            if(__grid[i] == nullptr){
                Position pos(i / __width, i % __width);
                __grid[i] = new Food(*this, pos, STARTING_RESOURCE_CAPACITY);
                numFood--;
            }
        }

        while(numAdvantage > 0){
            int i = d(gen);
            if(__grid[i] == nullptr){
                Position pos(i/__width, i%__width);
                __grid[i] = new Food(*this, pos, STARTING_RESOURCE_CAPACITY);
                numAdvantage--;
            }
        }

    }

    unsigned int Game::getNumSimple() const {
        unsigned int numAgents = 0;

        for (auto it = __grid.begin(); it != __grid.end(); ++it) {
            Simple *simple = dynamic_cast<Simple*>(*it);
            if (simple) numAgents ++;
        }

        return numAgents;
    }

    unsigned int Game::getNumStrategic() const {
        unsigned int numAgents = 0;

        for (auto it = __grid.begin(); it != __grid.end(); ++it) {
            Strategic *simple = dynamic_cast<Strategic*>(*it);
            if (simple) numAgents ++;
        }

        return numAgents;
    }

    const Piece *Game::getPiece(unsigned int x, unsigned int y) const {
        if (y >= __width || x >= __height) throw OutOfBoundsEx(__width, __height, x, y);
        if (__grid[y + (x * __width)] == nullptr) throw PositionEmptyEx(x, y);
        return __grid[y + (x * __width)];
    }

    void Game::addSimple(const Position &position) {
        int gameIndex = 0;
        gameIndex = (position.x * __width) + position.y;
        //if(gameIndex > (__width * __height))
        if(position.y >= __width || position.x >= __height)
            throw OutOfBoundsEx(__width, __height, position.x, position.y);
        else if (__grid[gameIndex] != nullptr)
            throw PositionNonemptyEx(position.x, position.y);
        else {
            __grid[gameIndex] = new Simple(*this, position, STARTING_AGENT_ENERGY);
            __numInitAgents++;
        }
    }

    void Game::addSimple(const Position &position, double energy) {
        int gameIndex = 0;
        gameIndex = (position.x * __width) + position.y;
        if(position.y >= __width || position.x >= __height)
            throw OutOfBoundsEx(__width, __height, position.x, position.y);
        else if (__grid[gameIndex] != nullptr)
            throw PositionNonemptyEx(position.x, position.y);
        else {
            __grid[gameIndex] = new Simple(*this, position, energy);
            __numInitAgents++;
        }
    }

    void Game::addSimple(unsigned x, unsigned y) {
        int gameIndex = (x * __width) + y;
        //gameIndex =
        Position p(x, y);
        if(y >= __width || x  >=__height)
            throw OutOfBoundsEx(__width, __height, x, y);
        else if (__grid[gameIndex] != nullptr)
            throw PositionNonemptyEx(x, y);

        else {
            __grid[gameIndex] = new Simple(*this, p, STARTING_AGENT_ENERGY);
            __numInitAgents++;
        }
    }

    void Game::addSimple(unsigned x, unsigned y, double energy) {
        int gameIndex = 0;
        gameIndex = (x * __width) + y;
        Position p(x, y);
        if(y >= __width || x  >=__height)
            throw OutOfBoundsEx(__width, __height, x, y);
        else if (__grid[gameIndex] != nullptr)
            throw PositionNonemptyEx(x, y);

        else {
            __grid[gameIndex] = new Simple(*this, p, energy);
            __numInitAgents++;
        }
    }

    void Game::addStrategic(const Position &position, Strategy *s) {
        int gameIndex = 0;
        gameIndex = (position.x * __width) + position.y;
        if(position.y >= __width || position.x >= __height)
            throw OutOfBoundsEx(__width, __height, position.x, position.y);
        else if (__grid[gameIndex] != nullptr)
            throw PositionNonemptyEx(position.x, position.y);

        else {
            __grid[gameIndex] = new Strategic(*this, position, STARTING_AGENT_ENERGY, s);
            __numInitAgents++;
        }
    }

    void Game::addStrategic(unsigned x, unsigned y, Strategy *s) {
        int gameIndex = 0;
        gameIndex = (x * __width) + y;
        Position p(x, y);
        if(y >= __width || x  >=__height)
            throw OutOfBoundsEx(__width, __height, x, y);
        else if (__grid[gameIndex] != nullptr)
            throw PositionNonemptyEx(x, y);
        else {
            __grid[gameIndex] = new Strategic(*this, p, STARTING_AGENT_ENERGY, s);
            __numInitAgents++;
        }
    }

    void Game::addFood(const Position &position) {
        int gameIndex = 0;
        gameIndex = (position.x*__width) + position.y;
        if(position.y >= __width || position.x >= __height)
            throw OutOfBoundsEx(__width, __height, position.x, position.y);
        else if(__grid[gameIndex] != nullptr)
            throw PositionNonemptyEx(position.x, position.y);

        else {
            __grid[gameIndex] = new Food(*this, position, STARTING_RESOURCE_CAPACITY);
            __numInitResources++;
        }
    }

    void Game::addFood(unsigned x, unsigned y) {
        int gameIndex = 0;
        gameIndex = (x * __width) + y;
        Position p(x, y);
        if(y >= __width || x  >=__height)
            throw OutOfBoundsEx(__width, __height, x, y);
        else if (__grid[gameIndex] != nullptr)
            throw PositionNonemptyEx(x, y);

        else {
            __grid[gameIndex] = new Food(*this, p, STARTING_RESOURCE_CAPACITY);
            __numInitResources++;
        }
    }

    void Game::addAdvantage(const Position &position) {
        int gameIndex = 0;
        gameIndex = (position.x * __width) + position.y;
        if(position.y >= __width || position.x >= __height)
            throw OutOfBoundsEx(__width, __height, position.x, position.y);
        else if (__grid[gameIndex] != nullptr)
            throw PositionNonemptyEx(position.x, position.y);

        else {
            __grid[gameIndex] = new Advantage(*this, position, STARTING_RESOURCE_CAPACITY);
            __numInitResources++;
        }
    }

    void Game::addAdvantage(unsigned x, unsigned y) {
        int gameIndex = 0;
        gameIndex = (x*__width) + y;
        Position p (x,y);
        if(y >= __width || x  >=__height)
            throw OutOfBoundsEx(__width, __height, x, y);

        else if(__grid[gameIndex] != nullptr)
            throw PositionNonemptyEx(x, y);

        else {
            __grid[gameIndex] = new Advantage(*this, p, STARTING_RESOURCE_CAPACITY);
            __numInitResources++;
        }
    }

    const Surroundings Game::getSurroundings(const Position &pos) const {
        Surroundings surr;
        for(int i = 0; i < 9; i++)
            surr.array[i] = EMPTY;

        for(int row = -1; row <= 1; row++){
            for(int colum = -1; colum <= 1; colum++){
                int gameIndex = ((pos.x+row)*__width) + (pos.y+colum);
                if((pos.x + row >= 0 && pos.x+row < __height)&&(pos.y + colum >= 0 && pos.y + colum < __width)) {
                    if (row == 0 && colum == 0) surr.array[((row + 1) * 3) + (colum + 1)] = SELF;
                    else if (__grid[gameIndex])
                        surr.array[((row + 1) * 3) + (colum + 1)] = __grid[gameIndex]->getType();
                }
                else
                    surr.array[((row + 1) * 3) + (colum + 1)] = INACCESSIBLE;
            }
        }
        return surr;
    }

    const ActionType Game::reachSurroundings(const Position &from, const Position &to) {
        int x, y;

        x = from.x - to.x;
        y = from.y - to.y;

        if(x == -1 && y == 0) return ActionType::S;
        if(x == -1 && y == 1) return ActionType::SW;
        if(x == -1 && y == -1) return ActionType::SE;
        if(x == 0 && y == -1) return  ActionType::E;
        if(x == 0 && y == 1) return ActionType::W;
        if(x == 1 && y == 0) return ActionType::N;
        if(x == 1 && y == 1) return ActionType::NW;
        if(x == 1 && y == -1) return ActionType::NE;
        else return ActionType::STAY;


    }

    bool Game::isLegal(const ActionType &ac, const Position &pos) const {
        int x = pos.x, y = pos.y;
        if(ac == ActionType::N) {
            --x;
        }
        if(ac == ActionType::NE) {
            x--;
            y++;
        }
        if(ac == ActionType::NW) {
            x--;
            y--;
        }
        if(ac == ActionType::W) {
            y--;
        }
        if(ac == ActionType::E) {
            y++;
        }
        if(ac == ActionType::S) {
            x++;
        }
        if(ac == ActionType::SE) {
            x++;
            y++;
        }
        if(ac == ActionType::SW) {
            x++;
            y--;
        }
        if(ac == ActionType::STAY) {
            x = pos.x;
            y = pos.y;
        }

        if(x >= 0 && x < __height && y >= 0 && y < __width) return true;
        else return false;

    }

    const Position Game::move(const Position &pos, const ActionType &ac) const {
        if(isLegal(ac, pos)){
            int x = pos.x, y = pos.y;
            if(ac == ActionType::N) {
                --x;
            }
            if(ac == ActionType::NE) {
                x--;
                y++;
            }
            if(ac == ActionType::NW) {
                x--;
                y--;
            }
            if(ac == ActionType::W) {
                y--;
            }
            if(ac == ActionType::E) {
                y++;
            }
            if(ac == ActionType::S) {
                x++;
            }
            if(ac == ActionType::SE) {
                x++;
                y++;
            }
            if(ac == ActionType::SW) {
                x++;
                y--;
            }
            if(ac == ActionType::STAY) {
                x = pos.x;
                y = pos.y;
            }
            Position p((unsigned) x,(unsigned) y);
            return p;
        }
        else return pos;
    }

    void Game::round() {
        std::set<Piece *> pieces;
        for (auto it = __grid.begin(); it != __grid.end(); ++it) {
            if (*it) {
                pieces.insert(pieces.end(), *it);
                //pieces.insert(*it);
                (*it)->setTurned(false);
            }
        }
        // Take turns
        for (auto it = pieces.begin(); it != pieces.end(); ++it) {
            if (!(*it)->getTurned()) {
                (*it)->setTurned(true);
                (*it)->age();
                ActionType ac = (*it)->takeTurn(getSurroundings((*it)->getPosition()));
                //std::cout << "------- Game::round -------" << std::endl;
                //std::cout << "Action: " << ac << std::endl;
                Position pos0 = (*it)->getPosition();
                //std::cout << "Pos0: " << pos0.x << "x" << pos0.y << std::endl;
                Position pos1 = move(pos0, ac);
                //std::cout << "Pos1: " << pos1.x << "x" << pos1.y << std::endl;
                if (pos0.x != pos1.x || pos0.y != pos1.y) {
                    Piece *p = __grid[pos1.y + (pos1.x * __width)];
                    if (p) {
                        (*(*it)) * (*p);
                        if ((*it)->getPosition().x != pos0.x || (*it)->getPosition().y != pos0.y) {
                            // piece moved
                            __grid[pos1.y + (pos1.x * __width)] = (*it);
                            __grid[pos0.y + (pos0.x * __width)] = p;
                        }
                    } else {
                        // empty move
                        (*it)->setPosition(pos1);
                        __grid[pos1.y + (pos1.x * __width)] = (*it);
                        __grid[pos0.y + (pos0.x * __width)] = nullptr;
                        //std::cout << "position updated of piece" << std::endl;
                    }
                }
            }
        }

        // Update positions and delete
        // Delete invalid first
        for (unsigned int i = 0; i < __grid.size(); ++i) {
            //if (__grid[i]) std::cout << "Piece viable: " << __grid[i]->isViable() << std::endl;
            if (__grid[i] && !(__grid[i]->isViable())) {
                delete __grid[i];
                __grid[i] = nullptr;
            }
            //if (__grid[i]) __grid[i]->age();
        }

        // Update positions of remaining
        /*for (unsigned int i = 0; i < __grid.size(); ++i) {
            Piece *currentPiece = __grid[i];
            if (currentPiece) {
                Position pos = currentPiece->getPosition();
                if (__grid[pos.y + (pos.x * __width)] != currentPiece) {
                    __grid[pos.y + (pos.x * __width)] = currentPiece;
                    __grid[i] = nullptr;
                    //std::cout << "place in __grid changed of a piece" << std::endl;
                }
            }
            i++;
        }*/

        // Check game over
        if (getNumResources() <= 0) {
            __status = Status::OVER;
        }
        __round++;
    }

    void Game::play(bool verbose) {
        /*__verbose = verbose;
        __status = PLAYING;
        std::cout << *this;
        while (__status != OVER) {
            round();
            if (verbose) std::cout << *this;
        }
        if (!verbose) std::cout << *this;*/
    }

    std::ostream &operator<<(std::ostream &os, const Game &game) {
        int multiple = 1;
        os << "Round " << game.__round << std::endl;
        for(int i = 0; i < game.__width*game.__height; i++){
            if(game.__grid[i] == nullptr){
                os << '[' << std::setw(6) << ']';
            }
            else{
                /*std::stringstream line;
                line << game.__grid[i];
                os << '[' << line << std::setw(6-line.str().length()) << ']';*/
                os << '[' << *game.__grid[i] << ']';
            }
            if((i+1) % game.__width == 0){
                os << std::endl;
            }
        }
        os << "Status: ";
        if(game.__status == Game::NOT_STARTED)
            os << "Not Started" << std::endl;
        else if(game.__status == Game::PLAYING)
            os << "Playing..." << std:: endl;
        else if(game.__status == Game::OVER)
            os << "Over!" << std::endl;
        return os;
    }
}







