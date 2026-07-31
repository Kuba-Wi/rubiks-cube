#include "Cube.h"

int main()
{
    Cube cube;

    cube.moveUPrime();
    cube.moveRPrime();
    cube.moveDPrime();
    cube.moveLPrime();
    cube.moveFPrime();
    cube.moveBPrime();

    cube.getUDSlice();

    cube.buildFlipMovesTable();
    cube.buildTwistMovesTable();
    cube.buildUDSliceMovesTable();

    cube.printCube();

    cube.buildTwistSlicePtb();
    cube.buildFlipSlicePtb();

    return 0;
}
