#pragma once

#include "Cube.h"
#include "ServoController.h"

#include <atomic>
#include <memory>

class CubeSolver
{
public:
    CubeSolver(std::unique_ptr<ServoController>&& servoController);
    void solveCube(Cube& cube);

private:
    void pauseFunction();

    std::unique_ptr<ServoController> _servoController;
    std::atomic<bool> _finishPauseThread{false};
};
