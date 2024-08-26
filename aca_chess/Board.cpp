//
//  Board.cpp
//  aca_chess
//
//  Created by Alex Aramyan on 03.07.24.
//

#include "Board.hpp"

#include <cmath>
#include <vector>
#include <future>

Board::Board()
{
    positionWhitePawn = 0xff << 8;
    positionWhiteRook = (1 << 7) | 0b1;
    positionWhiteBishop = (1 << 5) | 0b100;
    positionWhiteKnight = (1 << 6) | 0b10;
    positionWhiteQueen = 1 << 4;
    positionWhiteKing = 1 << 3;
    
    positionBlackPawn = positionWhitePawn << 40;
    positionBlackRook = positionWhiteRook << 56;
    positionBlackBishop = positionWhiteBishop << 56;
    positionBlackKnight = positionWhiteKnight << 56;
    positionBlackQueen = positionWhiteQueen << 56;
    positionBlackKing = positionWhiteKing << 56;
}

uint64_t& Board::getEncoding(Piece piece)
{
    switch (piece)
    {
        case Piece::WHITEPAWN:
            return positionWhitePawn;
        case Piece::WHITEROOK:
            return positionWhiteRook;
        case Piece::WHITEBISHOP:
            return positionWhiteBishop;
        case Piece::WHITEKNIGHT:
            return positionWhiteKnight;
        case Piece::WHITEQUEEN:
            return positionWhiteQueen;
        case Piece::WHITEKING:
            return positionWhiteKing;
        case Piece::BLACKPAWN:
            return positionBlackPawn;
        case Piece::BLACKROOK:
            return positionBlackRook;
        case Piece::BLACKBISHOP:
            return positionBlackBishop;
        case Piece::BLACKKNIGHT:
            return positionBlackKnight;
        case Piece::BLACKQUEEN:
            return positionBlackQueen;
        case Piece::BLACKKING:
            return positionBlackKing;
        default:
            break;
    }
    throw std::invalid_argument("The argument piece is Piece::NONE");
}

uint64_t Board::getValidMoves(int row, int col)
{
    Piece p = get(row, col);
    uint64_t vMoves = 0ULL;
    
    switch (p)
    {
        case Piece::WHITEKING:
        case Piece::BLACKKING:
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    if (i == 0 && j == 0) continue; // Skip the current position
                    
                    int vRow = row + i;
                    int vCol = col + j;
                    
                    // Check if the new position is within bounds
                    if (vRow >= 0 && vRow < 8 && vCol >= 0 && vCol < 8)
                    {
                        vMoves |= 1ULL << (vRow * 8 + (7 - vCol));
                    }
                }
            }
            return vMoves;

        case Piece::WHITEQUEEN:
        case Piece::BLACKQUEEN:
            // Rook-like movement (horizontal and vertical)
            for (int i = 1; i < 8; i++)
            {
                // Horizontal right
                if (col + i < 8) vMoves |= 1ULL << (row * 8 + (7 - (col + i)));
                // Horizontal left
                if (col - i >= 0) vMoves |= 1ULL << (row * 8 + (7 - (col - i)));
                // Vertical up
                if (row + i < 8) vMoves |= 1ULL << ((row + i) * 8 + (7 - col));
                // Vertical down
                if (row - i >= 0) vMoves |= 1ULL << ((row - i) * 8 + (7 - col));
            }

            // Bishop-like movement (diagonals)
            for (int i = 1; i < 8; i++)
            {
                // Top-right diagonal
                if (row + i < 8 && col + i < 8) vMoves |= 1ULL << ((row + i) * 8 + (7 - (col + i)));
                // Top-left diagonal
                if (row + i < 8 && col - i >= 0) vMoves |= 1ULL << ((row + i) * 8 + (7 - (col - i)));
                // Bottom-right diagonal
                if (row - i >= 0 && col + i < 8) vMoves |= 1ULL << ((row - i) * 8 + (7 - (col + i)));
                // Bottom-left diagonal
                if (row - i >= 0 && col - i >= 0) vMoves |= 1ULL << ((row - i) * 8 + (7 - (col - i)));
            }
            return vMoves;

        // Add cases for other pieces like rook, bishop, knight, and pawn
        case Piece::WHITEROOK:
        case Piece::BLACKROOK:
            for (int i = 1; i < 8; i++)
            {
                if (col + i < 8) vMoves |= 1ULL << (row * 8 + (7 - (col + i))); // Right
                if (col - i >= 0) vMoves |= 1ULL << (row * 8 + (7 - (col - i))); // Left
                if (row + i < 8) vMoves |= 1ULL << ((row + i) * 8 + (7 - col)); // Up
                if (row - i >= 0) vMoves |= 1ULL << ((row - i) * 8 + (7 - col)); // Down
            }
            return vMoves;

        case Piece::WHITEBISHOP:
        case Piece::BLACKBISHOP:
            for (int i = 1; i < 8; i++)
            {
                if (row + i < 8 && col + i < 8) vMoves |= 1ULL << ((row + i) * 8 + (7 - (col + i))); // Top-right
                if (row + i < 8 && col - i >= 0) vMoves |= 1ULL << ((row + i) * 8 + (7 - (col - i))); // Top-left
                if (row - i >= 0 && col + i < 8) vMoves |= 1ULL << ((row - i) * 8 + (7 - (col + i))); // Bottom-right
                if (row - i >= 0 && col - i >= 0) vMoves |= 1ULL << ((row - i) * 8 + (7 - (col - i))); // Bottom-left
            }
            return vMoves;

        case Piece::WHITEKNIGHT:
        case Piece::BLACKKNIGHT:
        {
            int knightMoves[8][2] = {
                {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
            };
            for (auto& move : knightMoves)
            {
                int vRow = row + move[0];
                int vCol = col + move[1];
                
                if (vRow >= 0 && vRow < 8 && vCol >= 0 && vCol < 8)
                {
                    vMoves |= 1ULL << (vRow * 8 + (7 - vCol));
                }
            }
            return vMoves;
        }

        case Piece::WHITEPAWN:
            if (row + 1 < 8) vMoves |= 1ULL << ((row + 1) * 8 + (7 - col)); // Single step
            if (row == 1 && row + 2 < 8) vMoves |= 1ULL << ((row + 2) * 8 + (7 - col)); // Double step
            return vMoves;

        case Piece::BLACKPAWN:
            if (row - 1 >= 0) vMoves |= 1ULL << ((row - 1) * 8 + (7 - col)); // Single step
            if (row == 6 && row - 2 >= 0) vMoves |= 1ULL << ((row - 2) * 8 + (7 - col)); // Double step
            return vMoves;

        default:
            break;
    }

    return 0;
}

