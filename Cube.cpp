#include "Cube.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>

Cube::Cube()
{
    resetCubeToSolved();

    for (size_t n = 0; n < EDGE_COUNT; ++n)
    {
        _CValues[n][0] = 1;
    }
    for (size_t k = 1; k <= SLICE_EDGE_COUNT; ++k)
    {
        _CValues[0][k] = 0;
    }

    for (size_t n = 1; n < EDGE_COUNT; ++n)
    {
        for (size_t k = 1; k <= SLICE_EDGE_COUNT; ++k)
        {
            _CValues[n][k] = _CValues[n - 1][k - 1] + _CValues[n - 1][k];
        }
    }

    // clang-format off
    _moveFunctions =
    {
        {Move::U, [this]() { moveU(); }},
        {Move::UPrime, [this]() { moveUPrime(); }},
        {Move::DoubleU, [this]() { move2U(); }},
        {Move::D, [this]() { moveD(); }},
        {Move::DPrime, [this]() { moveDPrime(); }},
        {Move::DoubleD, [this]() { move2D(); }},
        {Move::F, [this]() { moveF(); }},
        {Move::FPrime, [this]() { moveFPrime(); }},
        {Move::DoubleF, [this]() { move2F(); }},
        {Move::B, [this]() { moveB(); }},
        {Move::BPrime, [this]() { moveBPrime(); }},
        {Move::DoubleB, [this]() { move2B(); }},
        {Move::R, [this]() { moveR(); }},
        {Move::RPrime, [this]() { moveRPrime(); }},
        {Move::DoubleR, [this]() { move2R(); }},
        {Move::L, [this]() { moveL(); }},
        {Move::LPrime, [this]() { moveLPrime(); }},
        {Move::DoubleL, [this]() { move2L(); }}
    };
    // clang-format on

    buildFlipMovesTable();
    buildTwistMovesTable();
    buildUDSliceMovesTable();
    buildTwistSlicePtb();
    buildFlipSlicePtb();
}

void Cube::resetCubeToSolved()
{
    _cornerOrient.fill(0);
    _edgeOrient.fill(0);

    std::iota(_cornerPerm.begin(), _cornerPerm.end(), 0);
    std::iota(_edgePerm.begin(), _edgePerm.end(), 0);
}

void Cube::moveU()
{
    const uint8_t tempCornPerm0 = _cornerPerm[0];
    _cornerPerm[0] = _cornerPerm[1];
    _cornerPerm[1] = _cornerPerm[2];
    _cornerPerm[2] = _cornerPerm[3];
    _cornerPerm[3] = tempCornPerm0;

    const uint8_t tempCornOri0 = _cornerOrient[0];
    _cornerOrient[0] = _cornerOrient[1];
    _cornerOrient[1] = _cornerOrient[2];
    _cornerOrient[2] = _cornerOrient[3];
    _cornerOrient[3] = tempCornOri0;

    const uint8_t tempEdgePerm0 = _edgePerm[0];
    _edgePerm[0] = _edgePerm[1];
    _edgePerm[1] = _edgePerm[2];
    _edgePerm[2] = _edgePerm[3];
    _edgePerm[3] = tempEdgePerm0;

    const uint8_t tempEdgeOri0 = _edgeOrient[0];
    _edgeOrient[0] = _edgeOrient[1];
    _edgeOrient[1] = _edgeOrient[2];
    _edgeOrient[2] = _edgeOrient[3];
    _edgeOrient[3] = tempEdgeOri0;
}

void Cube::moveUPrime()
{
    const uint8_t tempCornPerm3 = _cornerPerm[3];
    _cornerPerm[3] = _cornerPerm[2];
    _cornerPerm[2] = _cornerPerm[1];
    _cornerPerm[1] = _cornerPerm[0];
    _cornerPerm[0] = tempCornPerm3;

    const uint8_t tempCornOri3 = _cornerOrient[3];
    _cornerOrient[3] = _cornerOrient[2];
    _cornerOrient[2] = _cornerOrient[1];
    _cornerOrient[1] = _cornerOrient[0];
    _cornerOrient[0] = tempCornOri3;

    const uint8_t tempEdgePerm3 = _edgePerm[3];
    _edgePerm[3] = _edgePerm[2];
    _edgePerm[2] = _edgePerm[1];
    _edgePerm[1] = _edgePerm[0];
    _edgePerm[0] = tempEdgePerm3;

    const uint8_t tempEdgeOri3 = _edgeOrient[3];
    _edgeOrient[3] = _edgeOrient[2];
    _edgeOrient[2] = _edgeOrient[1];
    _edgeOrient[1] = _edgeOrient[0];
    _edgeOrient[0] = tempEdgeOri3;
}

