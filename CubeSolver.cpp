#include "CubeSolver.h"

CubeSolver::CubeSolver(std::unique_ptr<ServoController>&& servoController)
    : _servoController(std::move(servoController))
{
}

void CubeSolver::solveCube(Cube& cube)
{
    std::vector<Cube::Move> movesSequence = cube.solveCube();
    _servoController->controlServos(movesSequence);
}