void Board::__set(int row, int col, Piece piece)
{
    Piece p = get(row, col);
    if (p != Piece::NONE)
    {
        auto mask = ~(1ULL << (row * 8 + (7 - col)));
        getEncoding(p) &= mask;
    }
    
    if (piece != Piece::NONE)
        getEncoding(piece) |= (1ULL << (row * 8 + (7 - col)));
}

void Board::set(Coordinate coord, Piece piece)
{
    char col = coord.col;
    uint8_t row = coord.row;
    
    if (row < 1 || row > 8 || col < 'a' || col > 'h')
        throw std::invalid_argument{"Error: invalid values for row or column!"};
    
    __set(row - 1, col - 'a', piece);
    
    for (int i = 0; i < 10; i++)
        std::cout << "\n";
    draw();
    std::cout << "\n";
    std::cout << "\n";
}

int Board::minimax(int depth, bool isMaximizingPlayer, int alpha=INT_MIN, int beta=INT_MAX)
{
    if (depth == 0)
        return isMate();

    if (isMaximizingPlayer) {
        int maxEval = INT_MIN;

        uint64_t allWhitePieces = positionWhitePawn | positionWhiteRook | positionWhiteBishop |
                                  positionWhiteKnight | positionWhiteQueen | positionWhiteKing;

        while (allWhitePieces) {
            int pos = (int)log2(allWhitePieces & -allWhitePieces);
            int rowFrom = pos >> 3;
            int colFrom = 8 - (pos + 1) & 7;  // Simplified bitwise operation

            Piece piece = get(rowFrom, colFrom);

            uint64_t validMoves = getValidMoves(rowFrom, colFrom); // Generate all valid moves in one go

            while (validMoves) {
                int targetPos = (int)log2(validMoves & -validMoves);
                int rowTo = targetPos >> 3;
                int colTo = 8 - (targetPos + 1) & 7;

                // Make the move
                Piece originalDestination = get(rowTo, colTo);
                if (__move(rowFrom, colFrom, rowTo, colTo)) {
                    int eval = minimax(depth - 1, false, alpha, beta);
                    maxEval = std::max(maxEval, eval);
                    alpha = std::max(alpha, eval);

                    // Undo the move
                    __set(rowTo, colTo, originalDestination);
                    __set(rowFrom, colFrom, piece);

                    if (beta <= alpha)
                        break; // Beta cut-off
                }

                validMoves &= validMoves - 1;
            }

            allWhitePieces &= allWhitePieces - 1;
        }

        return maxEval;
    } else {
        int minEval = INT_MAX;

        uint64_t allBlackPieces = positionBlackPawn | positionBlackRook | positionBlackBishop |
                                  positionBlackKnight | positionBlackQueen | positionBlackKing;

        while (allBlackPieces) {
            int pos = (int)log2(allBlackPieces & -allBlackPieces);
            int rowFrom = pos >> 3;
            int colFrom = 8 - (pos + 1) & 7;

            Piece piece = get(rowFrom, colFrom);

            uint64_t validMoves = getValidMoves(rowFrom, colFrom);

            while (validMoves) {
                int targetPos = (int)log2(validMoves & -validMoves);
                int rowTo = targetPos >> 3;
                int colTo = 8 - (targetPos + 1) & 7;

                // Make the move
                Piece originalDestination = get(rowTo, colTo);
                if (__move(rowFrom, colFrom, rowTo, colTo)) {
                    int eval = minimax(depth - 1, true, alpha, beta);
                    minEval = std::min(minEval, eval);
                    beta = std::min(beta, eval);

                    // Undo the move
                    __set(rowTo, colTo, originalDestination);
                    __set(rowFrom, colFrom, piece);

                    if (beta <= alpha)
                        break; // Alpha cut-off
                }

                validMoves &= validMoves - 1;
            }

            allBlackPieces &= allBlackPieces - 1;
        }

        return minEval;
    }
}

