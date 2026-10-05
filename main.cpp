#include "Cube.h"
#include "CubeSolver.h"
#include "ImagesManager.h"
#include "ImagesReader.h"
#include "ImagesStreamReceiver.h"
#include "ServoControllerTcp.h"

#include <iostream>

int main(int argc, char* argv[])
{
    std::string imagesDirPath;
    std::string serverIp;
    if (argc > 1)
    {
        imagesDirPath = argv[1];
    }
    if (argc > 2)
    {
        serverIp = argv[2];
    }

    Cube cube;
    CubeColorsData cubeColorsData;

    if (!imagesDirPath.empty())
    {
        ImagesManager imagesManager;
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
    std::unique_ptr<ServoController> servoController;
    if (!serverIp.empty())
    {
        servoController = std::make_unique<ServoControllerTcp>(serverIp);
    }
    else
    {
        servoController = std::make_unique<ServoControllerTcp>();
    }
    CubeSolver cubeSolver(std::move(servoController));
    cubeSolver.solveCube(cube);
    std::cout << "\nCube state after solving:\n";
    cube.printCube();

    return 0;
}
