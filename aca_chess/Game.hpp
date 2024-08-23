//
//  Engine.hpp
//  aca_chess
//
//  Created by Alex Aramyan on 01.07.24.
//

#ifndef Game_hpp
#define Game_hpp

#include "Board.hpp"

class Game
{
private:
    Board board;
    //
public:
    void init();
    void update();
    void draw();
};

#endif /* Engine_hpp */