//int Board::minimax(int depth, bool isMaximizingPlayer) {
//    if (depth == 0)
//        return isMate();
//
//    if (isMaximizingPlayer) // White's move
//    {
//        int maxEval = INT_MIN;
//        
//        uint64_t allWhitePieces = positionWhitePawn | positionWhiteRook | positionWhiteBishop |
//                                  positionWhiteKnight | positionWhiteQueen | positionWhiteKing;
//        
//        while (allWhitePieces)
//        {
//            int pos = (int)log2(allWhitePieces & -allWhitePieces);
//            int rowFrom = pos >> 3;
//            int colFrom = 8 - (pos + 1) & 7;
//            
//            Piece piece = get(rowFrom, colFrom);
//            
//            for (int rowTo = 0; rowTo < 8; rowTo++)
//            {
//                for (int colTo = 0; colTo < 8; colTo++)
//                {
//                    if (rowFrom == rowTo && colFrom == colTo)
//                        continue;
//                    if (!isMoveValid(rowFrom, colFrom, rowTo, colTo))
//                        continue;
//
//                    // Make the move
//                    Piece originalDestination = get(rowTo, colTo);
//                    if (__move(rowFrom, colFrom, rowTo, colTo))
//                    {
//                        int eval = minimax(depth - 1, false);
//                        maxEval = std::max(maxEval, eval);
//
//                        // Undo the move
//                        __set(rowTo, colTo, originalDestination);
//                        __set(rowFrom, colFrom, piece);
//                    }
//                }
//            }
//            
//            allWhitePieces &= allWhitePieces - 1;
//        }
//        
//        return maxEval;
//    }
//    else // Black's move
//    {
//        int minEval = INT_MAX;
//        
//        uint64_t allBlackPieces = positionBlackPawn | positionBlackRook | positionBlackBishop |
//                                  positionBlackKnight | positionBlackQueen | positionBlackKing;
//        
//        while (allBlackPieces)
//        {
//            int pos = (int)log2(allBlackPieces & -allBlackPieces);
//            int rowFrom = pos >> 3;
//            int colFrom = 8 - (pos + 1) & 7;
//            
//            Piece piece = get(rowFrom, colFrom);
//            
//            for (int rowTo = 0; rowTo < 8; rowTo++)
//            {
//                for (int colTo = 0; colTo < 8; colTo++)
//                {
//                    if (rowFrom == rowTo && colFrom == colTo)
//                        continue;
//                    if (!isMoveValid(rowFrom, colFrom, rowTo, colTo))
//                        continue;
//
//                    // Make the move
//                    Piece originalDestination = get(rowTo, colTo);
//                    if (__move(rowFrom, colFrom, rowTo, colTo))
//                    {
//                        int eval = minimax(depth - 1, true);
//                        minEval = std::min(minEval, eval);
//
//                        // Undo the move
//                        __set(rowTo, colTo, originalDestination);
//                        __set(rowFrom, colFrom, piece);
//                    }
//                }
//            }
//            
//            allBlackPieces &= allBlackPieces - 1;
//        }
//        
//        return minEval;
//    }
//}

