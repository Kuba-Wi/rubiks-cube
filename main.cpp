#include "Cube.h"
#include "ImagesManager.h"

#include <iostream>

int main(int argc, char* argv[])
{
    Cube cube;

    std::string imagesDirPath = "../images";
    if (argc > 1)
    {
        imagesDirPath = argv[1];
    }

    ImagesManager imagesManager(imagesDirPath);
    const bool cubeDataLoaded = imagesManager.loadCubeColorsData();
    if (!cubeDataLoaded)
    {
        return 1;
    }

    if (!cube.setCubeStateFromColorsData(imagesManager.getCubeColorsData()))
    {
        std::cerr << "Failed to set cube state from colors data." << std::endl;
        return 1;
    }
    std::cout << "\nInitial cube state:\n";
    cube.printCube();
    std::cout << "\nSolving the cube...\n";
    cube.solveCube();
    std::cout << "\nCube state after solving:\n";
    cube.printCube();

    return 0;
}
