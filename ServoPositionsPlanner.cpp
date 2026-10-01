#include "ServoPositionsPlanner.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <unordered_map>

ServoPositionsPlanner::ServoPositionsPlanner()
{
    reset();
}

void ServoPositionsPlanner::reset()
{
    _bottomServoPosition = BottomServoPosition::Center;
    _topServoPosition = TopServoPosition::Up;
    _cubeOrientation = {
        {Direction::Up,    Direction::Up   },
        {Direction::Down,  Direction::Down },
        {Direction::Left,  Direction::Left },
        {Direction::Right, Direction::Right},
        {Direction::Front, Direction::Front},
        {Direction::Back,  Direction::Back }
    };
    _servoPositionsSequence.clear();
}

ServoPositionsPlanner::ServoPositionsSequence ServoPositionsPlanner::planServoPositionsSequence(
    const std::vector<Cube::Move>& movesSequence)
{
    reset();
    for (const auto& move : movesSequence)
    {
        planServoPositionsForMove(move);
    }
    return _servoPositionsSequence;
}

void ServoPositionsPlanner::planServoPositionsForMove(Cube::Move move)
{
    _servoPositionsSequence.push_back({move, {}});

    // clang-format off
    static std::unordered_map<Cube::Move, std::function<void()>> moveFunctions = {
        {Cube::Move::U,       [this]() { rotateFaceClockwise(Direction::Up); }},
        {Cube::Move::UPrime,  [this]() { rotateFaceCounterClockwise(Direction::Up); }},
        {Cube::Move::DoubleU, [this]() { rotateFaceTwice(Direction::Up); }},
        {Cube::Move::D,       [this]() { rotateFaceClockwise(Direction::Down); }},
        {Cube::Move::DPrime,  [this]() { rotateFaceCounterClockwise(Direction::Down); }},
        {Cube::Move::DoubleD, [this]() { rotateFaceTwice(Direction::Down); }},
        {Cube::Move::F,       [this]() { rotateFaceClockwise(Direction::Front); }},
        {Cube::Move::FPrime,  [this]() { rotateFaceCounterClockwise(Direction::Front); }},
        {Cube::Move::DoubleF, [this]() { rotateFaceTwice(Direction::Front); }},
        {Cube::Move::B,       [this]() { rotateFaceClockwise(Direction::Back); }},
        {Cube::Move::BPrime,  [this]() { rotateFaceCounterClockwise(Direction::Back); }},
        {Cube::Move::DoubleB, [this]() { rotateFaceTwice(Direction::Back); }},
        {Cube::Move::R,       [this]() { rotateFaceClockwise(Direction::Right); }},
        {Cube::Move::RPrime,  [this]() { rotateFaceCounterClockwise(Direction::Right); }},
        {Cube::Move::DoubleR, [this]() { rotateFaceTwice(Direction::Right); }},
        {Cube::Move::L,       [this]() { rotateFaceClockwise(Direction::Left); }},
        {Cube::Move::LPrime,  [this]() { rotateFaceCounterClockwise(Direction::Left); }},
        {Cube::Move::DoubleL, [this]() { rotateFaceTwice(Direction::Left); }}
    };
    // clang-format on

    moveFunctions[move]();
}

void ServoPositionsPlanner::rotateFaceClockwise(Direction face)
{
    setFaceToBottomPosition(face);
    if (_bottomServoPosition == BottomServoPosition::CounterClockwise)
    {
        rotateCubeClockwise();
    }
    addServoPosToSequence(TopServoPosition::Down);
    _topServoPosition = TopServoPosition::Down;
    rotateCubeCounterClockwise();
    addServoPosToSequence(TopServoPosition::Up);
    _topServoPosition = TopServoPosition::Up;
}

void ServoPositionsPlanner::rotateFaceCounterClockwise(Direction face)
{
    setFaceToBottomPosition(face);
    if (_bottomServoPosition == BottomServoPosition::Clockwise)
    {
        rotateCubeCounterClockwise();
    }
    addServoPosToSequence(TopServoPosition::Down);
    _topServoPosition = TopServoPosition::Down;
    rotateCubeClockwise();
    addServoPosToSequence(TopServoPosition::Up);
    _topServoPosition = TopServoPosition::Up;
}

void ServoPositionsPlanner::rotateFaceTwice(Direction face)
{
    setFaceToBottomPosition(face);
    if (_bottomServoPosition == BottomServoPosition::Center)
    {
        rotateCubeClockwise();
    }
    addServoPosToSequence(TopServoPosition::Down);
    _topServoPosition = TopServoPosition::Down;
    rotateCubeTwice();
    addServoPosToSequence(TopServoPosition::Up);
    _topServoPosition = TopServoPosition::Up;
}