Piece Board::get(int row, int col) const
{
    unsigned long long mask = 1ULL << (row * 8 + (7 - col));
    
    if (positionWhitePawn & mask)
        return Piece::WHITEPAWN;
    else if (positionWhiteRook & mask)
        return Piece::WHITEROOK;
    else if (positionWhiteBishop & mask)
        return Piece::WHITEBISHOP;
    else if (positionWhiteKnight & mask)
        return Piece::WHITEKNIGHT;
    else if (positionWhiteQueen & mask)
        return Piece::WHITEQUEEN;
    else if (positionWhiteKing & mask)
        return Piece::WHITEKING;
    else if (positionBlackPawn & mask)
        return Piece::BLACKPAWN;
    else if (positionBlackRook & mask)
        return Piece::BLACKROOK;
    else if (positionBlackBishop & mask)
        return Piece::BLACKBISHOP;
    else if (positionBlackKnight & mask)
        return Piece::BLACKKNIGHT;
    else if (positionBlackQueen & mask)
        return Piece::BLACKQUEEN;
    else if (positionBlackKing & mask)
        return Piece::BLACKKING;
    else
        return Piece::NONE;
}

bool Board::isMoveValid(int rowFrom, int colFrom, int rowTo, int colTo) {
    Piece p = get(rowFrom, colFrom);
    Piece target = get(rowTo, colTo);

    if (rowFrom < 0 || rowFrom >= 8 || colFrom < 0 || colFrom >= 8 ||
        rowTo < 0 || rowTo >= 8 || colTo < 0 || colTo >= 8)
        return false;

    int rowDiff = rowTo - rowFrom;
    int colDiff = colTo - colFrom;

    // Determine piece color and movement direction
    bool isWhite = (int)p <= (int)Piece::WHITEKING;
    int forward = isWhite ? 1 : -1;

    // Validate the move based on the piece's movement rules
    bool validMove = false;
    switch (p) {
        case Piece::WHITEPAWN:
        case Piece::BLACKPAWN:
            if (colFrom == colTo && target == Piece::NONE) {
                if (rowDiff == forward)
                    validMove = true;
                if ((rowFrom == 1 && isWhite) || (rowFrom == 6 && !isWhite))
                    if (rowDiff == 2 * forward && get(rowFrom + forward, colFrom) == Piece::NONE)
                        validMove = true;
            } else if (std::abs(colDiff) == 1 && rowDiff == forward && target != Piece::NONE) {
                if ((isWhite && (int)target > (int)Piece::WHITEKING) ||
                    (!isWhite && (int)target <= (int)Piece::WHITEKING))
                    validMove = true;
            }
            break;

        case Piece::WHITEKNIGHT:
        case Piece::BLACKKNIGHT:
            if ((std::abs(rowDiff) == 2 && std::abs(colDiff) == 1) ||
                (std::abs(rowDiff) == 1 && std::abs(colDiff) == 2))
                if (target == Piece::NONE || (isWhite && (int)target > (int)Piece::WHITEKING) ||
                    (!isWhite && (int)target <= (int)Piece::WHITEKING))
                    validMove = true;
            break;

        case Piece::WHITEBISHOP:
        case Piece::BLACKBISHOP:
            if (std::abs(rowDiff) == std::abs(colDiff)) {
                int rowStep = rowDiff / std::abs(rowDiff);
                int colStep = colDiff / std::abs(colDiff);
                for (int i = 1; i < std::abs(rowDiff); ++i)
                    if (get(rowFrom + i * rowStep, colFrom + i * colStep) != Piece::NONE)
                        return false;

                if (target == Piece::NONE || (isWhite && (int)target > (int)Piece::WHITEKING) ||
                    (!isWhite && (int)target <= (int)Piece::WHITEKING))
                    validMove = true;
            }
            break;

        case Piece::WHITEROOK:
        case Piece::BLACKROOK:
            if (rowFrom == rowTo || colFrom == colTo) {
                int rowStep = rowDiff ? rowDiff / std::abs(rowDiff) : 0;
                int colStep = colDiff ? colDiff / std::abs(colDiff) : 0;
                int distance = std::max(std::abs(rowDiff), std::abs(colDiff));

                for (int i = 1; i < distance; ++i)
                    if (get(rowFrom + i * rowStep, colFrom + i * colStep) != Piece::NONE)
                        return false;

                if (target == Piece::NONE || (isWhite && (int)target > (int)Piece::WHITEKING) ||
                    (!isWhite && (int)target <= (int)Piece::WHITEKING))
                    validMove = true;
            }
            break;

        case Piece::WHITEQUEEN:
        case Piece::BLACKQUEEN:
            if (rowFrom == rowTo || colFrom == colTo || std::abs(rowDiff) == std::abs(colDiff)) {
                int rowStep = rowDiff ? rowDiff / std::abs(rowDiff) : 0;
                int colStep = colDiff ? colDiff / std::abs(colDiff) : 0;
                int distance = std::max(std::abs(rowDiff), std::abs(colDiff));

                for (int i = 1; i < distance; ++i)
                    if (get(rowFrom + i * rowStep, colFrom + i * colStep) != Piece::NONE)
                        return false;

                if (target == Piece::NONE || (isWhite && (int)target > (int)Piece::WHITEKING) ||
                    (!isWhite && (int)target <= (int)Piece::WHITEKING))
                    validMove = true;
            }
            break;

        case Piece::WHITEKING:
        case Piece::BLACKKING:
            if (std::abs(rowDiff) <= 1 && std::abs(colDiff) <= 1) {
                if (target == Piece::NONE || (isWhite && (int)target > (int)Piece::WHITEKING) ||
                    (!isWhite && (int)target <= (int)Piece::WHITEKING)) {

                    // Check if the move puts the king adjacent to the opponent's king
                    Piece opponentKing = isWhite ? Piece::BLACKKING : Piece::WHITEKING;
                    for (int r = -1; r <= 1; ++r) {
                        for (int c = -1; c <= 1; ++c) {
                            int checkRow = rowTo + r;
                            int checkCol = colTo + c;
                            if (checkRow >= 0 && checkRow < 8 && checkCol >= 0 && checkCol < 8) {
                                if (get(checkRow, checkCol) == opponentKing)
                                    return false; // Invalid if the king moves next to the opponent's king
                            }
                        }
                    }

                    validMove = true;
                }
            }
            break;

        default:
            return false;
    }

    // If the move is valid based on the piece's rules, check if it leaves the king in check
    if (validMove) {
        // Temporarily make the move
        __set(rowTo, colTo, p);
        __set(rowFrom, colFrom, Piece::NONE);

        bool kingInCheck = isWhite ? isAttackWhite() : isAttackBlack();

        // Revert the move
        __set(rowFrom, colFrom, p);
        __set(rowTo, colTo, target);

        // If the king is in check after the move, the move is invalid
        if (kingInCheck)
            return false;

        return true;
    }

    return false;
}




