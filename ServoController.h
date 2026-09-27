#pragma once

#include "Cube.h"

class ServoController
{
public:
    virtual ~ServoController() = default;
    virtual void controlServos(const std::vector<Cube::Move>& moves) = 0;
};

class ServoControllerTcp : public ServoController
{
public:
    ~ServoControllerTcp() override = default;
    void controlServos(const std::vector<Cube::Move>& moves) override;
};
