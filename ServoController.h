#pragma once

#include "Cube.h"

class ServoController
{
public:
    virtual ~ServoController() = default;
    virtual void controlServos(const std::vector<Cube::Move>& moves) = 0;
};
