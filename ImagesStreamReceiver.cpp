#include "ImagesStreamReceiver.h"

#include "cubeHelpers.h"
#include "ImagesReader.h"

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

void ImagesStreamReceiver::attachObserver(ImagesReader* observer)
{
    _observer = observer;
}

void ImagesStreamReceiver::stopStream()
{
    _streaming = false;
}

void ImagesStreamReceiver::receiveImageStream()
{
    cv::VideoCapture camera(0); // Open the default camera (index 0)

    if (!camera.isOpened())
    {
        std::cerr << "Fail. Couldn't open a stream\n";
        return;
    }

    std::cout << "Stream open.\n";

    cv::Mat frame;
    _streaming = true;
    while (_streaming)
    {
        camera >> frame;

        if (frame.empty())
        {
            std::cerr << "Error: Empty frame received.\n";
            break;
        }

        cv::imshow("Camera", frame);
        _observer->operateOnImage(frame);

        int keyClicked = cv::waitKey(1);
        _observer->onKeyClicked(keyClicked);
    }

    camera.release();
    cv::destroyAllWindows();
}
