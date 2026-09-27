#pragma once

#include "Cube.h"
#include "ServoController.h"

#include <memory>

class CubeSolver
{
public:
    CubeSolver(std::unique_ptr<ServoController>&& servoController);
    void solveCube(Cube& cube);

private:
    std::unique_ptr<ServoController> _servoController;
};
