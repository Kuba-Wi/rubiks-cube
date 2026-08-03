#include "Cube.h"

int main()
{
    Cube cube;

    cube.moveLPrime();
    cube.move2U();
    cube.moveB();
    cube.move2D();
    cube.moveUPrime();
    cube.moveFPrime();
    cube.move2R();
    cube.move2D();
    cube.move2B();
    cube.moveL();
    cube.moveRPrime();
    cube.moveUPrime();
    cube.move2B();
    cube.moveLPrime();
    cube.move2D();
    cube.move2B();
    cube.moveRPrime();
    cube.moveBPrime();
    cube.moveL();
    cube.moveD();
    cube.moveB();
    cube.move2U();
    cube.moveBPrime();
    cube.moveR();
    cube.move2L();

    cube.printCube();

    cube.findMovesToG1State();
    cube.printCube();

    return 0;
}
