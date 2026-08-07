#include "Cube.h"
#include "ImagesManager.h"

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
    cube.solveCube();
    cube.printCube();

    ImagesManager imagesManager("../images");
    imagesManager.loadCubeColorsData();

    return 0;
}
