#pragma once

class ImagesReader;

class ImagesStreamReceiver
{
public:
    void attachObserver(ImagesReader* observer);
    void receiveImageStream();
    void stopStream();

private:
    const int port = 5000;

    bool _streaming = false;
    ImagesReader* _observer = nullptr;
};
