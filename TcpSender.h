#pragma once

#include <cstdint>
#include <string>
#include <vector>

class TcpSender
{
public:
    TcpSender(const std::string& serverIp = "192.168.5.9", uint16_t port = 5000);
    ~TcpSender();

    bool connectToServer();
    void sendData(const std::vector<uint8_t>& data);

private:
    std::string _serverIp;
    uint16_t _port;
    int _sock;
};
