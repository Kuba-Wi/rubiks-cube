#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <limits>
#include <unordered_map>

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
 *
 * In moving tables, the order of moves is as follows:
 * 0: U,    6: F,   12: R,
 * 1: U',   7: F',  13: R',
 * 2: 2U,   8: 2F,  14: 2R,
 * 3: D,    9: B,   15: L,
 * 4: D',   10: B', 16: L',
 * 5: 2D,   11: 2B, 17: 2L
 */
class Cube
{
public:
    Cube();
    ~Cube() = default;
    Cube(const Cube& other) = delete;
    Cube& operator=(const Cube& other) = delete;

    void resetCubeToSolved();

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

    /*!
     * Returns a twist of the cube which is a number representing the orientation of the corners.
     * The twist is calculated as follows:
     * For each corner, the orientation is multiplied by 3 raised to the power of its index, and the results are summed up.
     * The twist is a number between 0 and 2186 (3^7 - 1), where 0 represents a solved cube and 2186 represents a cube with
     * all corners twisted counterclockwise.
     * Note: the last corner's orientation is determined by the first seven corners, so it is not included in the
     * calculation.
     */
    uint32_t getTwist() const;
    void setCornerOrientFromTwist(uint32_t twist);

    /*!
     * Returns a flip of the cube which is a number representing the orientation of the edges.
     * The flip is calculated as follows:
     * For each edge, the orientation is multiplied by 2 raised to the power of its index, and the results are summed up.
     * The flip is a number between 0 and 2047 (2^11 - 1), where 0 represents a solved cube and 2047 represents a cube with
     * all edges flipped.
     * Note: the last edge's orientation is determined by the first eleven edges, so it is not included in the calculation.
     */
    uint32_t getFlip() const;
    void setEdgeOrientFromFlip(uint32_t flip);

    /*!
     * Returns a number representing the position of the edges from the UD slice (the middle layer).
     * The UD slice consists of the edges FL, FR, BL, and BR.
     * The position is calculated as follows:
     * We find positions (n: range 0-11) of the edges (k: range 0-3) and calculate Newton symbol C(n, k)=n!/(k!*(n-k)!) for
     * each edge in the UD slice and sum them up to get the UD slice. Order of UD slice edges is not important.
     * The UDSlice is a number between 0 and 494, where 0 represents all edges put in the positions with the lowest indices
     * (UF, UR, UB, UL) and 494 represents all edges put in the positions with the highest indices (LD, BD, RD, FD).
     */
    uint32_t getUDSlice() const;

    /*!
     * Sets one of the possible edge positions of the cube based on a given UD slice.
     * Note: It sets only the positions of edges coming from the UD slice and sets other edges permutation to 0.
     */
    void setEdgePosFromUDSlice(uint32_t udSlice);

    void buildTwistMovesTable();
    void buildFlipMovesTable();
    void buildUDSliceMovesTable();

    void buildTwistPtb();
    void buildFlipPtb();
    void buildUDSlicePtb();

    void printCube() const;

    static constexpr size_t CORNER_COUNT = 8;
    static constexpr size_t EDGE_COUNT = 12;
    static constexpr size_t SLICE_EDGE_COUNT = 4; // FL, FR, BL, BR
    static constexpr size_t TWIST_COUNT = 2187;   // 3^7
    static constexpr size_t FLIP_COUNT = 2048;    // 2^11
    static constexpr size_t UDSLICE_COUNT = 495;  // C(12, 4)
    static constexpr size_t MOVES_COUNT = 18;     // U, U', 2U, D, D', 2D, F, F', 2F, B, B', 2B, R, R', 2R, L, L', 2L

private:
    template <typename TPtb, typename TMovesTable>
    void buildPtb(TPtb& ptb, const TMovesTable& movesTable, uint32_t initialSolvedState)
    {
        constexpr uint32_t notSetValue = std::numeric_limits<uint32_t>::max();
        std::fill(ptb.begin(), ptb.end(), notSetValue);
        uint32_t currentState = initialSolvedState;
        ptb[currentState] = 0; // The solved state has a distance of 0
        uint32_t nextState;
        std::vector<uint32_t> nextStates(1, currentState);
        while (!nextStates.empty())
        {
            currentState = nextStates.front();
            nextStates.erase(nextStates.begin());
            for (size_t move = 0; move < MOVES_COUNT; ++move)
            {
                nextState = movesTable[currentState][move];
                if (ptb[nextState] == notSetValue)
                {
                    ptb[nextState] = ptb[currentState] + 1;
                    nextStates.push_back(nextState);
                }
            }
        }
    }

    // Move functions mapped by their corresponding move index
    std::unordered_map<size_t, std::function<void()>> _moveFunctions;

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

    /*!
     * Precomputed values for C(n, k) = n! / (k! * (n - k)!)
     * It is needed for calculating the UD slice permutation.
     */
    std::array<std::array<uint32_t, SLICE_EDGE_COUNT + 1>, EDGE_COUNT> _CValues;

    /*!
     * Precomputed moves for each twist of the cube.
     * _twistMovesTable[0][0] says what will be a new twist of a cube after applying move 0 (U) to a cube with twist 0.
     */
    std::array<std::array<uint32_t, MOVES_COUNT>, TWIST_COUNT> _twistMovesTable;
    std::array<std::array<uint32_t, MOVES_COUNT>, FLIP_COUNT> _flipMovesTable;
    std::array<std::array<uint32_t, MOVES_COUNT>, UDSLICE_COUNT> _udSliceMovesTable;

    /*!
     * Precomputed table for pruning the twist of the cube
     * _twistPtb[twist] gives the distance from the solved cube to a cube with a given twist.
     */
    std::array<uint32_t, TWIST_COUNT> _twistPtb;
    std::array<uint32_t, FLIP_COUNT> _flipPtb;
    std::array<uint32_t, UDSLICE_COUNT> _udSlicePtb;
};