bool Board::__move(int rowFrom, int colFrom, int rowTo, int colTo)
{
    std::cout << std::endl;
    
    if (isMoveValid(rowFrom, colFrom, rowTo, colTo))
    {
        Piece p = get(rowFrom, colFrom);
        
        __set(rowTo, colTo, p);
        __set(rowFrom, colFrom, Piece::NONE);
        
        draw();
        
        return true;
    }
    
    return false;
}

bool Board::move(Coordinate fromCoord, Coordinate toCoord)
{
    int rowFrom = fromCoord.row - 1;
    int colFrom = fromCoord.col - 'a';
    int rowTo = toCoord.row - 1;
    int colTo = toCoord.col - 'a';

    return __move(rowFrom, colFrom, rowTo, colTo);
}

//void Board::draw()
//{
//    const char* whiteKing = "\u2654";
//    const char* whiteQueen = "\u2655";
//    const char* whiteRook = "\u2656";
//    const char* whiteBishop = "\u2657";
//    const char* whiteKnight = "\u2658";
//    const char* whitePawn = "\u2659";
//    
//    const char* blackKing = "\u265A";
//    const char* blackQueen = "\u265B";
//    const char* blackRook = "\u265C";
//    const char* blackBishop = "\u265D";
//    const char* blackKnight = "\u265E";
//    const char* blackPawn = "\u265F";
//    
//    for (int row = 7; row >= 0; row--)
//    {
//        for (int col = 0; col < 8; col++)
//        {
//            unsigned long long mask = 1ULL << (row * 8 + (7 - col));
//            
//            if (positionWhitePawn & mask)
//                std::cout << whitePawn;
//            else if (positionWhiteRook & mask)
//                std::cout << whiteRook;
//            else if (positionWhiteBishop & mask)
//                std::cout << whiteBishop;
//            else if (positionWhiteKnight & mask)
//                std::cout << whiteKnight;
//            else if (positionWhiteQueen & mask)
//                std::cout << whiteQueen;
//            else if (positionWhiteKing & mask)
//                std::cout << whiteKing;
//            else if (positionBlackPawn & mask)
//                std::cout << blackPawn;
//            else if (positionBlackRook & mask)
//                std::cout << blackRook;
//            else if (positionBlackBishop & mask)
//                std::cout << blackBishop;
//            else if (positionBlackKnight & mask)
//                std::cout << blackKnight;
//            else if (positionBlackQueen & mask)
//                std::cout << blackQueen;
//            else if (positionBlackKing & mask)
//                std::cout << blackKing;
//            else
//                std::cout << ".";
//            std::cout << " ";
//        }
//        std::cout << std::endl;
//    }
//}

