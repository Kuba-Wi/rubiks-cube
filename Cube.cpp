#include "Cube.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>

Cube::Cube()
{
    for (size_t n = 0; n < Edge::EdgesCount; ++n)
    {
        _CValues[n][0] = 1;
    }
    for (size_t k = 1; k <= SLICE_EDGE_COUNT; ++k)
    {
        _CValues[0][k] = 0;
    }

    for (size_t n = 1; n < Edge::EdgesCount; ++n)
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

    _moveFunctionsPhase2 =
    {
        {Move::U, [this]() { moveU(); }},
        {Move::UPrime, [this]() { moveUPrime(); }},
        {Move::DoubleU, [this]() { move2U(); }},
        {Move::D, [this]() { moveD(); }},
        {Move::DPrime, [this]() { moveDPrime(); }},
        {Move::DoubleD, [this]() { move2D(); }},
        {Move::DoubleF, [this]() { move2F(); }},
        {Move::DoubleB, [this]() { move2B(); }},
        {Move::DoubleR, [this]() { move2R(); }},
        {Move::DoubleL, [this]() { move2L(); }}
    };
    // clang-format on

    buildFlipMovesTable();
    buildTwistMovesTable();
    buildUDSliceMovesTable();
    buildTwistSlicePtb();
    buildFlipSlicePtb();

    buildCornerPermMovesTable();
    buildTopBottomEdgePermMovesTable();
    buildUDSlicePermMovesTable();
    buildCornerSlicePermPtb();
    buildEdgeTopBottomSlicePermPtb();

    resetCubeToSolved();
}

void Cube::resetCubeToSolved()
{
    _cornerOrient.fill(0);
    _edgeOrient.fill(0);

    std::iota(_cornerPerm.begin(), _cornerPerm.end(), 0);
    std::iota(_edgePerm.begin(), _edgePerm.end(), 0);
}

void Cube::solveCube()
{
    std::vector<Move> movesToG1 = findMovesToG1State();
    for (Move move : movesToG1)
    {
        _moveFunctions[move]();
        std::cout << "Move to G1: " << moveToString(move) << std::endl;
    }
    std::vector<Move> movesToSolved = findMovesFromG1ToSolvedState();

    // for (Move move : movesToG1)
    // {
    //     std::cout << "Move: " << moveToString(move) << std::endl;
    // }

    for (Move move : movesToSolved)
    {
        std::cout << "Move: " << moveToString(move) << std::endl;
        _moveFunctions[move]();
    }
}

void Cube::moveU()
{
    const uint8_t tempCornPerm = _cornerPerm[Corner::UFL];
    _cornerPerm[Corner::UFL] = _cornerPerm[Corner::UFR];
    _cornerPerm[Corner::UFR] = _cornerPerm[Corner::UBR];
    _cornerPerm[Corner::UBR] = _cornerPerm[Corner::UBL];
    _cornerPerm[Corner::UBL] = tempCornPerm;

    const uint8_t tempCornOri = _cornerOrient[Corner::UFL];
    _cornerOrient[Corner::UFL] = _cornerOrient[Corner::UFR];
    _cornerOrient[Corner::UFR] = _cornerOrient[Corner::UBR];
    _cornerOrient[Corner::UBR] = _cornerOrient[Corner::UBL];
    _cornerOrient[Corner::UBL] = tempCornOri;

    const uint8_t tempEdgePerm = _edgePerm[Edge::UF];
    _edgePerm[Edge::UF] = _edgePerm[Edge::UR];
    _edgePerm[Edge::UR] = _edgePerm[Edge::UB];
    _edgePerm[Edge::UB] = _edgePerm[Edge::UL];
    _edgePerm[Edge::UL] = tempEdgePerm;

    const uint8_t tempEdgeOri = _edgeOrient[Edge::UF];
    _edgeOrient[Edge::UF] = _edgeOrient[Edge::UR];
    _edgeOrient[Edge::UR] = _edgeOrient[Edge::UB];
    _edgeOrient[Edge::UB] = _edgeOrient[Edge::UL];
    _edgeOrient[Edge::UL] = tempEdgeOri;
}

void Cube::moveUPrime()
{
    const uint8_t tempCornPerm = _cornerPerm[Corner::UBL];
    _cornerPerm[Corner::UBL] = _cornerPerm[Corner::UBR];
    _cornerPerm[Corner::UBR] = _cornerPerm[Corner::UFR];
    _cornerPerm[Corner::UFR] = _cornerPerm[Corner::UFL];
    _cornerPerm[Corner::UFL] = tempCornPerm;

    const uint8_t tempCornOri = _cornerOrient[Corner::UBL];
    _cornerOrient[Corner::UBL] = _cornerOrient[Corner::UBR];
    _cornerOrient[Corner::UBR] = _cornerOrient[Corner::UFR];
    _cornerOrient[Corner::UFR] = _cornerOrient[Corner::UFL];
    _cornerOrient[Corner::UFL] = tempCornOri;

    const uint8_t tempEdgePerm = _edgePerm[Edge::UL];
    _edgePerm[Edge::UL] = _edgePerm[Edge::UB];
    _edgePerm[Edge::UB] = _edgePerm[Edge::UR];
    _edgePerm[Edge::UR] = _edgePerm[Edge::UF];
    _edgePerm[Edge::UF] = tempEdgePerm;

    const uint8_t tempEdgeOri = _edgeOrient[Edge::UL];
    _edgeOrient[Edge::UL] = _edgeOrient[Edge::UB];
    _edgeOrient[Edge::UB] = _edgeOrient[Edge::UR];
    _edgeOrient[Edge::UR] = _edgeOrient[Edge::UF];
    _edgeOrient[Edge::UF] = tempEdgeOri;
}