void Cube::move2U()
{
    moveU();
    moveU();
}

void Cube::moveD()
{
    const uint8_t tempCornPerm6 = _cornerPerm[6];
    _cornerPerm[6] = _cornerPerm[5];
    _cornerPerm[5] = _cornerPerm[4];
    _cornerPerm[4] = _cornerPerm[7];
    _cornerPerm[7] = tempCornPerm6;

    const uint8_t tempCornOri6 = _cornerOrient[6];
    _cornerOrient[6] = _cornerOrient[5];
    _cornerOrient[5] = _cornerOrient[4];
    _cornerOrient[4] = _cornerOrient[7];
    _cornerOrient[7] = tempCornOri6;

    const uint8_t tempEdgePerm10 = _edgePerm[10];
    _edgePerm[10] = _edgePerm[9];
    _edgePerm[9] = _edgePerm[8];
    _edgePerm[8] = _edgePerm[11];
    _edgePerm[11] = tempEdgePerm10;

    const uint8_t tempEdgeOri10 = _edgeOrient[10];
    _edgeOrient[10] = _edgeOrient[9];
    _edgeOrient[9] = _edgeOrient[8];
    _edgeOrient[8] = _edgeOrient[11];
    _edgeOrient[11] = tempEdgeOri10;
}

void Cube::moveDPrime()
{
    const uint8_t tempCornPerm7 = _cornerPerm[7];
    _cornerPerm[7] = _cornerPerm[4];
    _cornerPerm[4] = _cornerPerm[5];
    _cornerPerm[5] = _cornerPerm[6];
    _cornerPerm[6] = tempCornPerm7;

    const uint8_t tempCornOri7 = _cornerOrient[7];
    _cornerOrient[7] = _cornerOrient[4];
    _cornerOrient[4] = _cornerOrient[5];
    _cornerOrient[5] = _cornerOrient[6];
    _cornerOrient[6] = tempCornOri7;

    const uint8_t tempEdgePerm11 = _edgePerm[11];
    _edgePerm[11] = _edgePerm[8];
    _edgePerm[8] = _edgePerm[9];
    _edgePerm[9] = _edgePerm[10];
    _edgePerm[10] = tempEdgePerm11;

    const uint8_t tempEdgeOri11 = _edgeOrient[11];
    _edgeOrient[11] = _edgeOrient[8];
    _edgeOrient[8] = _edgeOrient[9];
    _edgeOrient[9] = _edgeOrient[10];
    _edgeOrient[10] = tempEdgeOri11;
}

void Cube::move2D()
{
    moveD();
    moveD();
}

void Cube::moveF()
{
    const uint8_t tempCornPerm4 = _cornerPerm[4];
    _cornerPerm[4] = _cornerPerm[5];
    _cornerPerm[5] = _cornerPerm[1];
    _cornerPerm[1] = _cornerPerm[0];
    _cornerPerm[0] = tempCornPerm4;

    const uint8_t tempCornOri5 = _cornerOrient[5];
    _cornerOrient[5] = static_cast<uint8_t>((_cornerOrient[1] + 2) % 3);
    _cornerOrient[1] = static_cast<uint8_t>((_cornerOrient[0] + 1) % 3);
    _cornerOrient[0] = static_cast<uint8_t>((_cornerOrient[4] + 2) % 3);
    _cornerOrient[4] = static_cast<uint8_t>((tempCornOri5 + 1) % 3);

    const uint8_t tempEdgePerm4 = _edgePerm[4];
    _edgePerm[4] = _edgePerm[8];
    _edgePerm[8] = _edgePerm[5];
    _edgePerm[5] = _edgePerm[0];
    _edgePerm[0] = tempEdgePerm4;

    const uint8_t tempEdgeOri4 = _edgeOrient[4];
    _edgeOrient[4] = _edgeOrient[8] ^ 1;
    _edgeOrient[8] = _edgeOrient[5] ^ 1;
    _edgeOrient[5] = _edgeOrient[0] ^ 1;
    _edgeOrient[0] = tempEdgeOri4 ^ 1;
}