void Board::draw()
{
    const char* whiteKing = "\u2654";
    const char* whiteQueen = "\u2655";
    const char* whiteRook = "\u2656";
    const char* whiteBishop = "\u2657";
    const char* whiteKnight = "\u2658";
    const char* whitePawn = "\u2659";
    
    const char* blackKing = "\u265A";
    const char* blackQueen = "\u265B";
    const char* blackRook = "\u265C";
    const char* blackBishop = "\u265D";
    const char* blackKnight = "\u265E";
    const char* blackPawn = "\u265F";

    // Print column labels (A-H)
    std::cout << "  A B C D E F G H" << std::endl;

    for (int row = 7; row >= 0; row--)
    {
        // Print row label (1-8)
        std::cout << row + 1 << " ";

        for (int col = 0; col < 8; col++)
        {
            // Calculate the bitboard mask for the current square
            unsigned long long mask = 1ULL << (row * 8 + (7 - col));
            
            // Check which piece is on the current square
            if (positionWhitePawn & mask)
                std::cout << whitePawn;
            else if (positionWhiteRook & mask)
                std::cout << whiteRook;
            else if (positionWhiteBishop & mask)
                std::cout << whiteBishop;
            else if (positionWhiteKnight & mask)
                std::cout << whiteKnight;
            else if (positionWhiteQueen & mask)
                std::cout << whiteQueen;
            else if (positionWhiteKing & mask)
                std::cout << whiteKing;
            else if (positionBlackPawn & mask)
                std::cout << blackPawn;
            else if (positionBlackRook & mask)
                std::cout << blackRook;
            else if (positionBlackBishop & mask)
                std::cout << blackBishop;
            else if (positionBlackKnight & mask)
                std::cout << blackKnight;
            else if (positionBlackQueen & mask)
                std::cout << blackQueen;
            else if (positionBlackKing & mask)
                std::cout << blackKing;
            else
                std::cout << ".";  // Empty square
            
            std::cout << " ";
        }

        // End of row, move to the next line
        std::cout << std::endl;
    }
}


bool Board::isAttackBlack()
{
    int bKingPos = (int)log2(positionBlackKing);
    int bKingRow = bKingPos >> 3;
    int bKingCol = 8 - (bKingPos + 1) & 7;
//    int bKingCol = 8 - (bKingPos + 1) % 8;

    uint64_t allWhitePieces = positionWhitePawn | positionWhiteRook | positionWhiteBishop |
                              positionWhiteKnight | positionWhiteQueen | positionWhiteKing;

//    std::cout << std::bitset<64>(allWhitePieces) << std::endl;
    
    while (allWhitePieces)
    {
        int pos = (int)log2(allWhitePieces & -allWhitePieces);
        int row = pos >> 3;
        int col = 8 - (pos + 1) & 7;

        if (isMoveValid(row, col, bKingRow, bKingCol))
            return true;

        allWhitePieces &= allWhitePieces - 1;
    }

    return false;
}

