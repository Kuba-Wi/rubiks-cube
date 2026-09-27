#include "ServoPositionsPlanner.h"

#include <algorithm>
#include <cassert>

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

std::vector<std::variant<BottomServoPosition, TopServoPosition>> ServoPositionsPlanner::planServoPositionsSequence(
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
    switch (move)
    {
        case Cube::Move::U:
            rotateFaceClockwise(Direction::Up);
            break;
        case Cube::Move::UPrime:
            rotateFaceCounterClockwise(Direction::Up);
            break;
        case Cube::Move::DoubleU:
            rotateFaceTwice(Direction::Up);
            break;
        case Cube::Move::D:
            rotateFaceClockwise(Direction::Down);
            break;
        case Cube::Move::DPrime:
            rotateFaceCounterClockwise(Direction::Down);
            break;
        case Cube::Move::DoubleD:
            rotateFaceTwice(Direction::Down);
            break;
        case Cube::Move::F:
            rotateFaceClockwise(Direction::Front);
            break;
        case Cube::Move::FPrime:
            rotateFaceCounterClockwise(Direction::Front);
            break;
        case Cube::Move::DoubleF:
            rotateFaceTwice(Direction::Front);
            break;
        case Cube::Move::B:
            rotateFaceClockwise(Direction::Back);
            break;
        case Cube::Move::BPrime:
            rotateFaceCounterClockwise(Direction::Back);
            break;
        case Cube::Move::DoubleB:
            rotateFaceTwice(Direction::Back);
            break;
        case Cube::Move::R:
            rotateFaceClockwise(Direction::Right);
            break;
        case Cube::Move::RPrime:
            rotateFaceCounterClockwise(Direction::Right);
            break;
        case Cube::Move::DoubleR:
            rotateFaceTwice(Direction::Right);
            break;
        case Cube::Move::L:
            rotateFaceClockwise(Direction::Left);
            break;
        case Cube::Move::LPrime:
            rotateFaceCounterClockwise(Direction::Left);
            break;
        case Cube::Move::DoubleL:
            rotateFaceTwice(Direction::Left);
            break;
        default:
            break;
    }
}

void ServoPositionsPlanner::rotateFaceClockwise(Direction face)
{
    setFaceToBottomPosition(face);
    if (_bottomServoPosition == BottomServoPosition::Clockwise)
    {
        rotateCubeCounterClockwise();
    }
    _servoPositionsSequence.push_back(TopServoPosition::Down);
    rotateCubeClockwise();
    _servoPositionsSequence.push_back(TopServoPosition::Up);
}

void ServoPositionsPlanner::rotateFaceCounterClockwise(Direction face)
{
    setFaceToBottomPosition(face);
    if (_bottomServoPosition == BottomServoPosition::CounterClockwise)
    {
        rotateCubeClockwise();
    }
    _servoPositionsSequence.push_back(TopServoPosition::Down);
    rotateCubeCounterClockwise();
    _servoPositionsSequence.push_back(TopServoPosition::Up);
}

void ServoPositionsPlanner::rotateFaceTwice(Direction face)
{
    setFaceToBottomPosition(face);
    if (_bottomServoPosition == BottomServoPosition::Center)
    {
        rotateCubeClockwise();
    }
    _servoPositionsSequence.push_back(TopServoPosition::Down);
    rotateCubeTwice();
    _servoPositionsSequence.push_back(TopServoPosition::Up);
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
            _servoPositionsSequence.push_back(BottomServoPosition::Clockwise);
            break;
        }
        case BottomServoPosition::CounterClockwise:
        {
            _bottomServoPosition = BottomServoPosition::Center;
            _servoPositionsSequence.push_back(BottomServoPosition::Center);
            break;
        }
    }

    const auto frontOrientation = _cubeOrientation[Direction::Front];
    _cubeOrientation[Direction::Front] = Direction::Right;
    _cubeOrientation[Direction::Right] = Direction::Back;
    _cubeOrientation[Direction::Back] = Direction::Left;
    _cubeOrientation[Direction::Left] = frontOrientation;
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
            _servoPositionsSequence.push_back(BottomServoPosition::CounterClockwise);
            break;
        }
        case BottomServoPosition::Clockwise:
        {
            _bottomServoPosition = BottomServoPosition::Center;
            _servoPositionsSequence.push_back(BottomServoPosition::Center);
            break;
        }
    }

    const auto frontOrientation = _cubeOrientation[Direction::Front];
    _cubeOrientation[Direction::Front] = Direction::Left;
    _cubeOrientation[Direction::Left] = Direction::Back;
    _cubeOrientation[Direction::Back] = Direction::Right;
    _cubeOrientation[Direction::Right] = frontOrientation;
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
            _servoPositionsSequence.push_back(BottomServoPosition::CounterClockwise);
            break;
        }
        case BottomServoPosition::CounterClockwise:
        {
            _bottomServoPosition = BottomServoPosition::Clockwise;
            _servoPositionsSequence.push_back(BottomServoPosition::Clockwise);
            break;
        }
    }

    std::swap(_cubeOrientation[Direction::Front], _cubeOrientation[Direction::Back]);
    std::swap(_cubeOrientation[Direction::Left], _cubeOrientation[Direction::Right]);
    return true;
}

void ServoPositionsPlanner::rotateCubeVertically()
{
    const auto frontOrientation = _cubeOrientation[Direction::Front];
    _cubeOrientation[Direction::Front] = _cubeOrientation[Direction::Down];
    _cubeOrientation[Direction::Down] = _cubeOrientation[Direction::Back];
    _cubeOrientation[Direction::Back] = _cubeOrientation[Direction::Up];
    _cubeOrientation[Direction::Up] = frontOrientation;

    _servoPositionsSequence.push_back(TopServoPosition::Lift);
    _servoPositionsSequence.push_back(TopServoPosition::Up);
}
