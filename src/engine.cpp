#include <array>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include "bitposition.h"

// Declare Zobrist Numbers as global variables
std::array<uint64_t, 64> whitePawnZobristNumbers;
std::array<uint64_t, 64> whiteKnightZobristNumbers;
std::array<uint64_t, 64> whiteBishopZobristNumbers;
std::array<uint64_t, 64> whiteRookZobristNumbers;
std::array<uint64_t, 64> whiteQueenZobristNumbers;
std::array<uint64_t, 64> whiteKingZobristNumbers;
std::array<uint64_t, 64> blackPawnZobristNumbers;
std::array<uint64_t, 64> blackKnightZobristNumbers;
std::array<uint64_t, 64> blackBishopZobristNumbers;
std::array<uint64_t, 64> blackRookZobristNumbers;
std::array<uint64_t, 64> blackQueenZobristNumbers;
std::array<uint64_t, 64> blackKingZobristNumbers;

std::array<uint64_t, 16> castlingRightsZobristNumbers;
std::unordered_map<int, uint64_t> passantSquaresZobristNumbers;

// Function to generate random numbers
std::vector<uint64_t> generateRandomNumbers(size_t count, uint64_t seed)
{
    std::unordered_set<uint64_t> randomNumbers;
    std::mt19937_64 eng(seed);
    std::uniform_int_distribution<uint64_t> distr(1, UINT64_MAX - 1);

    while (randomNumbers.size() < count)
    {
        randomNumbers.insert(distr(eng));
    }

    return std::vector<uint64_t>(randomNumbers.begin(), randomNumbers.end());
}

int Search()
{
    // Generate random numbers
    const size_t totalNumbers = 801;
    auto randomNumbers = generateRandomNumbers(totalNumbers, 96620);

    // Fill arrays with random numbers
    for (size_t i = 0; i < 64; ++i)
    {
        whitePawnZobristNumbers[i] = randomNumbers[i];
        whiteKnightZobristNumbers[i] = randomNumbers[64 + i];
        whiteBishopZobristNumbers[i] = randomNumbers[64 * 2 + i];
        whiteRookZobristNumbers[i] = randomNumbers[64 * 3 + i];
        whiteQueenZobristNumbers[i] = randomNumbers[64 * 4 + i];
        whiteKingZobristNumbers[i] = randomNumbers[64 * 5 + i];
        blackPawnZobristNumbers[i] = randomNumbers[64 * 6 + i];
        blackKnightZobristNumbers[i] = randomNumbers[64 * 7 + i];
        blackBishopZobristNumbers[i] = randomNumbers[64 * 8 + i];
        blackRookZobristNumbers[i] = randomNumbers[64 * 9 + i];
        blackQueenZobristNumbers[i] = randomNumbers[64 * 10 + i];
        blackKingZobristNumbers[i] = randomNumbers[62 * 11 + i];
    }

    // Initialize castlingRightsZobristNumbers and passantSquaresZobristNumbers
    uint64_t blackToMoveZobristNumber = randomNumbers[12 * 64];

    std::array<uint64_t, 16> castlingRightsZobristNumbers;
    for (size_t i = 0; i < 16; ++i)
    {
        castlingRightsZobristNumbers[i] = randomNumbers[(12 * 64) + 1 + i];
    }

    std::unordered_map<int, uint64_t> passantSquaresZobristNumbers;
    // Initialize passant squares zobrist numbers
    passantSquaresZobristNumbers[-1] = 0;
    for (size_t i = 0; i < 8; ++i)
    {
        passantSquaresZobristNumbers[16 + i] = randomNumbers[(12 * 64) + 17 + i];
        passantSquaresZobristNumbers[40 + i] = randomNumbers[(12 * 64) + 25 + i];
    }
    return 0;
}