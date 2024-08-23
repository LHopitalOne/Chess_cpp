//
//  main.cpp
//  aca_chess
//
//  Created by Alex Aramyan on 01.07.24.
//

#include "Game.hpp"

#include <iostream>

int main(int argc, const char * argv[])
{
    Game game;
    
    game.init();
    game.update();
    game.draw();
    
    std::cout << "\n" << std::endl;

    return 0;
}
