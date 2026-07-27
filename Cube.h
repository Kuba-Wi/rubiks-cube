#pragma once

#include <array>
#include <cstdint>

/*!
 * @brief Class representing a Rubik's Cube.
 *
 * Indexes of corners are as follows:
 * 0: UFL,      4: DFL,
 * 1: UFR,      5: DFR,
 * 2: UBR,      6: DBR,
 * 3: UBL,      7: DRL
 * Indexes of edges are as follows:
 * 0: UF,       6: BR,
 * 1: UR,       7: BL,
 * 2: UB,       8: FD,
 * 3: UL,       9: RD,
 * 4: FL,       10: BD,
 * 5: FR,       11: LD,
 */
class Cube
{
public:
    Cube();
    ~Cube() = default;
    Cube(const Cube& other) = delete;
    Cube& operator=(const Cube& other) = delete;

    void moveU();
    void moveUPrime();
    void move2U();

    void moveD();
    void moveDPrime();
    void move2D();

    void moveF();
    void moveFPrime();
    void move2F();

    void moveB();
    void moveBPrime();
    void move2B();

    void moveR();
    void moveRPrime();
    void move2R();

    void moveL();
    void moveLPrime();
    void move2L();

    void printCube() const;

    static constexpr uint8_t CORNER_COUNT = 8;
    static constexpr uint8_t EDGE_COUNT = 12;

private:
    std::array<uint8_t, CORNER_COUNT> _cornerPerm;

    /*!
     * Corner orientation: 0 = correct, 1 = clockwise, 2 = counterclockwise
     * Moves U, D, U' and D' do not twist corner orientation.
     * _cornerOrient[0] tells orientation of a corner that is currently in position 0 (UFL), etc.
     * Corner is in correct orientation if the U or D color is on the U or D face (U color on D face is correct).
     */
    std::array<uint8_t, CORNER_COUNT> _cornerOrient;

    std::array<uint8_t, EDGE_COUNT> _edgePerm;

    /*!
     * Edge orientation: 0 = correct, 1 = flipped
     * Only moves F, B, F', B' flip edge orientation.
     * _edgeOrient[0] tells orientation of an edge that is currently in position 0 (UF), etc.
     */
    std::array<uint8_t, EDGE_COUNT> _edgeOrient;
};
