#include "ServoController.h"

#include "ServoPositionsPlanner.h"
#include "TcpSender.h"

#include <iostream>
#include <thread>

namespace
{
constexpr std::chrono::milliseconds defaultSleepTime(1000);
constexpr std::chrono::milliseconds longSleepTime(1500);

enum ServoIndex : uint8_t
{
    BottomServo = 0,
    TopServo = 1
};

std::vector<uint8_t> translateServoPositionToCommand(BottomServoPosition servoPosition)
{
    enum BottomServoAngle : uint8_t
    {
        ClockwiseAngle = 0,
        CenterAngle = 90,
        CounterClockwiseAngle = 180
    };

    std::vector<uint8_t> command;
    switch (servoPosition)
    {
        case BottomServoPosition::CounterClockwise:
            command = {BottomServo, CounterClockwiseAngle};
            break;
        case BottomServoPosition::Center:
            command = {BottomServo, CenterAngle};
            break;
        case BottomServoPosition::Clockwise:
            command = {BottomServo, ClockwiseAngle};
            break;
    }
    return command;
}

std::vector<uint8_t> translateServoPositionToCommand(TopServoPosition servoPosition)
{
    enum TopServoAngle : uint8_t
    {
        LiftAngle = 0,
        UpAngle = 58,
        DownAngle = 88
    };

    std::vector<uint8_t> command;
    switch (servoPosition)
    {
        case TopServoPosition::Lift:
            command = {TopServo, LiftAngle};
            break;
        case TopServoPosition::Up:
            command = {TopServo, UpAngle};
            break;
        case TopServoPosition::Down:
            command = {TopServo, DownAngle};
            break;
    }
    return command;
}
} // namespace

void ServoControllerTcp::controlServos(const std::vector<Cube::Move>& moves)
{
    TcpSender tcpSender;
    if (!tcpSender.connectToServer())
    {
        std::cerr << "Failed to connect to the server." << std::endl;
        return;
    }

    ServoPositionsPlanner servoPositionsPlanner;
    const auto servoPositionsSequence = servoPositionsPlanner.planServoPositionsSequence(moves);

    BottomServoPosition currentBottomServoPosition = ServoPositionsPlanner::defaultBottomServoPosition();
    TopServoPosition currentTopServoPosition = ServoPositionsPlanner::defaultTopServoPosition();
    std::chrono::milliseconds sleepDuration = defaultSleepTime;
    for (const auto& servoPosition : servoPositionsSequence)
    {
        if (std::holds_alternative<BottomServoPosition>(servoPosition))
        {
            const auto bottomServoPosition = std::get<BottomServoPosition>(servoPosition);
            if (bottomServoPosition != currentBottomServoPosition)
            {
                if (bottomServoPosition == BottomServoPosition::Center ||
                    currentBottomServoPosition == BottomServoPosition::Center)
                {
                    sleepDuration = defaultSleepTime;
                }
                else
                {
                    // If the bottom servo is moving from clockwise to counter-clockwise or vice versa, we need to wait
                    // longer for the servo to complete the rotation.
                    sleepDuration = longSleepTime;
                }
                currentBottomServoPosition = bottomServoPosition;
                tcpSender.sendData(translateServoPositionToCommand(bottomServoPosition));
                std::this_thread::sleep_for(sleepDuration);
            }
        }
        else if (std::holds_alternative<TopServoPosition>(servoPosition))
        {
            const auto topServoPosition = std::get<TopServoPosition>(servoPosition);
            if (topServoPosition != currentTopServoPosition)
            {
                currentTopServoPosition = topServoPosition;
                tcpSender.sendData(translateServoPositionToCommand(topServoPosition));
                std::this_thread::sleep_for(sleepDuration);
            }
        }
    }
}
