#include "ServoControllerTcp.h"

#include "Logger.h"

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

ServoControllerTcp::ServoControllerTcp(const std::string& serverIp, uint16_t port)
    : _tcpSender(serverIp, port)
{
}

bool ServoControllerTcp::initServos()
{
    LOG_INFO("Connecting to the server at ", _tcpSender.serverIp, ":", int(_tcpSender.port), ".");
    if (!_tcpSender.connectToServer())
    {
        LOG_ERROR("Failed to connect to the server.");
        return false;
    }
    LOG_INFO("Connected.");

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
    ServoPositionsPlanner servoPositionsPlanner;
    auto servoPositionsSequence = servoPositionsPlanner.planServoPositionsSequence(moves);

    for (const auto& [move, servoPositions] : servoPositionsSequence)
    {
        LOG_INFO("Executing move: ", Cube::moveToString(move), ".");

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
}

void ServoControllerTcp::pauseControl()
{
    std::lock_guard<std::mutex> lock(_pauseMutex);
    _paused = true;
}

void ServoControllerTcp::resumeControl()
{
    std::lock_guard<std::mutex> lock(_pauseMutex);
    _paused = false;
    _pauseCondition.notify_all();
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
