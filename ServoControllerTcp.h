#pragma once

#include "Cube.h"
#include "ServoController.h"
#include "ServoPositionsPlanner.h"
#include "TcpSender.h"

#include <atomic>
#include <condition_variable>
#include <thread>

class ServoControllerTcp : public ServoController
{
public:
    enum BottomServoAngle : uint8_t
    {
        ClockwiseAngle = 0,
        CenterAngle = 90,
        CounterClockwiseAngle = 180,
    };

    enum TopServoAngle : uint8_t
    {
        LiftAngle = 0,
        UpAngle = 58,
        DownAngle = 88
    };

    ~ServoControllerTcp() override = default;
    void controlServos(const std::vector<Cube::Move>& moves) override;

private:
    bool initServos();
    void pauseThreadFunction();
    void setBottomServoPosition(BottomServoPosition bottomServoPosition);
    void setTopServoPosition(TopServoPosition topServoPosition);

    std::thread _pauseThread;
    std::atomic<bool> _paused{false};
    std::atomic<bool> _finishPauseThread{false};
    std::mutex _pauseMutex;
    std::condition_variable _pauseCondition;

    TcpSender _tcpSender;
    BottomServoPosition _currentBottomServoPosition;
    TopServoPosition _currentTopServoPosition;
};
