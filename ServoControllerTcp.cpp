#include "ServoControllerTcp.h"

#include <iostream>
#include <thread>

namespace
{
constexpr std::chrono::milliseconds defaultSleepTime(1000);
constexpr std::chrono::milliseconds longSleepTime(2000);

enum ServoIndex : uint8_t
{
    BottomServo = 0,
    TopServo = 1
};

std::vector<uint8_t> translateServoPositionToCommand(BottomServoPosition servoPosition)
{
    std::vector<uint8_t> command;
    switch (servoPosition)
    {
        case BottomServoPosition::CounterClockwise:
            command = {BottomServo, ServoControllerTcp::CounterClockwiseAngle};
            break;
        case BottomServoPosition::Center:
            command = {BottomServo, ServoControllerTcp::CenterAngle};
            break;
        case BottomServoPosition::Clockwise:
            command = {BottomServo, ServoControllerTcp::ClockwiseAngle};
            break;
    }
    return command;
}

std::vector<uint8_t> translateServoPositionToCommand(TopServoPosition servoPosition)
{
    std::vector<uint8_t> command;
    switch (servoPosition)
    {
        case TopServoPosition::Lift:
            command = {TopServo, ServoControllerTcp::LiftAngle};
            break;
        case TopServoPosition::Up:
            command = {TopServo, ServoControllerTcp::UpAngle};
            break;
        case TopServoPosition::Down:
            command = {TopServo, ServoControllerTcp::DownAngle};
            break;
    }
    return command;
}
} // namespace

bool ServoControllerTcp::initServos()
{
    if (!_tcpSender.connectToServer())
    {
        std::cerr << "Failed to connect to the server." << std::endl;
        return false;
    }

    _currentBottomServoPosition = ServoPositionsPlanner::defaultBottomServoPosition();
    _tcpSender.sendData(translateServoPositionToCommand(_currentBottomServoPosition));
    std::this_thread::sleep_for(defaultSleepTime);

    _currentTopServoPosition = ServoPositionsPlanner::defaultTopServoPosition();
    _tcpSender.sendData(translateServoPositionToCommand(_currentTopServoPosition));
    std::this_thread::sleep_for(defaultSleepTime);

    return true;
}

void ServoControllerTcp::controlServos(const std::vector<Cube::Move>& moves)
{
    if (!initServos())
    {
        return;
    }

    ServoPositionsPlanner servoPositionsPlanner;
    const auto servoPositionsSequence = servoPositionsPlanner.planServoPositionsSequence(moves);

    std::cout << "Put cube in the initial position and press Enter to start solving..." << std::endl;
    std::cin.get();

    _pauseThread = std::thread(&ServoControllerTcp::pauseThreadFunction, this);

    for (const auto& [move, servoPositions] : servoPositionsSequence)
    {
        std::cout << "Executing move: " << Cube::moveToString(move) << std::endl;

        for (const auto& servoPosition : servoPositions)
        {
            std::unique_lock<std::mutex> lock(_pauseMutex);
            _pauseCondition.wait(lock,
                                 [this]
                                 {
                                     return !_paused;
                                 });
            lock.unlock();

            if (std::holds_alternative<BottomServoPosition>(servoPosition))
            {
                const auto bottomServoPosition = std::get<BottomServoPosition>(servoPosition);
                setBottomServoPosition(bottomServoPosition);
            }
            else if (std::holds_alternative<TopServoPosition>(servoPosition))
            {
                const auto topServoPosition = std::get<TopServoPosition>(servoPosition);
                setTopServoPosition(topServoPosition);
            }
        }
    }

    _finishPauseThread = true;
    _pauseThread.join();
}

void ServoControllerTcp::setBottomServoPosition(BottomServoPosition bottomServoPosition)
{
    if (bottomServoPosition != _currentBottomServoPosition)
    {
        _currentBottomServoPosition = bottomServoPosition;
        _tcpSender.sendData(translateServoPositionToCommand(bottomServoPosition));
        if (bottomServoPosition == BottomServoPosition::Center || _currentBottomServoPosition == BottomServoPosition::Center)
        {
            std::this_thread::sleep_for(defaultSleepTime);
        }
        else
        {
            // If the bottom servo is moving from clockwise to counter-clockwise or vice versa, we need to wait
            // longer for the servo to complete the rotation.
            std::this_thread::sleep_for(longSleepTime);
        }
    }
}
void ServoControllerTcp::setTopServoPosition(TopServoPosition topServoPosition)
{
    if (topServoPosition != _currentTopServoPosition)
    {
        _currentTopServoPosition = topServoPosition;
        _tcpSender.sendData(translateServoPositionToCommand(topServoPosition));
        std::this_thread::sleep_for(defaultSleepTime);
    }
}

void ServoControllerTcp::pauseThreadFunction()
{
    std::cout << "Press Enter to pause/resume the servo control..." << std::endl;
    while (!_finishPauseThread)
    {
        std::cin.get();
        std::lock_guard<std::mutex> lock(_pauseMutex);
        _paused = !_paused;
        if (_paused)
        {
            std::cout << "Servo control paused. Press Enter to resume..." << std::endl;
        }
        else
        {
            std::cout << "Servo control resumed." << std::endl;
        }
        _pauseCondition.notify_all();
    }
}
