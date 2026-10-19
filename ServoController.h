#pragma once

#include "Cube.h"

class ServoController
{
public:
    virtual ~ServoController() = default;
    virtual bool initServos() = 0;
    virtual void controlServos(const std::vector<Cube::Move>& moves) = 0;
    virtual void pauseControl() = 0;
    virtual void resumeControl() = 0;
};
