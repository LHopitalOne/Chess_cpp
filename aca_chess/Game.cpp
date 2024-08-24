//
//  Engine.cpp
//  aca_chess
//
//  Created by Alex Aramyan on 01.07.24.
//

#ifndef Game_hpp
#include "Game.hpp"
#endif

#include <iostream>
#include <bitset>
#include <locale>

#include <chrono>

void Game::init()
{
    std::locale::global(std::locale());
    
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            board.set({static_cast<char>('a' + j), static_cast<uint8_t>(i + 1)}, Piece::NONE);
    
//    board.set({'b', 3}, Piece::WHITEROOK);
//    board.set({'c', 3}, Piece::WHITEKING);
//    board.set({'a', 2}, Piece::BLACKKING);
    
    board.set({'h', 4}, Piece::BLACKKING);
    board.set({'g', 1}, Piece::WHITEKING);
    board.set({'g', 5}, Piece::WHITEROOK);
    board.set({'f', 6}, Piece::WHITEROOK);
    
//    board.set({'b', 2}, Piece::WHITEROOK);
//    board.set({'c', 2}, Piece::WHITEKING);
//    board.set({'a', 1}, Piece::BLACKKING);
    
//    board.set({'h', 7}, Piece::BLACKKING);
//    board.set({'g', 6}, Piece::WHITEROOK);
//    board.set({'f', 6}, Piece::WHITEKING);
    
//    board.set({'e', 6}, Piece::WHITEKING);
//    board.set({'e', 7}, Piece::WHITEPAWN);
//    board.set({'g', 4}, Piece::WHITEKNIGHT);
//    board.set({'h', 2}, Piece::BLACKPAWN);
//    board.set({'e', 8}, Piece::BLACKKING);
//    board.set({'b', 7}, Piece::BLACKBISHOP);
}

void Game::update()
{
    
}

void Game::draw()
{
    board.draw();
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << std::boolalpha << board.isWinInTwoMoves() << std::endl;
    std::cout << (std::chrono::high_resolution_clock::now() - start);
}