void Cube::moveFPrime()
{
    const uint8_t tempCornPerm0 = _cornerPerm[0];
    _cornerPerm[0] = _cornerPerm[1];
    _cornerPerm[1] = _cornerPerm[5];
    _cornerPerm[5] = _cornerPerm[4];
    _cornerPerm[4] = tempCornPerm0;

    const uint8_t tempCornOri0 = _cornerOrient[0];
    _cornerOrient[0] = static_cast<uint8_t>((_cornerOrient[1] + 2) % 3);
    _cornerOrient[1] = static_cast<uint8_t>((_cornerOrient[5] + 1) % 3);
    _cornerOrient[5] = static_cast<uint8_t>((_cornerOrient[4] + 2) % 3);
    _cornerOrient[4] = static_cast<uint8_t>((tempCornOri0 + 1) % 3);

    const uint8_t tempEdgePerm0 = _edgePerm[0];
    _edgePerm[0] = _edgePerm[5];
    _edgePerm[5] = _edgePerm[8];
    _edgePerm[8] = _edgePerm[4];
    _edgePerm[4] = tempEdgePerm0;

    const uint8_t tempEdgeOri0 = _edgeOrient[0];
    _edgeOrient[0] = _edgeOrient[5] ^ 1;
    _edgeOrient[5] = _edgeOrient[8] ^ 1;
    _edgeOrient[8] = _edgeOrient[4] ^ 1;
    _edgeOrient[4] = tempEdgeOri0 ^ 1;
}

void Cube::move2F()
{
    moveF();
    moveF();
}

void Cube::moveB()
{
    const uint8_t tempCornPerm7 = _cornerPerm[7];
    _cornerPerm[7] = _cornerPerm[3];
    _cornerPerm[3] = _cornerPerm[2];
    _cornerPerm[2] = _cornerPerm[6];
    _cornerPerm[6] = tempCornPerm7;

    const uint8_t tempCornOri7 = _cornerOrient[7];
    _cornerOrient[7] = static_cast<uint8_t>((_cornerOrient[3] + 2) % 3);
    _cornerOrient[3] = static_cast<uint8_t>((_cornerOrient[2] + 1) % 3);
    _cornerOrient[2] = static_cast<uint8_t>((_cornerOrient[6] + 2) % 3);
    _cornerOrient[6] = static_cast<uint8_t>((tempCornOri7 + 1) % 3);

    const uint8_t tempEdgePerm10 = _edgePerm[10];
    _edgePerm[10] = _edgePerm[7];
    _edgePerm[7] = _edgePerm[2];
    _edgePerm[2] = _edgePerm[6];
    _edgePerm[6] = tempEdgePerm10;

    const uint8_t tempEdgeOri10 = _edgeOrient[10];
    _edgeOrient[10] = _edgeOrient[7] ^ 1;
    _edgeOrient[7] = _edgeOrient[2] ^ 1;
    _edgeOrient[2] = _edgeOrient[6] ^ 1;
    _edgeOrient[6] = tempEdgeOri10 ^ 1;
}

void Cube::moveBPrime()
{
    const uint8_t tempCornPerm6 = _cornerPerm[6];
    _cornerPerm[6] = _cornerPerm[2];
    _cornerPerm[2] = _cornerPerm[3];
    _cornerPerm[3] = _cornerPerm[7];
    _cornerPerm[7] = tempCornPerm6;

    const uint8_t tempCornOri6 = _cornerOrient[6];
    _cornerOrient[6] = static_cast<uint8_t>((_cornerOrient[2] + 1) % 3);
    _cornerOrient[2] = static_cast<uint8_t>((_cornerOrient[3] + 2) % 3);
    _cornerOrient[3] = static_cast<uint8_t>((_cornerOrient[7] + 1) % 3);
    _cornerOrient[7] = static_cast<uint8_t>((tempCornOri6 + 2) % 3);

    const uint8_t tempEdgePerm6 = _edgePerm[6];
    _edgePerm[6] = _edgePerm[2];
    _edgePerm[2] = _edgePerm[7];
    _edgePerm[7] = _edgePerm[10];
    _edgePerm[10] = tempEdgePerm6;

    const uint8_t tempEdgeOri6 = _edgeOrient[6];
    _edgeOrient[6] = _edgeOrient[2] ^ 1;
    _edgeOrient[2] = _edgeOrient[7] ^ 1;
    _edgeOrient[7] = _edgeOrient[10] ^ 1;
    _edgeOrient[10] = tempEdgeOri6 ^ 1;
}