void ServoPositionsPlanner::setFaceToBottomPosition(Direction face)
{
    const auto positionIt = std::find_if(_cubeOrientation.begin(),
                                         _cubeOrientation.end(),
                                         [face](const auto& pair)
                                         {
                                             return pair.second == face;
                                         });

    assert(positionIt != _cubeOrientation.end() && "Face not found in cube orientation mapping");

    switch (positionIt->first)
    {
        case Direction::Up:
        {
            rotateCubeVertically();
            rotateCubeVertically();
            break;
        }
        case Direction::Down:
        {
            break;
        }
        case Direction::Left:
        {
            if (rotateCubeClockwise())
            {
                rotateCubeVertically();
            }
            else
            {
                rotateCubeCounterClockwise();
                rotateCubeVertically();
                rotateCubeVertically();
                rotateCubeVertically();
            }
            break;
        }
        case Direction::Right:
        {
            if (rotateCubeCounterClockwise())
            {
                rotateCubeVertically();
            }
            else
            {
                rotateCubeClockwise();
                rotateCubeVertically();
                rotateCubeVertically();
                rotateCubeVertically();
            }
            break;
        }
        case Direction::Front:
        {
            rotateCubeVertically();
            rotateCubeVertically();
            rotateCubeVertically();
            break;
        }
        case Direction::Back:
        {
            rotateCubeVertically();
            break;
        }
    }
}

bool ServoPositionsPlanner::rotateCubeClockwise()
{
    switch (_bottomServoPosition)
    {
        case BottomServoPosition::Clockwise:
        {
            return false; // Already in clockwise position, cannot rotate further
        }
        case BottomServoPosition::Center:
        {
            _bottomServoPosition = BottomServoPosition::Clockwise;
            addServoPosToSequence(BottomServoPosition::Clockwise);
            break;
        }
        case BottomServoPosition::CounterClockwise:
        {
            _bottomServoPosition = BottomServoPosition::Center;
            addServoPosToSequence(BottomServoPosition::Center);
            break;
        }
    }

    if (_topServoPosition == TopServoPosition::Up)
    {
        const auto frontOrientation = _cubeOrientation[Direction::Front];
        _cubeOrientation[Direction::Front] = _cubeOrientation[Direction::Right];
        _cubeOrientation[Direction::Right] = _cubeOrientation[Direction::Back];
        _cubeOrientation[Direction::Back] = _cubeOrientation[Direction::Left];
        _cubeOrientation[Direction::Left] = frontOrientation;
    }
    return true;
}

bool ServoPositionsPlanner::rotateCubeCounterClockwise()
{
    switch (_bottomServoPosition)
    {
        case BottomServoPosition::CounterClockwise:
        {
            return false; // Already in counter-clockwise position, cannot rotate further
        }
        case BottomServoPosition::Center:
        {
            _bottomServoPosition = BottomServoPosition::CounterClockwise;
            addServoPosToSequence(BottomServoPosition::CounterClockwise);
            break;
        }
        case BottomServoPosition::Clockwise:
        {
            _bottomServoPosition = BottomServoPosition::Center;
            addServoPosToSequence(BottomServoPosition::Center);
            break;
        }
    }

    if (_topServoPosition == TopServoPosition::Up)
    {
        const auto frontOrientation = _cubeOrientation[Direction::Front];
        _cubeOrientation[Direction::Front] = _cubeOrientation[Direction::Left];
        _cubeOrientation[Direction::Left] = _cubeOrientation[Direction::Back];
        _cubeOrientation[Direction::Back] = _cubeOrientation[Direction::Right];
        _cubeOrientation[Direction::Right] = frontOrientation;
    }
    return true;
}

bool ServoPositionsPlanner::rotateCubeTwice()
{
    switch (_bottomServoPosition)
    {
        case BottomServoPosition::Center:
        {
            return false; // Can't rotate twice from center position, must rotate to either clockwise or counter-clockwise
                          // first
        }
        case BottomServoPosition::Clockwise:
        {
            _bottomServoPosition = BottomServoPosition::CounterClockwise;
            addServoPosToSequence(BottomServoPosition::CounterClockwise);
            break;
        }
        case BottomServoPosition::CounterClockwise:
        {
            _bottomServoPosition = BottomServoPosition::Clockwise;
            addServoPosToSequence(BottomServoPosition::Clockwise);
            break;
        }
    }

    if (_topServoPosition == TopServoPosition::Up)
    {
        std::swap(_cubeOrientation[Direction::Front], _cubeOrientation[Direction::Back]);
        std::swap(_cubeOrientation[Direction::Left], _cubeOrientation[Direction::Right]);
    }
    return true;
}

void ServoPositionsPlanner::rotateCubeVertically()
{
    const auto frontOrientation = _cubeOrientation[Direction::Front];
    _cubeOrientation[Direction::Front] = _cubeOrientation[Direction::Down];
    _cubeOrientation[Direction::Down] = _cubeOrientation[Direction::Back];
    _cubeOrientation[Direction::Back] = _cubeOrientation[Direction::Up];
    _cubeOrientation[Direction::Up] = frontOrientation;

    addServoPosToSequence(TopServoPosition::Lift);
    addServoPosToSequence(TopServoPosition::Up);
}
