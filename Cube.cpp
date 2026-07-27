#include "Cube.h"

#include <iostream>
#include <numeric>

Cube::Cube()
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