void Cube::move2B()
{
    moveB();
    moveB();
}

void Cube::moveR()
{
    const uint8_t tempCornPerm1 = _cornerPerm[1];
    _cornerPerm[1] = _cornerPerm[5];
    _cornerPerm[5] = _cornerPerm[6];
    _cornerPerm[6] = _cornerPerm[2];
    _cornerPerm[2] = tempCornPerm1;

    const uint8_t tempCornOri1 = _cornerOrient[1];
    _cornerOrient[1] = static_cast<uint8_t>((_cornerOrient[5] + 2) % 3);
    _cornerOrient[5] = static_cast<uint8_t>((_cornerOrient[6] + 1) % 3);
    _cornerOrient[6] = static_cast<uint8_t>((_cornerOrient[2] + 2) % 3);
    _cornerOrient[2] = static_cast<uint8_t>((tempCornOri1 + 1) % 3);

    const uint8_t tempEdgePerm1 = _edgePerm[1];
    _edgePerm[1] = _edgePerm[5];
    _edgePerm[5] = _edgePerm[9];
    _edgePerm[9] = _edgePerm[6];
    _edgePerm[6] = tempEdgePerm1;

    const uint8_t tempEdgeOri1 = _edgeOrient[1];
    _edgeOrient[1] = _edgeOrient[5];
    _edgeOrient[5] = _edgeOrient[9];
    _edgeOrient[9] = _edgeOrient[6];
    _edgeOrient[6] = tempEdgeOri1;
}

void Cube::moveRPrime()
{
    const uint8_t tempCornPerm2 = _cornerPerm[2];
    _cornerPerm[2] = _cornerPerm[6];
    _cornerPerm[6] = _cornerPerm[5];
    _cornerPerm[5] = _cornerPerm[1];
    _cornerPerm[1] = tempCornPerm2;

    const uint8_t tempCornOri2 = _cornerOrient[2];
    _cornerOrient[2] = static_cast<uint8_t>((_cornerOrient[6] + 1) % 3);
    _cornerOrient[6] = static_cast<uint8_t>((_cornerOrient[5] + 2) % 3);
    _cornerOrient[5] = static_cast<uint8_t>((_cornerOrient[1] + 1) % 3);
    _cornerOrient[1] = static_cast<uint8_t>((tempCornOri2 + 2) % 3);

    const uint8_t tempEdgePerm6 = _edgePerm[6];
    _edgePerm[6] = _edgePerm[9];
    _edgePerm[9] = _edgePerm[5];
    _edgePerm[5] = _edgePerm[1];
    _edgePerm[1] = tempEdgePerm6;

    const uint8_t tempEdgeOri6 = _edgeOrient[6];
    _edgeOrient[6] = _edgeOrient[9];
    _edgeOrient[9] = _edgeOrient[5];
    _edgeOrient[5] = _edgeOrient[1];
    _edgeOrient[1] = tempEdgeOri6;
}

void Cube::move2R()
{
    moveR();
    moveR();
}

void Cube::moveL()
{
    const uint8_t tempCornPerm0 = _cornerPerm[0];
    _cornerPerm[0] = _cornerPerm[3];
    _cornerPerm[3] = _cornerPerm[7];
    _cornerPerm[7] = _cornerPerm[4];
    _cornerPerm[4] = tempCornPerm0;

    const uint8_t tempCornOri0 = _cornerOrient[0];
    _cornerOrient[0] = static_cast<uint8_t>((_cornerOrient[3] + 1) % 3);
    _cornerOrient[3] = static_cast<uint8_t>((_cornerOrient[7] + 2) % 3);
    _cornerOrient[7] = static_cast<uint8_t>((_cornerOrient[4] + 1) % 3);
    _cornerOrient[4] = static_cast<uint8_t>((tempCornOri0 + 2) % 3);

    const uint8_t tempEdgePerm3 = _edgePerm[3];
    _edgePerm[3] = _edgePerm[7];
    _edgePerm[7] = _edgePerm[11];
    _edgePerm[11] = _edgePerm[4];
    _edgePerm[4] = tempEdgePerm3;

    const uint8_t tempEdgeOri3 = _edgeOrient[3];
    _edgeOrient[3] = _edgeOrient[7];
    _edgeOrient[7] = _edgeOrient[11];
    _edgeOrient[11] = _edgeOrient[4];
    _edgeOrient[4] = tempEdgeOri3;
}

