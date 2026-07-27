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

    cube.printCube();
    cube.getUDSlice();

    return 0;
}