void Cube::move2U()
{
    moveU();
    moveU();
}

void Cube::moveD()
{
    const uint8_t tempCornPerm = _cornerPerm[Corner::DBR];
    _cornerPerm[Corner::DBR] = _cornerPerm[Corner::DFR];
    _cornerPerm[Corner::DFR] = _cornerPerm[Corner::DFL];
    _cornerPerm[Corner::DFL] = _cornerPerm[Corner::DRL];
    _cornerPerm[Corner::DRL] = tempCornPerm;

    const uint8_t tempCornOri = _cornerOrient[Corner::DBR];
    _cornerOrient[Corner::DBR] = _cornerOrient[Corner::DFR];
    _cornerOrient[Corner::DFR] = _cornerOrient[Corner::DFL];
    _cornerOrient[Corner::DFL] = _cornerOrient[Corner::DRL];
    _cornerOrient[Corner::DRL] = tempCornOri;

    const uint8_t tempEdgePerm = _edgePerm[Edge::BD];
    _edgePerm[Edge::BD] = _edgePerm[Edge::RD];
    _edgePerm[Edge::RD] = _edgePerm[Edge::FD];
    _edgePerm[Edge::FD] = _edgePerm[Edge::LD];
    _edgePerm[Edge::LD] = tempEdgePerm;

    const uint8_t tempEdgeOri = _edgeOrient[Edge::BD];
    _edgeOrient[Edge::BD] = _edgeOrient[Edge::RD];
    _edgeOrient[Edge::RD] = _edgeOrient[Edge::FD];
    _edgeOrient[Edge::FD] = _edgeOrient[Edge::LD];
    _edgeOrient[Edge::LD] = tempEdgeOri;
}

void Cube::moveDPrime()
{
    const uint8_t tempCornPerm = _cornerPerm[Corner::DRL];
    _cornerPerm[Corner::DRL] = _cornerPerm[Corner::DFL];
    _cornerPerm[Corner::DFL] = _cornerPerm[Corner::DFR];
    _cornerPerm[Corner::DFR] = _cornerPerm[Corner::DBR];
    _cornerPerm[Corner::DBR] = tempCornPerm;

    const uint8_t tempCornOri = _cornerOrient[Corner::DRL];
    _cornerOrient[Corner::DRL] = _cornerOrient[Corner::DFL];
    _cornerOrient[Corner::DFL] = _cornerOrient[Corner::DFR];
    _cornerOrient[Corner::DFR] = _cornerOrient[Corner::DBR];
    _cornerOrient[Corner::DBR] = tempCornOri;

    const uint8_t tempEdgePerm = _edgePerm[Edge::LD];
    _edgePerm[Edge::LD] = _edgePerm[Edge::FD];
    _edgePerm[Edge::FD] = _edgePerm[Edge::RD];
    _edgePerm[Edge::RD] = _edgePerm[Edge::BD];
    _edgePerm[Edge::BD] = tempEdgePerm;

    const uint8_t tempEdgeOri = _edgeOrient[Edge::LD];
    _edgeOrient[Edge::LD] = _edgeOrient[Edge::FD];
    _edgeOrient[Edge::FD] = _edgeOrient[Edge::RD];
    _edgeOrient[Edge::RD] = _edgeOrient[Edge::BD];
    _edgeOrient[Edge::BD] = tempEdgeOri;
}

void Cube::move2D()
{
    moveD();
    moveD();
}

void Cube::moveF()
{
    const uint8_t tempCornPerm4 = _cornerPerm[Corner::DFL];
    _cornerPerm[Corner::DFL] = _cornerPerm[Corner::DFR];
    _cornerPerm[Corner::DFR] = _cornerPerm[Corner::UFR];
    _cornerPerm[Corner::UFR] = _cornerPerm[Corner::UFL];
    _cornerPerm[Corner::UFL] = tempCornPerm4;

    const uint8_t tempCornOri = _cornerOrient[Corner::DFR];
    _cornerOrient[Corner::DFR] = static_cast<uint8_t>((_cornerOrient[Corner::UFR] + 2) % 3);
    _cornerOrient[Corner::UFR] = static_cast<uint8_t>((_cornerOrient[Corner::UFL] + 1) % 3);
    _cornerOrient[Corner::UFL] = static_cast<uint8_t>((_cornerOrient[Corner::DFL] + 2) % 3);
    _cornerOrient[Corner::DFL] = static_cast<uint8_t>((tempCornOri + 1) % 3);

    const uint8_t tempEdgePerm4 = _edgePerm[Edge::FL];
    _edgePerm[Edge::FL] = _edgePerm[Edge::FD];
    _edgePerm[Edge::FD] = _edgePerm[Edge::FR];
    _edgePerm[Edge::FR] = _edgePerm[Edge::UF];
    _edgePerm[Edge::UF] = tempEdgePerm4;

    const uint8_t tempEdgeOri = _edgeOrient[Edge::FL];
    _edgeOrient[Edge::FL] = _edgeOrient[Edge::FD] ^ 1;
    _edgeOrient[Edge::FD] = _edgeOrient[Edge::FR] ^ 1;
    _edgeOrient[Edge::FR] = _edgeOrient[Edge::UF] ^ 1;
    _edgeOrient[Edge::UF] = tempEdgeOri ^ 1;
}

