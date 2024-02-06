#ifndef TESTS_H
#define TESTS_H
#include "bitposition.h"
#include <vector>
#include <iostream> // For printing

// Additional helper function for printing moves
void printMove(const Move &move)
{
    // Assuming you have a way to convert 'Move' to a string or standard notation
    std::cout << move.toString() << ": ";
}

unsigned long long runPerftTest(BitPosition& position, int depth, int currentDepth = 0)
{

    if (depth == 0)
        return 1;

    std::vector<Move> captures;
    std::vector<Move> non_captures;
    if (position.isCheck())
    {
        position.setChecksAndPinsBits();
        captures = position.inCheckCaptures();
        non_captures = position.inCheckMoves();
    }
    else
    {
        position.setPinsBits();
        captures = position.captureMoves();
        non_captures = position.nonCaptureMoves();
    }

    unsigned long long moveCount = 0;
    for (const Move &move : captures)
    {
        if (currentDepth == 0)
        {
            printMove(move);
        }
        position.makeMove(move);

        unsigned long long subCount = runPerftTest(position, depth - 1, currentDepth + 1);
        
        position.unmakeMove();

        if (currentDepth == 0)
        {
            std::cout << subCount << std::endl; // Print the number of moves leading from this move
        }

        moveCount += subCount;
    }
    for (const Move &move : non_captures)
    {
        if (currentDepth == 0)
        {
            printMove(move);
        }
        position.makeMove(move);

        unsigned long long subCount = runPerftTest(position, depth - 1, currentDepth + 1);

        position.unmakeMove();

        if (currentDepth == 0)
        {
            std::cout << subCount << std::endl; // Print the number of moves leading from this move
        }

        moveCount += subCount;
    }
    return moveCount;
}

#endif
