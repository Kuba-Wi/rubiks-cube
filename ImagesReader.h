#pragma once

#include "cubeHelpers.h"
#include "ImagesManager.h"
#include "ImagesStreamReceiver.h"

#include <memory>
#include <opencv2/opencv.hpp>

class ImagesReader
{
public:
    ImagesReader(std::unique_ptr<ImagesManager>&& imgManager, std::unique_ptr<ImagesStreamReceiver>&& imgStreamReceiver);
    void operateOnImage(const cv::Mat& image);
    void onKeyClicked(int key);
    void readImages();
    void printCubeColorsData() const;
    const CubeColorsData& getCubeColorsData() const;

private:
    StickerColor _currentColor = StickerColor::White;
    std::unique_ptr<ImagesManager> _imgManager;
    std::unique_ptr<ImagesStreamReceiver> _imgStreamReceiver;
};