void Cube::moveLPrime()
{
    const uint8_t tempCornPerm4 = _cornerPerm[4];
    _cornerPerm[4] = _cornerPerm[7];
    _cornerPerm[7] = _cornerPerm[3];
    _cornerPerm[3] = _cornerPerm[0];
    _cornerPerm[0] = tempCornPerm4;

    const uint8_t tempCornOri4 = _cornerOrient[4];
    _cornerOrient[4] = static_cast<uint8_t>((_cornerOrient[7] + 2) % 3);
    _cornerOrient[7] = static_cast<uint8_t>((_cornerOrient[3] + 1) % 3);
    _cornerOrient[3] = static_cast<uint8_t>((_cornerOrient[0] + 2) % 3);
    _cornerOrient[0] = static_cast<uint8_t>((tempCornOri4 + 1) % 3);

    const uint8_t tempEdgePerm4 = _edgePerm[4];
    _edgePerm[4] = _edgePerm[11];
    _edgePerm[11] = _edgePerm[7];
    _edgePerm[7] = _edgePerm[3];
    _edgePerm[3] = tempEdgePerm4;

    const uint8_t tempEdgeOri4 = _edgeOrient[4];
    _edgeOrient[4] = _edgeOrient[11];
    _edgeOrient[11] = _edgeOrient[7];
    _edgeOrient[7] = _edgeOrient[3];
    _edgeOrient[3] = tempEdgeOri4;
}

void Cube::move2L()
{
    moveL();
    moveL();
}

uint32_t Cube::getTwist() const
{
    uint32_t twist = 0;
    uint32_t factor = 1;
    for (uint8_t i = 0; i < (CORNER_COUNT - 1); ++i)
    {
        twist += _cornerOrient[i] * factor;
        factor *= 3;
    }
    return twist;
}

void Cube::setCornerOrientFromTwist(uint32_t twist)
{
    for (uint8_t i = 0; i < (CORNER_COUNT - 1); ++i)
    {
        _cornerOrient[i] = static_cast<uint8_t>(twist % 3);
        twist /= 3;
    }
    // The orientation of the last corner is determined by the first seven corners
    _cornerOrient[CORNER_COUNT - 1] =
        static_cast<uint8_t>((3 - (_cornerOrient[0] + _cornerOrient[1] + _cornerOrient[2] + _cornerOrient[3] +
                                   _cornerOrient[4] + _cornerOrient[5] + _cornerOrient[6]) %
                                      3) %
                             3);
}

uint32_t Cube::getFlip() const
{
    uint32_t flip = 0;
    uint32_t factor = 1;
    for (uint8_t i = 0; i < (EDGE_COUNT - 1); ++i)
    {
        flip += _edgeOrient[i] * factor;
        factor *= 2;
    }
    return flip;
}

void Cube::setEdgeOrientFromFlip(uint32_t flip)
{
    for (uint8_t i = 0; i < (EDGE_COUNT - 1); ++i)
    {
        _edgeOrient[i] = flip % 2;
        flip /= 2;
    }
    // The orientation of the last edge is determined by the first eleven edges
    _edgeOrient[EDGE_COUNT - 1] = 0;
    for (uint8_t i = 0; i < (EDGE_COUNT - 1); ++i)
    {
        _edgeOrient[EDGE_COUNT - 1] ^= _edgeOrient[i];
    }
}

uint32_t Cube::getUDSlice() const
{
    constexpr uint8_t FL = 4;
    constexpr uint8_t FR = 5;
    constexpr uint8_t BR = 6;
    constexpr uint8_t BL = 7;

    uint32_t udSlice = 0;
    size_t remainingSliceEdges = SLICE_EDGE_COUNT;
    for (int i = EDGE_COUNT - 1; i >= 0; --i)
    {
        if (_edgePerm[i] == FL || _edgePerm[i] == FR || _edgePerm[i] == BR || _edgePerm[i] == BL)
        {
            udSlice += _CValues[i][remainingSliceEdges];
            --remainingSliceEdges;
        }
    }

    return udSlice;
}