void Cube::moveFPrime()
{
    const uint8_t tempCornPerm = _cornerPerm[Corner::UFL];
    _cornerPerm[Corner::UFL] = _cornerPerm[Corner::UFR];
    _cornerPerm[Corner::UFR] = _cornerPerm[Corner::DFR];
    _cornerPerm[Corner::DFR] = _cornerPerm[Corner::DFL];
    _cornerPerm[Corner::DFL] = tempCornPerm;

    const uint8_t tempCornOri = _cornerOrient[Corner::UFL];
    _cornerOrient[Corner::UFL] = static_cast<uint8_t>((_cornerOrient[Corner::UFR] + 2) % 3);
    _cornerOrient[Corner::UFR] = static_cast<uint8_t>((_cornerOrient[Corner::DFR] + 1) % 3);
    _cornerOrient[Corner::DFR] = static_cast<uint8_t>((_cornerOrient[Corner::DFL] + 2) % 3);
    _cornerOrient[Corner::DFL] = static_cast<uint8_t>((tempCornOri + 1) % 3);

    const uint8_t tempEdgePerm = _edgePerm[Edge::UF];
    _edgePerm[Edge::UF] = _edgePerm[Edge::FR];
    _edgePerm[Edge::FR] = _edgePerm[Edge::FD];
    _edgePerm[Edge::FD] = _edgePerm[Edge::FL];
    _edgePerm[Edge::FL] = tempEdgePerm;

    const uint8_t tempEdgeOri = _edgeOrient[Edge::UF];
    _edgeOrient[Edge::UF] = _edgeOrient[Edge::FR] ^ 1;
    _edgeOrient[Edge::FR] = _edgeOrient[Edge::FD] ^ 1;
    _edgeOrient[Edge::FD] = _edgeOrient[Edge::FL] ^ 1;
    _edgeOrient[Edge::FL] = tempEdgeOri ^ 1;
}

void Cube::move2F()
{
    moveF();
    moveF();
}

void Cube::moveB()
{
    const uint8_t tempCornPerm = _cornerPerm[Corner::DRL];
    _cornerPerm[Corner::DRL] = _cornerPerm[Corner::UBL];
    _cornerPerm[Corner::UBL] = _cornerPerm[Corner::UBR];
    _cornerPerm[Corner::UBR] = _cornerPerm[Corner::DBR];
    _cornerPerm[Corner::DBR] = tempCornPerm;

    const uint8_t tempCornOri = _cornerOrient[Corner::DRL];
    _cornerOrient[Corner::DRL] = static_cast<uint8_t>((_cornerOrient[Corner::UBL] + 2) % 3);
    _cornerOrient[Corner::UBL] = static_cast<uint8_t>((_cornerOrient[Corner::UBR] + 1) % 3);
    _cornerOrient[Corner::UBR] = static_cast<uint8_t>((_cornerOrient[Corner::DBR] + 2) % 3);
    _cornerOrient[Corner::DBR] = static_cast<uint8_t>((tempCornOri + 1) % 3);

    const uint8_t tempEdgePerm = _edgePerm[Edge::BD];
    _edgePerm[Edge::BD] = _edgePerm[Edge::BL];
    _edgePerm[Edge::BL] = _edgePerm[Edge::UB];
    _edgePerm[Edge::UB] = _edgePerm[Edge::BR];
    _edgePerm[Edge::BR] = tempEdgePerm;

    const uint8_t tempEdgeOri = _edgeOrient[Edge::BD];
    _edgeOrient[Edge::BD] = _edgeOrient[Edge::BL] ^ 1;
    _edgeOrient[Edge::BL] = _edgeOrient[Edge::UB] ^ 1;
    _edgeOrient[Edge::UB] = _edgeOrient[Edge::BR] ^ 1;
    _edgeOrient[Edge::BR] = tempEdgeOri ^ 1;
}

void Cube::moveBPrime()
{
    const uint8_t tempCornPerm = _cornerPerm[Corner::DBR];
    _cornerPerm[Corner::DBR] = _cornerPerm[Corner::UBR];
    _cornerPerm[Corner::UBR] = _cornerPerm[Corner::UBL];
    _cornerPerm[Corner::UBL] = _cornerPerm[Corner::DRL];
    _cornerPerm[Corner::DRL] = tempCornPerm;

    const uint8_t tempCornOri = _cornerOrient[Corner::DBR];
    _cornerOrient[Corner::DBR] = static_cast<uint8_t>((_cornerOrient[Corner::UBR] + 1) % 3);
    _cornerOrient[Corner::UBR] = static_cast<uint8_t>((_cornerOrient[Corner::UBL] + 2) % 3);
    _cornerOrient[Corner::UBL] = static_cast<uint8_t>((_cornerOrient[Corner::DRL] + 1) % 3);
    _cornerOrient[Corner::DRL] = static_cast<uint8_t>((tempCornOri + 2) % 3);

    const uint8_t tempEdgePerm = _edgePerm[Edge::BR];
    _edgePerm[Edge::BR] = _edgePerm[Edge::UB];
    _edgePerm[Edge::UB] = _edgePerm[Edge::BL];
    _edgePerm[Edge::BL] = _edgePerm[Edge::BD];
    _edgePerm[Edge::BD] = tempEdgePerm;

    const uint8_t tempEdgeOri = _edgeOrient[Edge::BR];
    _edgeOrient[Edge::BR] = _edgeOrient[Edge::UB] ^ 1;
    _edgeOrient[Edge::UB] = _edgeOrient[Edge::BL] ^ 1;
    _edgeOrient[Edge::BL] = _edgeOrient[Edge::BD] ^ 1;
    _edgeOrient[Edge::BD] = tempEdgeOri ^ 1;
}