bool Board::isAttackWhite()
{
    int bKingPos = (int)log2(positionWhiteKing);
    int bKingRow = bKingPos >> 3;
    int bKingCol = 8 - (bKingPos + 1) & 7;
//    int bKingCol = 8 - (bKingPos + 1) % 8;

    uint64_t allBlackPieces = positionBlackPawn | positionBlackRook | positionBlackBishop |
                              positionBlackKnight | positionBlackQueen | positionBlackKing;

//    std::cout << std::bitset<64>(allBlackPieces) << std::endl;
    
    while (allBlackPieces)
    {
        int pos = (int)log2(allBlackPieces & -allBlackPieces);
        int row = pos >> 3;
        int col = 8 - (pos + 1) & 7;

        if (isMoveValid(row, col, bKingRow, bKingCol))
            return true;

        allBlackPieces &= allBlackPieces - 1;
    }

    return false;
}

int Board::isAttack()
{
    if (isAttackWhite())
        return -1;
    else if (isAttackBlack())
        return 1;
    else
        return 0;
}
//
//bool Board::isMate()
//{
//    if (!isAttack())
//        return false;
//
//    int bKingPos = (int)log2(positionBlackKing & -positionBlackKing);
//    int bKingRow = bKingPos >> 3;
//    int bKingCol = 8 - (bKingPos + 1) & 7;
//
//    int moves[8][2] = {
//        {-1,  1}, { 0, 1 }, {1,  1},
//        {-1,  0},           {1,  0},
//        {-1, -1}, { 0, -1}, {1,  -1}
//    };
//
//    for (auto&& move : moves)
//    {
//        int newCol = bKingCol + move[0];
//        int newRow = bKingRow + move[1];
//
//        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
//        {
//            Piece target = get(newRow, newCol);
//            if (target == Piece::NONE || (int)target <= (int)Piece::WHITEKING)
//            {
//                __set(newRow, newCol, Piece::BLACKKING);
//                __set(bKingRow, bKingCol, Piece::NONE);
//
//                bool stillInCheck = isAttack();
//
//                __set(bKingRow, bKingCol, Piece::BLACKKING);
//                __set(newRow, newCol, target);
//
//                if (!stillInCheck)
//                    return false;
//            }
//        }
//    }
//
//    return true;
//}

int Board::isMate() {
    int attackStatus = isAttack();

    if (attackStatus == 0)
        return 0;  // No check, hence no mate.

    int kingPos, kingRow, kingCol;
    Piece kingPiece;

    if (attackStatus == 1) {  // White is attacking Black's King
        kingPiece = Piece::BLACKKING;
        kingPos = (int)log2(positionBlackKing & -positionBlackKing);
    } else {  // Black is attacking White's King
        kingPiece = Piece::WHITEKING;
        kingPos = (int)log2(positionWhiteKing & -positionWhiteKing);
    }

    kingRow = kingPos >> 3;
    kingCol = 8 - (kingPos + 1) & 7;

    int moves[8][2] = {
        {-1,  1}, { 0, 1 }, {1,  1},
        {-1,  0},           {1,  0},
        {-1, -1}, { 0, -1}, {1,  -1}
    };

    for (auto&& move : moves) {
        int newCol = kingCol + move[0];
        int newRow = kingRow + move[1];

        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            Piece target = get(newRow, newCol);
            if (target == Piece::NONE ||
                (attackStatus == 1 && (int)target <= (int)Piece::WHITEKING) ||
                (attackStatus == -1 && (int)target > (int)Piece::WHITEKING)) {
                
//                __set(newRow, newCol, kingPiece);
//                __set(kingRow, kingCol, Piece::NONE);

                if (__move(kingRow, kingCol, newRow, newCol))
                {
                    bool stillInCheck = isAttack();
                    
                    __set(kingRow, kingCol, kingPiece);
                    __set(newRow, newCol, target);
                    
                    if (!stillInCheck)
                        return 0;  // The king can move out of check, so no mate.
                }
                
            }
        }
    }

    return attackStatus;  // Return 1 if Black is checkmated, -1 if White is checkmated.
}

bool Board::isWinInOneMove()
{
    // minimax(depth, whitestarts) -> whitewins
    
    return minimax(1, true) == 1;
}

bool Board::isWinInTwoMoves()
{
    // minimax(depth, whitestarts) -> whitewins
    
    return minimax(3, true) == 1;
}
