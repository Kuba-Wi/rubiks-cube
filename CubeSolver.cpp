#include "CubeSolver.h"

#include "Logger.h"

#include <iostream>
#include <poll.h>
#include <thread>

CubeSolver::CubeSolver(std::unique_ptr<ServoController>&& servoController) :
    _servoController(std::move(servoController))
{
}

void CubeSolver::solveCube(Cube& cube)
{
    std::vector<Cube::Move> movesSequence = cube.solveCube();

    if (!_servoController->initServos())
    {
        LOG_ERROR("Failed to initialize servos.");
        return;
    }

    LOG_INFO("Put cube in the initial position and press Enter to start solving...");
    std::cin.get();

    auto pauseThread = std::thread(&CubeSolver::pauseFunction, this);
    _servoController->controlServos(movesSequence);
    _finishPauseThread = true;
    pauseThread.join();
}

void CubeSolver::pauseFunction()
{
    LOG_INFO("Press Enter to pause/resume solving...");
    bool isPaused = false;

    pollfd pfd{};
    pfd.fd = STDIN_FILENO;
    pfd.events = POLLIN;

    while (!_finishPauseThread)
    {
        int result = poll(&pfd, 1, 100);
        if (result == 0)
        {
            continue; // Timeout, no input
        }
        else if (result > 0 && (pfd.revents & POLLIN))
        {
            char c;
            [[maybe_unused]] ssize_t bytesRead = read(STDIN_FILENO, &c, 1);
        }

        isPaused = !isPaused;
        if (isPaused)
        {
            LOG_INFO("Solving paused. Press Enter to resume...");
            _servoController->pauseControl();
        }
        else
        {
            LOG_INFO("Solving resumed.");
            _servoController->resumeControl();
        }
    }
}
