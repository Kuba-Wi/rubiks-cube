#pragma once

#include "Cube.h"

#include <map>
#include <variant>
#include <vector>

/*!
 * Represents the position of the bottom servo controlling the cube's rotation looking from the top.
 */
enum class BottomServoPosition
{
    CounterClockwise,
    Center,
    Clockwise
};

enum class TopServoPosition
{
    Lift, // position for lifting the cube
    Up,
    Down
};

/*!
 * The ServoPositionsPlanner class is responsible for planning the sequence of servo positions required to execute a given
 * sequence of cube moves.
 */
class ServoPositionsPlanner
{
    /*!
     * Represents the face of the cube (associated with a color) or the direction the face is currently facing based on the
     * cube's orientation.
     */
    enum class Direction
    {
        Up,
        Down,
        Left,
        Right,
        Front,
        Back
    };

public:
    using ServoPosition = std::variant<BottomServoPosition, TopServoPosition>;
    using ServoPositionsSequence = std::vector<std::pair<Cube::Move, std::vector<ServoPosition>>>;

    ServoPositionsPlanner();
    ServoPositionsSequence planServoPositionsSequence(
        const std::vector<Cube::Move>& movesSequence);

    static BottomServoPosition defaultBottomServoPosition()
    {
        return BottomServoPosition::Center;
    }

    static TopServoPosition defaultTopServoPosition()
    {
        return TopServoPosition::Up;
    }

private:
    void reset();
    void planServoPositionsForMove(Cube::Move move);
    void rotateFaceClockwise(Direction face);
    void rotateFaceCounterClockwise(Direction face);
    void rotateFaceTwice(Direction face);

    // sets face to the position in which it can be rotated by bottom servo
    void setFaceToBottomPosition(Direction face);

    bool rotateCubeClockwise();
    bool rotateCubeCounterClockwise();
    bool rotateCubeTwice();

    // rotates the cube vertically, so that front face becomes the up face and so on
    void rotateCubeVertically();

    template <typename ServoPos>
    void addServoPosToSequence(ServoPos servoPos)
    {
        _servoPositionsSequence.back().second.push_back(servoPos);
    }

    /*!
     * Maps cube faces to their current orientation based on the cube's position.
     * For example, if the cube is rotated such that the Up face is now facing the front, the mapping would be: Up -> Front.
     * Key: Current orientation of a face (which direction it is facing), Value: Face of the cube (associated with a
     * specific color).
     */
    std::map<Direction, Direction> _cubeOrientation;

    BottomServoPosition _bottomServoPosition;
    TopServoPosition _topServoPosition;

    /*!
     * Sequence of servo positions updated when each rotation command is executed.
     * Each element in the vector represents a specific position for either the bottom or top servo.
     */
    ServoPositionsSequence _servoPositionsSequence;
};