void Cube::setEdgePosFromUDSlice(uint32_t udSlice)
{
    constexpr uint8_t BL = 7;

    std::array<uint8_t, EDGE_COUNT> newEdgePerm;
    newEdgePerm.fill(0); // Initialize all edges to 0 (not in UD slice)
    int remainingSliceEdges = SLICE_EDGE_COUNT;
    uint8_t nextSliceEdge = BL;

    for (int i = EDGE_COUNT - 1; i >= 0 && remainingSliceEdges > 0; --i)
    {
        if (_CValues[i][remainingSliceEdges] <= udSlice)
        {
            newEdgePerm[i] = nextSliceEdge;
            udSlice -= _CValues[i][remainingSliceEdges];
            --remainingSliceEdges;
            --nextSliceEdge;
        }
    }

    _edgePerm = newEdgePerm;
}

void Cube::buildTwistMovesTable()
{
    for (uint32_t twist = 0; twist < TWIST_COUNT; ++twist)
    {
        for (size_t move = 0; move < Move::MovesCount; ++move)
        {
            setCornerOrientFromTwist(twist);
            _moveFunctions[move]();
            _twistMovesTable[twist][move] = getTwist();
        }
    }
}

void Cube::buildFlipMovesTable()
{
    for (uint32_t flip = 0; flip < FLIP_COUNT; ++flip)
    {
        for (size_t move = 0; move < Move::MovesCount; ++move)
        {
            setEdgeOrientFromFlip(flip);
            _moveFunctions[move]();
            _flipMovesTable[flip][move] = getFlip();
        }
    }
}

void Cube::buildUDSliceMovesTable()
{
    for (uint32_t udSlice = 0; udSlice < UDSLICE_COUNT; ++udSlice)
    {
        for (size_t move = 0; move < Move::MovesCount; ++move)
        {
            setEdgePosFromUDSlice(udSlice);
            _moveFunctions[move]();
            _udSliceMovesTable[udSlice][move] = getUDSlice();
        }
    }
}

void Cube::buildTwistSlicePtb()
{
    constexpr uint8_t notSetValue = std::numeric_limits<uint8_t>::max();
    resetCubeToSolved();
    uint32_t initialTwist = getTwist();
    uint32_t inititalUDSlice = getUDSlice();

    _twistSlicePtb.resize(TWIST_COUNT);
    for (auto& row : _twistSlicePtb)
    {
        row.resize(UDSLICE_COUNT);
        std::fill(row.begin(), row.end(), notSetValue);
    }

    _twistSlicePtb[initialTwist][inititalUDSlice] = 0; // The solved state has a distance of 0
    std::queue<std::pair<uint32_t, uint32_t>> nextStates;
    nextStates.push({initialTwist, inititalUDSlice});
    while (!nextStates.empty())
    {
        auto [currentTwist, currentUDSlice] = nextStates.front();
        nextStates.pop();
        for (size_t move = 0; move < Move::MovesCount; ++move)
        {
            uint32_t nextTwist = _twistMovesTable[currentTwist][move];
            uint32_t nextUDSlice = _udSliceMovesTable[currentUDSlice][move];
            if (_twistSlicePtb[nextTwist][nextUDSlice] == notSetValue)
            {
                _twistSlicePtb[nextTwist][nextUDSlice] = _twistSlicePtb[currentTwist][currentUDSlice] + 1;
                nextStates.push({nextTwist, nextUDSlice});
            }
        }
    }
}

void Cube::buildFlipSlicePtb()
{
    constexpr uint8_t notSetValue = std::numeric_limits<uint8_t>::max();
    resetCubeToSolved();
    uint32_t initialFlip = getFlip();
    uint32_t initialUDSlice = getUDSlice();

    _flipSlicePtb.resize(FLIP_COUNT);
    for (auto& row : _flipSlicePtb)
    {
        row.resize(UDSLICE_COUNT);
        std::fill(row.begin(), row.end(), notSetValue);
    }

    _flipSlicePtb[initialFlip][initialUDSlice] = 0; // The solved state has a distance of 0
    std::queue<std::pair<uint32_t, uint32_t>> nextStates;
    nextStates.push({initialFlip, initialUDSlice});
    while (!nextStates.empty())
    {
        auto [currentFlip, currentUDSlice] = nextStates.front();
        nextStates.pop();
        for (size_t move = 0; move < Move::MovesCount; ++move)
        {
            uint32_t nextFlip = _flipMovesTable[currentFlip][move];
            uint32_t nextUDSlice = _udSliceMovesTable[currentUDSlice][move];
            if (_flipSlicePtb[nextFlip][nextUDSlice] == notSetValue)
            {
                _flipSlicePtb[nextFlip][nextUDSlice] = _flipSlicePtb[currentFlip][currentUDSlice] + 1;
                nextStates.push({nextFlip, nextUDSlice});
            }
        }
    }
}