void Cube::move2B()
{
    moveB();
    moveB();
}

void Cube::moveR()
{
    const uint8_t tempCornPerm = _cornerPerm[Corner::UFR];
    _cornerPerm[Corner::UFR] = _cornerPerm[Corner::DFR];
    _cornerPerm[Corner::DFR] = _cornerPerm[Corner::DBR];
    _cornerPerm[Corner::DBR] = _cornerPerm[Corner::UBR];
    _cornerPerm[Corner::UBR] = tempCornPerm;

    const uint8_t tempCornOri = _cornerOrient[Corner::UFR];
    _cornerOrient[Corner::UFR] = static_cast<uint8_t>((_cornerOrient[Corner::DFR] + 2) % 3);
    _cornerOrient[Corner::DFR] = static_cast<uint8_t>((_cornerOrient[Corner::DBR] + 1) % 3);
    _cornerOrient[Corner::DBR] = static_cast<uint8_t>((_cornerOrient[Corner::UBR] + 2) % 3);
    _cornerOrient[Corner::UBR] = static_cast<uint8_t>((tempCornOri + 1) % 3);

    const uint8_t tempEdgePerm = _edgePerm[Edge::UR];
    _edgePerm[Edge::UR] = _edgePerm[Edge::FR];
    _edgePerm[Edge::FR] = _edgePerm[Edge::RD];
    _edgePerm[Edge::RD] = _edgePerm[Edge::BR];
    _edgePerm[Edge::BR] = tempEdgePerm;

    const uint8_t tempEdgeOri = _edgeOrient[Edge::UR];
    _edgeOrient[Edge::UR] = _edgeOrient[Edge::FR];
    _edgeOrient[Edge::FR] = _edgeOrient[Edge::RD];
    _edgeOrient[Edge::RD] = _edgeOrient[Edge::BR];
    _edgeOrient[Edge::BR] = tempEdgeOri;
}

void Cube::moveRPrime()
{
    const uint8_t tempCornPerm = _cornerPerm[Corner::UBR];
    _cornerPerm[Corner::UBR] = _cornerPerm[Corner::DBR];
    _cornerPerm[Corner::DBR] = _cornerPerm[Corner::DFR];
    _cornerPerm[Corner::DFR] = _cornerPerm[Corner::UFR];
    _cornerPerm[Corner::UFR] = tempCornPerm;

    const uint8_t tempCornOri = _cornerOrient[Corner::UBR];
    _cornerOrient[Corner::UBR] = static_cast<uint8_t>((_cornerOrient[Corner::DBR] + 1) % 3);
    _cornerOrient[Corner::DBR] = static_cast<uint8_t>((_cornerOrient[Corner::DFR] + 2) % 3);
    _cornerOrient[Corner::DFR] = static_cast<uint8_t>((_cornerOrient[Corner::UFR] + 1) % 3);
    _cornerOrient[Corner::UFR] = static_cast<uint8_t>((tempCornOri + 2) % 3);

    const uint8_t tempEdgePerm = _edgePerm[Edge::BR];
    _edgePerm[Edge::BR] = _edgePerm[Edge::RD];
    _edgePerm[Edge::RD] = _edgePerm[Edge::FR];
    _edgePerm[Edge::FR] = _edgePerm[Edge::UR];
    _edgePerm[Edge::UR] = tempEdgePerm;

    const uint8_t tempEdgeOri = _edgeOrient[Edge::BR];
    _edgeOrient[Edge::BR] = _edgeOrient[Edge::RD];
    _edgeOrient[Edge::RD] = _edgeOrient[Edge::FR];
    _edgeOrient[Edge::FR] = _edgeOrient[Edge::UR];
    _edgeOrient[Edge::UR] = tempEdgeOri;
}

void Cube::move2R()
{
    moveR();
    moveR();
}

void Cube::moveL()
{
    const uint8_t tempCornPerm = _cornerPerm[Corner::UFL];
    _cornerPerm[Corner::UFL] = _cornerPerm[Corner::UBL];
    _cornerPerm[Corner::UBL] = _cornerPerm[Corner::DRL];
    _cornerPerm[Corner::DRL] = _cornerPerm[Corner::DFL];
    _cornerPerm[Corner::DFL] = tempCornPerm;

    const uint8_t tempCornOri = _cornerOrient[Corner::UFL];
    _cornerOrient[Corner::UFL] = static_cast<uint8_t>((_cornerOrient[Corner::UBL] + 1) % 3);
    _cornerOrient[Corner::UBL] = static_cast<uint8_t>((_cornerOrient[Corner::DRL] + 2) % 3);
    _cornerOrient[Corner::DRL] = static_cast<uint8_t>((_cornerOrient[Corner::DFL] + 1) % 3);
    _cornerOrient[Corner::DFL] = static_cast<uint8_t>((tempCornOri + 2) % 3);

    const uint8_t tempEdgePerm = _edgePerm[Edge::UL];
    _edgePerm[Edge::UL] = _edgePerm[Edge::BL];
    _edgePerm[Edge::BL] = _edgePerm[Edge::LD];
    _edgePerm[Edge::LD] = _edgePerm[Edge::FL];
    _edgePerm[Edge::FL] = tempEdgePerm;

    const uint8_t tempEdgeOri = _edgeOrient[Edge::UL];
    _edgeOrient[Edge::UL] = _edgeOrient[Edge::BL];
    _edgeOrient[Edge::BL] = _edgeOrient[Edge::LD];
    _edgeOrient[Edge::LD] = _edgeOrient[Edge::FL];
    _edgeOrient[Edge::FL] = tempEdgeOri;
}

