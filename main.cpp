#include "Cube.h"
#include "ImagesManager.h"
#include "ImagesReader.h"
#include "ImagesStreamReceiver.h"

#include <iostream>

int main(int argc, char* argv[])
{
    Cube cube;
    CubeColorsData cubeColorsData;

    if (argc > 1)
    {
        ImagesManager imagesManager;
        std::string imagesDirPath = argv[1];
        const bool cubeDataLoaded = imagesManager.loadCubeColorsData(imagesDirPath);
        if (!cubeDataLoaded)
        {
            return 1;
        }
        cubeColorsData = imagesManager.getCubeColorsData();
    }
    else
    {
        ImagesReader imagesReader(std::make_unique<ImagesManager>(), std::make_unique<ImagesStreamReceiver>());
        imagesReader.readImages();
        imagesReader.printCubeColorsData();
        cubeColorsData = imagesReader.getCubeColorsData();
    }

    if (!cube.setCubeStateFromColorsData(cubeColorsData))
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
