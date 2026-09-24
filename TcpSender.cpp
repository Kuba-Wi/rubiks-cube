#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#include <thread>
#include <chrono>

int main()
{
    const char* SERVER_IP = "192.168.5.9";
    const int PORT = 5000;

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        perror("socket");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0)
    {
        std::cerr << "Wrong IP\n";
        close(sock);
        return 1;
    }
    
    sockaddr address;
    std::memcpy(&address, &serverAddr, sizeof(serverAddr)); // memcpy to avoid strict aliasing issues

    if (connect(sock, &address, sizeof(address)) < 0)
    {
        perror("connect");
        close(sock);
        return 1;
    }

    std::cout << "Connected.\n";

    auto sendData = [&sock](uint8_t servoIndex, uint8_t angle, int delayMs = 1000) {
        uint8_t data[2] = {servoIndex, angle};
        ssize_t sentBytes = send(sock, data, sizeof(data), 0);
        if (sentBytes < 0)
        {
            perror("send");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    };

    sendData(0, 0);
    sendData(1, 0, 2000);


    while (true)
    {
        sendData(1, 58);
        sendData(0, 180, 1500);
        sendData(1, 88);
        sendData(0, 0, 1500);
        sendData(1, 58);
        sendData(0, 90);

        sendData(1, 0);
        sendData(1, 58);
        sendData(1, 0);
        sendData(1, 58);

        sendData(0, 180);
        sendData(1, 88);
        sendData(0, 90);
    }

    close(sock);

    return 0;
}