void Cube::moveLPrime()
{
    const uint8_t tempCornPerm = _cornerPerm[Corner::DFL];
    _cornerPerm[Corner::DFL] = _cornerPerm[Corner::DRL];
    _cornerPerm[Corner::DRL] = _cornerPerm[Corner::UBL];
    _cornerPerm[Corner::UBL] = _cornerPerm[Corner::UFL];
    _cornerPerm[Corner::UFL] = tempCornPerm;

    const uint8_t tempCornOri = _cornerOrient[Corner::DFL];
    _cornerOrient[Corner::DFL] = static_cast<uint8_t>((_cornerOrient[Corner::DRL] + 2) % 3);
    _cornerOrient[Corner::DRL] = static_cast<uint8_t>((_cornerOrient[Corner::UBL] + 1) % 3);
    _cornerOrient[Corner::UBL] = static_cast<uint8_t>((_cornerOrient[Corner::UFL] + 2) % 3);
    _cornerOrient[Corner::UFL] = static_cast<uint8_t>((tempCornOri + 1) % 3);

    const uint8_t tempEdgePerm = _edgePerm[Edge::FL];
    _edgePerm[Edge::FL] = _edgePerm[Edge::LD];
    _edgePerm[Edge::LD] = _edgePerm[Edge::BL];
    _edgePerm[Edge::BL] = _edgePerm[Edge::UL];
    _edgePerm[Edge::UL] = tempEdgePerm;

    const uint8_t tempEdgeOri = _edgeOrient[Edge::FL];
    _edgeOrient[Edge::FL] = _edgeOrient[Edge::LD];
    _edgeOrient[Edge::LD] = _edgeOrient[Edge::BL];
    _edgeOrient[Edge::BL] = _edgeOrient[Edge::UL];
    _edgeOrient[Edge::UL] = tempEdgeOri;
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
    for (uint8_t i = 0; i < (Corner::CornersCount - 1); ++i)
    {
        twist += _cornerOrient[i] * factor;
        factor *= 3;
    }
    return twist;
}

void Cube::setCornerOrientFromTwist(uint32_t twist)
{
    for (uint8_t i = 0; i < (Corner::CornersCount - 1); ++i)
    {
        _cornerOrient[i] = static_cast<uint8_t>(twist % 3);
        twist /= 3;
    }
    // The orientation of the last corner is determined by the first seven corners
    _cornerOrient[Corner::CornersCount - 1] =
        static_cast<uint8_t>((3 - (_cornerOrient[0] + _cornerOrient[1] + _cornerOrient[2] + _cornerOrient[3] +
                                   _cornerOrient[4] + _cornerOrient[5] + _cornerOrient[6]) %
                                      3) %
                             3);
}

uint32_t Cube::getFlip() const
{
    uint32_t flip = 0;
    uint32_t factor = 1;
    for (uint8_t i = 0; i < (Edge::EdgesCount - 1); ++i)
    {
        flip += _edgeOrient[i] * factor;
        factor *= 2;
    }
    return flip;
}

void Cube::setEdgeOrientFromFlip(uint32_t flip)
{
    for (uint8_t i = 0; i < (Edge::EdgesCount - 1); ++i)
    {
        _edgeOrient[i] = flip % 2;
        flip /= 2;
    }
    // The orientation of the last edge is determined by the first eleven edges
    _edgeOrient[Edge::EdgesCount - 1] = 0;
    for (uint8_t i = 0; i < (Edge::EdgesCount - 1); ++i)
    {
        _edgeOrient[Edge::EdgesCount - 1] ^= _edgeOrient[i];
    }
}

uint32_t Cube::getUDSlice() const
{
    uint32_t udSlice = 0;
    size_t remainingSliceEdges = SLICE_EDGE_COUNT;
    for (int i = Edge::EdgesCount - 1; i >= 0; --i)
    {
        if (_edgePerm[i] == Edge::FL || _edgePerm[i] == Edge::FR || _edgePerm[i] == Edge::BR || _edgePerm[i] == Edge::BL)
        {
            udSlice += _CValues[i][remainingSliceEdges];
            --remainingSliceEdges;
        }
    }

    return udSlice;
}