void Cube::findMovesToG1State()
{
    uint32_t currentTwist = getTwist();
    uint32_t currentFlip = getFlip();
    uint32_t currentUDSlice = getUDSlice();
    uint8_t currentLimit = std::max(_twistSlicePtb[currentTwist][currentUDSlice], _flipSlicePtb[currentFlip][currentUDSlice]);
    uint8_t depth = 0;
    std::vector<Move> movesSequence;
    while (true)
    {
        auto [result, resultMovesSequence] =
            searchStatesToGetToG1State(currentTwist, currentFlip, currentUDSlice, depth, currentLimit, movesSequence);
        if (result == 0)
        {
            for (Move move : resultMovesSequence)
            {
                std::cout << "Move: " << moveToString(move) << std::endl;
                _moveFunctions[move]();
            }
            return;
        }
        currentLimit = result;
    }
}

std::pair<uint8_t, std::vector<Cube::Move>> Cube::searchStatesToGetToG1State(uint32_t currentTwist,
                                                                             uint32_t currentFlip,
                                                                             uint32_t currentUDSlice,
                                                                             uint8_t depth,
                                                                             uint8_t currentLimit,
                                                                             std::vector<Move> movesSequence)
{
    uint8_t value = std::max(_twistSlicePtb[currentTwist][currentUDSlice], _flipSlicePtb[currentFlip][currentUDSlice]);
    if (value == 0)
    {
        return {value, movesSequence};
    }
    value += depth;
    if (value > currentLimit)
    {
        return {value, {}};
    }

    uint8_t minResult = std::numeric_limits<uint8_t>::max();
    std::vector<Move> bestMovesSequence; // moves sequence that leads to the state with minResult value
    uint32_t nextTwist;
    uint32_t nextFlip;
    uint32_t nextUDSlice;
    for (size_t move = 0; move < Move::MovesCount; ++move)
    {
        nextTwist = _twistMovesTable[currentTwist][move];
        nextFlip = _flipMovesTable[currentFlip][move];
        nextUDSlice = _udSliceMovesTable[currentUDSlice][move];

        movesSequence.push_back(static_cast<Move>(move));
        auto [tmpResult, tmpMovesSequence] =
            searchStatesToGetToG1State(nextTwist, nextFlip, nextUDSlice, depth + 1, currentLimit, movesSequence);
        if (tmpResult == 0)
        {
            return {tmpResult, tmpMovesSequence};
        }
        if (tmpResult < minResult)
        {
            minResult = tmpResult;
            bestMovesSequence = movesSequence;
        }
        movesSequence.pop_back();
    }
    return {minResult, bestMovesSequence};
}

std::string Cube::moveToString(Move move) const
{
    switch (move)
    {
        case Move::U:
            return "U";
        case Move::UPrime:
            return "U'";
        case Move::DoubleU:
            return "U2";
        case Move::D:
            return "D";
        case Move::DPrime:
            return "D'";
        case Move::DoubleD:
            return "D2";
        case Move::F:
            return "F";
        case Move::FPrime:
            return "F'";
        case Move::DoubleF:
            return "F2";
        case Move::B:
            return "B";
        case Move::BPrime:
            return "B'";
        case Move::DoubleB:
            return "B2";
        case Move::R:
            return "R";
        case Move::RPrime:
            return "R'";
        case Move::DoubleR:
            return "R2";
        case Move::L:
            return "L";
        case Move::LPrime:
            return "L'";
        case Move::DoubleL:
            return "L2";
        default:
            return "";
    }
}

void Cube::printCube() const
{
    std::cout << "Corner Permutation: ";
    for (const auto& corner : _cornerPerm)
    {
        std::cout << static_cast<int>(corner) << " ";
    }
    std::cout << "\nCorner Orientation: ";
    for (const auto& orient : _cornerOrient)
    {
        std::cout << static_cast<int>(orient) << " ";
    }
    std::cout << "\nEdge Permutation: ";
    for (const auto& edge : _edgePerm)
    {
        std::cout << static_cast<int>(edge) << " ";
    }
    std::cout << "\nEdge Orientation: ";
    for (const auto& orient : _edgeOrient)
    {
        std::cout << static_cast<int>(orient) << " ";
    }
    std::cout << std::endl;
}
