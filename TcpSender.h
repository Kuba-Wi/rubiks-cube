#pragma once

#include <cstdint>
#include <string>
#include <vector>

class TcpSender
{
public:
    TcpSender(const std::string& ip, uint16_t portNum);
    ~TcpSender();

    bool connectToServer();
    void sendData(const std::vector<uint8_t>& data);

    const std::string serverIp;
    const uint16_t port;

private:
    int _sock;
};
