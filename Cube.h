#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <limits>
#include <map>
#include <string>

class Cube
{
    enum Corner : uint8_t
    {
        UFL = 0,
        UFR,
        UBR,
        UBL,
        DFL,
        DFR,
        DBR,
        DRL,
        CornersCount
    };

    enum Edge : uint8_t
    {
        UF = 0,
        UR,
        UB,
        UL,
        FL, // member of the UD slice
        FR, // member of the UD slice
        BR, // member of the UD slice
        BL, // member of the UD slice
        FD,
        RD,
        BD,
        LD,
        EdgesCount,
        EdgesCountPhase2 = 8, // edges considered in phase 2 of the solving algorithm (UF, UR, UB, UL, FD, RD, BD, LD)
        EdgesCountUDSlice = 4 // edges considered in the UD slice (FL, FR, BR, BL)
    };

    enum Move : size_t
    {
        U = 0,
        UPrime,
        DoubleU,
        D,
        DPrime,
        DoubleD,
        F,
        FPrime,
        DoubleF,
        B,
        BPrime,
        DoubleB,
        R,
        RPrime,
        DoubleR,
        L,
        LPrime,
        DoubleL,
        MovesCount,
        MovesCountPhase2 = 10 // moves that are allowed in phase 2 of the solving algorithm (U, U', U2, D, D', D2, F2, B2,
                              // R2, L2)
    };

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

    uint32_t getCornerPerm() const;
    void setCornerPermFromIndex(uint32_t index);

    uint32_t getTopBottomEdgePerm() const;
    void setTopBottomEdgePermFromIndex(uint32_t index);

    uint32_t getUDSlicePerm() const;
    void setUDSlicePermFromIndex(uint32_t index);

    void buildTwistMovesTable();
    void buildFlipMovesTable();
    void buildUDSliceMovesTable();
    void buildCornerPermMovesTable();
    void buildTopBottomEdgePermMovesTable();
    void buildUDSlicePermMovesTable();

    void buildTwistSlicePtb();
    void buildFlipSlicePtb();
    void buildCornerPermPtb();
    void buildEdgeTopBottomPermPtb();
    void buildUDSlicePermPtb();

    /*!
     * Finds a sequence of moves that brings the cube to a G1 state.
     * The G1 state is defined as a state where the cube's corners and edges are correctly oriented and the UD slice edges
     * are all in the UD slice but in any order.
     * For now it prints the sequence of moves to the console and performs the moves on the cube.
     */
    void findMovesToG1State();

    void printCube() const;

    static constexpr size_t SLICE_EDGE_COUNT = 4;      // FL, FR, BL, BR
    static constexpr size_t TWIST_COUNT = 2187;        // 3^7
    static constexpr size_t FLIP_COUNT = 2048;         // 2^11
    static constexpr size_t UDSLICE_COUNT = 495;       // C(12, 4)
    static constexpr size_t CORNER_PERM_COUNT = 40320; // 8!
    static constexpr size_t EDGE_PERM_COUNT = 40320;   // 8! - it only concerns the edges from top and bottom layers
    static constexpr size_t UDSLICE_PERM_COUNT = 24;   // 4!

private:
    /*!
     * Searches for a sequence of moves that brings the cube to a G1 state.
     * Returns a pair consisting of the distance of state A from the G1 state and the corresponding moves sequence to reach
     * state A. It returns the smallest possible distance and the corresponding moves sequence among all possible moves from
     * the current state with a given search depth limitation and currentLimit (the maximum distance of state A from the G1
     * state).
     */
    std::pair<uint8_t, std::vector<Move>> searchStatesToGetToG1State(uint32_t currentTwist,
                                                                     uint32_t currentFlip,
                                                                     uint32_t currentUDSlice,
                                                                     uint8_t depth,
                                                                     uint8_t currentLimit,
                                                                     std::vector<Move> movesSequence);

    std::string moveToString(Move move) const;

    // Move functions mapped by their corresponding move index
    std::map<Move, std::function<void()>> _moveFunctions;
    // Move functions mapped by their corresponding move index for phase 2 of the solving algorithm
    std::map<Move, std::function<void()>> _moveFunctionsPhase2;

    std::array<uint8_t, Corner::CornersCount> _cornerPerm;

    /*!
     * Corner orientation: 0 = correct, 1 = clockwise, 2 = counterclockwise
     * Moves U, D, U' and D' do not twist corner orientation.
     * _cornerOrient[0] tells orientation of a corner that is currently in position 0 (UFL), etc.
     * Corner is in correct orientation if the U or D color is on the U or D face (U color on D face is correct).
     */
    std::array<uint8_t, Corner::CornersCount> _cornerOrient;

    std::array<uint8_t, Edge::EdgesCount> _edgePerm;

    /*!
     * Edge orientation: 0 = correct, 1 = flipped
     * Only moves F, B, F', B' flip edge orientation.
     * _edgeOrient[0] tells orientation of an edge that is currently in position 0 (UF), etc.
     */
    std::array<uint8_t, Edge::EdgesCount> _edgeOrient;

    /*!
     * Precomputed values for C(n, k) = n! / (k! * (n - k)!)
     * It is needed for calculating the UD slice permutation.
     */
    std::array<std::array<uint32_t, SLICE_EDGE_COUNT + 1>, Edge::EdgesCount> _CValues;

    /*!
     * Precomputed moves for each twist of the cube.
     * _twistMovesTable[0][0] says what will be a new twist of a cube after applying move 0 (U) to a cube with twist 0.
     */
    std::array<std::array<uint32_t, Move::MovesCount>, TWIST_COUNT> _twistMovesTable;
    std::array<std::array<uint32_t, Move::MovesCount>, FLIP_COUNT> _flipMovesTable;
    std::array<std::array<uint32_t, Move::MovesCount>, UDSLICE_COUNT> _udSliceMovesTable;
    std::array<std::map<Move, uint32_t>, CORNER_PERM_COUNT> _cornerPermMovesTable;
    std::array<std::map<Move, uint32_t>, EDGE_PERM_COUNT> _edgeTopBottomPermMovesTable;
    std::array<std::map<Move, uint32_t>, UDSLICE_PERM_COUNT> _udSlicePermMovesTable;

    /*!
     * Precomputed table for pruning the twist of the cube
     * _twistSlicePtb[twist][udSlice] gives the distance from the solved cube to a cube with a given twist and UD slice.
     */
    std::vector<std::vector<uint8_t>> _twistSlicePtb;
    std::vector<std::vector<uint8_t>> _flipSlicePtb;
    std::vector<uint8_t> _cornerPermPtb;
    std::vector<uint8_t> _edgeTopBottomPermPtb;
    std::vector<uint8_t> _udSlicePermPtb;
};
