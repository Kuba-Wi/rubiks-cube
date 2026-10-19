#include "TcpSender.h"
#include "Logger.h"
#include <arpa/inet.h>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

TcpSender::TcpSender(const std::string& ip, uint16_t portNum) :
    serverIp(ip),
    port(portNum),
    _sock(-1)
{
}

TcpSender::~TcpSender()
{
    if (_sock >= 0)
    {
        close(_sock);
    }
}

bool TcpSender::connectToServer()
{
    _sock = socket(AF_INET, SOCK_STREAM, 0);

    if (_sock < 0)
    {
        perror("socket");
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr) <= 0)
    {
        LOG_ERROR("Wrong IP");
        close(_sock);
        return false;
    }

    sockaddr address;
    std::memcpy(&address, &serverAddr, sizeof(serverAddr)); // memcpy to avoid strict aliasing issues

    if (connect(_sock, &address, sizeof(address)) < 0)
    {
        perror("connect");
        close(_sock);
        return false;
    }
    return true;
}

void TcpSender::sendData(const std::vector<uint8_t>& data)
{
    if (_sock < 0)
    {
        LOG_ERROR("Socket is not connected");
        return;
    }

    ssize_t sentBytes = send(_sock, data.data(), data.size(), 0);
    if (sentBytes < 0)
    {
        perror("send");
    }
}