void Cube::setEdgePosFromUDSlice(uint32_t udSlice)
{
    std::array<uint8_t, Edge::EdgesCount> newEdgePerm;
    newEdgePerm.fill(0); // Initialize all edges to 0 (not in UD slice)
    int remainingSliceEdges = SLICE_EDGE_COUNT;
    uint8_t nextSliceEdge = Edge::BL;

    for (int i = Edge::EdgesCount - 1; i >= 0 && remainingSliceEdges > 0; --i)
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

uint32_t Cube::getCornerPerm() const
{
    auto lehmerCode = _cornerPerm;
    uint8_t smallerCount = 0;
    for (size_t i = 0; i < Corner::CornersCount; ++i)
    {
        smallerCount = 0;
        for (size_t j = i + 1; j < Corner::CornersCount; ++j)
        {
            if (lehmerCode[j] < lehmerCode[i])
            {
                ++smallerCount;
            }
        }
        lehmerCode[i] = smallerCount;
    }
    uint32_t factorial = 1;
    uint32_t result = 0;
    for (int i = Corner::CornersCount - 2; i >= 0; --i)
    {
        factorial *= (Corner::CornersCount - 1 - i);
        result += lehmerCode[i] * factorial;
    }

    return result;
}

void Cube::setCornerPermFromIndex(uint32_t index)
{
    std::array<uint32_t, Corner::CornersCount> lehmerCode;
    lehmerCode.fill(0);
    uint32_t factorial = 1;
    for (int i = 1; i < Corner::CornersCount; ++i)
    {
        factorial *= i;
    }

    for (int i = 0; i < Corner::CornersCount - 1; ++i)
    {
        lehmerCode[i] = index / factorial;
        index %= factorial;
        factorial /= (Corner::CornersCount - 1 - i);
    }

    std::vector<uint8_t> availableCorners(Corner::CornersCount);
    std::iota(availableCorners.begin(), availableCorners.end(), 0);
    for (size_t i = 0; i < Corner::CornersCount; ++i)
    {
        _cornerPerm[i] = availableCorners[lehmerCode[i]];
        availableCorners.erase(availableCorners.begin() + lehmerCode[i]);
    }
}

uint32_t Cube::getTopBottomEdgePerm() const
{
    const std::array<uint8_t, Edge::EdgesCountPhase2> topBottomEdges = {
        Edge::UF, Edge::UR, Edge::UB, Edge::UL, Edge::FD, Edge::RD, Edge::BD, Edge::LD};

    std::vector<uint32_t> lehmerCode(Edge::EdgesCountPhase2);
    for (size_t i = 0; i < Edge::EdgesCountPhase2; ++i)
    {
        lehmerCode[i] = _edgePerm[topBottomEdges[i]];
    }

    uint8_t smallerCount = 0;
    for (size_t i = 0; i < Edge::EdgesCountPhase2; ++i)
    {
        smallerCount = 0;
        for (size_t j = i + 1; j < Edge::EdgesCountPhase2; ++j)
        {
            if (lehmerCode[j] < lehmerCode[i])
            {
                ++smallerCount;
            }
        }
        lehmerCode[i] = smallerCount;
    }
    uint32_t factorial = 1;
    uint32_t result = 0;
    for (int i = Edge::EdgesCountPhase2 - 2; i >= 0; --i)
    {
        factorial *= (Edge::EdgesCountPhase2 - 1 - i);
        result += lehmerCode[i] * factorial;
    }

    return result;
}

void Cube::setTopBottomEdgePermFromIndex(uint32_t index)
{
    const std::array<uint8_t, Edge::EdgesCountPhase2> topBottomEdges = {
        Edge::UF, Edge::UR, Edge::UB, Edge::UL, Edge::FD, Edge::RD, Edge::BD, Edge::LD};

    std::array<uint32_t, Edge::EdgesCountPhase2> lehmerCode;
    lehmerCode.fill(0);
    uint32_t factorial = 1;
    for (int i = 1; i < Edge::EdgesCountPhase2; ++i)
    {
        factorial *= i;
    }

    for (int i = 0; i < Edge::EdgesCountPhase2 - 1; ++i)
    {
        lehmerCode[i] = index / factorial;
        index %= factorial;
        factorial /= (Edge::EdgesCountPhase2 - 1 - i);
    }

    std::vector<uint8_t> availableEdges(Edge::EdgesCountPhase2);
    std::iota(availableEdges.begin(), availableEdges.end(), 0);
    _edgePerm.fill(0);
    for (size_t i = 0; i < Edge::EdgesCountPhase2; ++i)
    {
        _edgePerm[topBottomEdges[i]] = availableEdges[lehmerCode[i]];
        availableEdges.erase(availableEdges.begin() + lehmerCode[i]);
    }
}

uint32_t Cube::getUDSlicePerm() const
{
    const std::array<uint8_t, Edge::EdgesCountUDSlice> udSliceEdges = {Edge::FL, Edge::FR, Edge::BR, Edge::BL};

    std::vector<uint32_t> lehmerCode(Edge::EdgesCountUDSlice);
    for (size_t i = 0; i < Edge::EdgesCountUDSlice; ++i)
    {
        lehmerCode[i] = _edgePerm[udSliceEdges[i]];
    }

    uint8_t smallerCount = 0;
    for (size_t i = 0; i < Edge::EdgesCountUDSlice; ++i)
    {
        smallerCount = 0;
        for (size_t j = i + 1; j < Edge::EdgesCountUDSlice; ++j)
        {
            if (lehmerCode[j] < lehmerCode[i])
            {
                ++smallerCount;
            }
        }
        lehmerCode[i] = smallerCount;
    }
    uint32_t factorial = 1;
    uint32_t result = 0;
    for (int i = Edge::EdgesCountUDSlice - 2; i >= 0; --i)
    {
        factorial *= (Edge::EdgesCountUDSlice - 1 - i);
        result += lehmerCode[i] * factorial;
    }

    return result;
}

void Cube::setUDSlicePermFromIndex(uint32_t index)
{
    const std::array<uint8_t, Edge::EdgesCountUDSlice> udSliceEdges = {Edge::FL, Edge::FR, Edge::BR, Edge::BL};

    std::array<uint32_t, Edge::EdgesCountUDSlice> lehmerCode;
    lehmerCode.fill(0);
    uint32_t factorial = 1;
    for (int i = 1; i < Edge::EdgesCountUDSlice; ++i)
    {
        factorial *= i;
    }

    for (int i = 0; i < Edge::EdgesCountUDSlice - 1; ++i)
    {
        lehmerCode[i] = index / factorial;
        index %= factorial;
        factorial /= (Edge::EdgesCountUDSlice - 1 - i);
    }

    std::vector<uint8_t> availableEdges(Edge::EdgesCountUDSlice);
    std::iota(availableEdges.begin(), availableEdges.end(), static_cast<uint8_t>(Edge::FL));
    _edgePerm.fill(0);
    for (size_t i = 0; i < Edge::EdgesCountUDSlice; ++i)
    {
        _edgePerm[udSliceEdges[i]] = availableEdges[lehmerCode[i]];
        availableEdges.erase(availableEdges.begin() + lehmerCode[i]);
    }
}

void Cube::buildTwistMovesTable()
{
    for (uint32_t twist = 0; twist < TWIST_COUNT; ++twist)
    {
        for (const auto& [move, moveFunction] : _moveFunctions)
        {
            setCornerOrientFromTwist(twist);
            moveFunction();
            _twistMovesTable[twist][move] = getTwist();
        }
    }
}

void Cube::buildFlipMovesTable()
{
    for (uint32_t flip = 0; flip < FLIP_COUNT; ++flip)
    {
        for (const auto& [move, moveFunction] : _moveFunctions)
        {
            setEdgeOrientFromFlip(flip);
            moveFunction();
            _flipMovesTable[flip][move] = getFlip();
        }
    }
}

void Cube::buildUDSliceMovesTable()
{
    for (uint32_t udSlice = 0; udSlice < UDSLICE_COUNT; ++udSlice)
    {
        for (const auto& [move, moveFunction] : _moveFunctions)
        {
            setEdgePosFromUDSlice(udSlice);
            moveFunction();
            _udSliceMovesTable[udSlice][move] = getUDSlice();
        }
    }
}

void Cube::buildCornerPermMovesTable()
{
    for (uint32_t cornerPerm = 0; cornerPerm < CORNER_PERM_COUNT; ++cornerPerm)
    {
        for (const auto& [moveIndex, moveFunction] : _moveFunctionsPhase2)
        {
            setCornerPermFromIndex(cornerPerm);
            moveFunction();
            _cornerPermMovesTable[cornerPerm][moveIndex] = getCornerPerm();
        }
    }
}

void Cube::buildTopBottomEdgePermMovesTable()
{
    for (uint32_t edgePerm = 0; edgePerm < EDGE_PERM_COUNT; ++edgePerm)
    {
        for (const auto& [moveIndex, moveFunction] : _moveFunctionsPhase2)
        {
            setTopBottomEdgePermFromIndex(edgePerm);
            moveFunction();
            _edgeTopBottomPermMovesTable[edgePerm][moveIndex] = getTopBottomEdgePerm();
        }
    }
}

void Cube::buildUDSlicePermMovesTable()
{
    for (uint32_t edgePerm = 0; edgePerm < UDSLICE_PERM_COUNT; ++edgePerm)
    {
        for (const auto& [moveIndex, moveFunction] : _moveFunctionsPhase2)
        {
            setUDSlicePermFromIndex(edgePerm);
            moveFunction();
            _udSlicePermMovesTable[edgePerm][moveIndex] = getUDSlicePerm();
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

void Cube::buildCornerSlicePermPtb()
{
    constexpr uint8_t notSetValue = std::numeric_limits<uint8_t>::max();
    resetCubeToSolved();
    uint32_t initialCornerPerm = getCornerPerm();
    uint32_t initialUDSlice = getUDSlicePerm();

    _cornerSlicePermPtb.resize(CORNER_PERM_COUNT);
    for (auto& row : _cornerSlicePermPtb)
    {
        row.resize(UDSLICE_COUNT);
        std::fill(row.begin(), row.end(), notSetValue);
    }

    _cornerSlicePermPtb[initialCornerPerm][initialUDSlice] = 0; // The solved state has a distance of 0
    std::queue<std::pair<uint32_t, uint32_t>> nextStates;
    nextStates.push({initialCornerPerm, initialUDSlice});
    while (!nextStates.empty())
    {
        auto [currentCornerPerm, currentUDSlice] = nextStates.front();
        nextStates.pop();
        for (const auto& [move, _] : _moveFunctionsPhase2)
        {
            uint32_t nextCornerPerm = _cornerPermMovesTable[currentCornerPerm][move];
            uint32_t nextUDSlice = _udSlicePermMovesTable[currentUDSlice][move];
            if (_cornerSlicePermPtb[nextCornerPerm][nextUDSlice] == notSetValue)
            {
                _cornerSlicePermPtb[nextCornerPerm][nextUDSlice] = _cornerSlicePermPtb[currentCornerPerm][currentUDSlice] + 1;
                nextStates.push({nextCornerPerm, nextUDSlice});
            }
        }
    }
}

void Cube::buildEdgeTopBottomSlicePermPtb()
{
    constexpr uint8_t notSetValue = std::numeric_limits<uint8_t>::max();
    resetCubeToSolved();
    uint32_t initialEdgePerm = getTopBottomEdgePerm();
    uint32_t initialUDSlice = getUDSlicePerm();

    _edgeTopBottomSlicePermPtb.resize(EDGE_PERM_COUNT);
    for (auto& row : _edgeTopBottomSlicePermPtb)
    {
        row.resize(UDSLICE_COUNT);
        std::fill(row.begin(), row.end(), notSetValue);
    }

    _edgeTopBottomSlicePermPtb[initialEdgePerm][initialUDSlice] = 0; // The solved state has a distance of 0
    std::queue<std::pair<uint32_t, uint32_t>> nextStates;
    nextStates.push({initialEdgePerm, initialUDSlice});
    while (!nextStates.empty())
    {
        auto [currentEdgePerm, currentUDSlice] = nextStates.front();
        nextStates.pop();
        for (const auto& [move, _] : _moveFunctionsPhase2)
        {
            uint32_t nextEdgePerm = _edgeTopBottomPermMovesTable[currentEdgePerm][move];
            uint32_t nextUDSlice = _udSlicePermMovesTable[currentUDSlice][move];
            if (_edgeTopBottomSlicePermPtb[nextEdgePerm][nextUDSlice] == notSetValue)
            {
                _edgeTopBottomSlicePermPtb[nextEdgePerm][nextUDSlice] =
                    _edgeTopBottomSlicePermPtb[currentEdgePerm][currentUDSlice] + 1;
                nextStates.push({nextEdgePerm, nextUDSlice});
            }
        }
    }
}

std::vector<Cube::Move> Cube::findMovesToG1State()
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
            return resultMovesSequence;
        }
        currentLimit = result;
    }
}

std::pair<uint8_t, std::vector<Cube::Move>> Cube::searchStatesToGetToG1State(uint32_t currentTwist,
                                                                             uint32_t currentFlip,
                                                                             uint32_t currentUDSlice,
                                                                             uint8_t depth,
                                                                             uint8_t currentLimit,
                                                                             std::vector<Move> movesSequence) const
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

std::vector<Cube::Move> Cube::findMovesFromG1ToSolvedState()
{
    uint32_t currentCornerPerm = getCornerPerm();
    uint32_t currentTopBottomEdgePerm = getTopBottomEdgePerm();
    uint32_t currentUDSlicePerm = getUDSlicePerm();
    uint8_t currentLimit = std::max(_cornerSlicePermPtb[currentCornerPerm][currentUDSlicePerm],
                                    _edgeTopBottomSlicePermPtb[currentTopBottomEdgePerm][currentUDSlicePerm]);
    uint8_t depth = 0;
    std::vector<Move> movesSequence;
    while (true)
    {
        auto [result, resultMovesSequence] = searchStatesToGetToSolvedState(
            currentCornerPerm, currentTopBottomEdgePerm, currentUDSlicePerm, depth, currentLimit, movesSequence);
        if (result == 0)
        {
            return resultMovesSequence;
        }
        currentLimit = result;
    }
}

std::pair<uint8_t, std::vector<Cube::Move>> Cube::searchStatesToGetToSolvedState(uint32_t currentCornerPerm,
                                                                                 uint32_t currentTopBottomEdgePerm,
                                                                                 uint32_t currentUDSlicePerm,
                                                                                 uint8_t depth,
                                                                                 uint8_t currentLimit,
                                                                                 std::vector<Move> movesSequence)
{
    uint8_t value = std::max(_cornerSlicePermPtb[currentCornerPerm][currentUDSlicePerm],
                             _edgeTopBottomSlicePermPtb[currentTopBottomEdgePerm][currentUDSlicePerm]);
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
    uint32_t nextCornerPerm;
    uint32_t nextTopBottomEdgePerm;
    uint32_t nextUDSlicePerm;

    std::vector<Move> movesU{Move::U, Move::UPrime, Move::DoubleU};
    std::vector<Move> movesD{Move::D, Move::DPrime, Move::DoubleD};
    for (const auto& [move, _] : _moveFunctionsPhase2)
    {
        if (!movesSequence.empty())
        {
            if (movesSequence.back() == move)
            {
                continue; // Skip the same move as the last one
            }
            if (std::find(movesU.begin(), movesU.end(), movesSequence.back()) != movesU.end() &&
                std::find(movesU.begin(), movesU.end(), move) != movesU.end())
            {
                continue; // Skip if the last move and the current move are both U moves
            }
            if (std::find(movesD.begin(), movesD.end(), movesSequence.back()) != movesD.end() &&
                std::find(movesD.begin(), movesD.end(), move) != movesD.end())
            {
                continue; // Skip if the last move and the current move are both D moves
            }
        }

        nextCornerPerm = _cornerPermMovesTable[currentCornerPerm][move];
        nextTopBottomEdgePerm = _edgeTopBottomPermMovesTable[currentTopBottomEdgePerm][move];
        nextUDSlicePerm = _udSlicePermMovesTable[currentUDSlicePerm][move];

        movesSequence.push_back(move);
        auto [tmpResult, tmpMovesSequence] = searchStatesToGetToSolvedState(
            nextCornerPerm, nextTopBottomEdgePerm, nextUDSlicePerm, depth + 1, currentLimit, movesSequence);
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
