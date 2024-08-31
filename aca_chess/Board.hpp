//
//  Board.hpp
//  aca_chess
//
//  Created by Alex Aramyan on 03.07.24.
//

#ifndef Board_hpp
#define Board_hpp

#include <iostream>
#include <cstdint>

enum class Piece
{
    NONE,
    
    WHITEPAWN,
    WHITEROOK,
    WHITEBISHOP,
    WHITEKNIGHT,
    WHITEQUEEN,
    WHITEKING,
    
    BLACKPAWN,
    BLACKROOK,
    BLACKBISHOP,
    BLACKKNIGHT,
    BLACKQUEEN,
    BLACKKING
};

struct Coordinate
{
    char col;
    uint8_t row;
};

class Board
{
    uint64_t positionWhitePawn;
    uint64_t positionWhiteRook;
    uint64_t positionWhiteBishop;
    uint64_t positionWhiteKnight;
    uint64_t positionWhiteQueen;
    uint64_t positionWhiteKing;
    
    uint64_t positionBlackPawn;
    uint64_t positionBlackRook;
    uint64_t positionBlackBishop;
    uint64_t positionBlackKnight;
    uint64_t positionBlackQueen;
    uint64_t positionBlackKing;
private:
    uint64_t& getEncoding(Piece piece);
    uint64_t getValidMoves(int row, int col);
    bool isMoveValid(int rowFrom, int colFrom, int rowTo, int colTo);
    
    int evaluateBoard();
    bool isAttackWhite();
    bool isAttackBlack();
//    int minimax(int depth, bool isMaximizingPlayer);
    int minimax(int depth, bool isMaximizingPlayer, bool& foundSolution, int alpha, int beta);
    
    void __set(int row, int col, Piece piece);
    bool __move(int rowFrom, int colFrom, int rowTo, int colTo);
public:
    Board();
    
    Piece get(int row, int col) const;
    void set(Coordinate coord, Piece piece);
    bool move(Coordinate fromCoord, Coordinate toCoord);
    
    int isMate();
    int isAttack();
    bool isWinInOneMove();
    bool isWinInTwoMoves();
    
    void draw();
};

#endif /* Board_hpp */
