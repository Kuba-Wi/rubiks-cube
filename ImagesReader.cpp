#include "ImagesReader.h"

ImagesReader::ImagesReader(std::unique_ptr<ImagesManager>&& imgManager,
                           std::unique_ptr<ImagesStreamReceiver>&& imgStreamReceiver) :
    _imgManager(std::move(imgManager)),
    _imgStreamReceiver(std::move(imgStreamReceiver))
{
    _imgStreamReceiver->attachObserver(this);
}

void ImagesReader::operateOnImage(const cv::Mat& image)
{
    _imgManager->loadCubeColorsDataForImage(image, _currentColor);
}

void ImagesReader::onKeyClicked(int key)
{
    static std::map<char, StickerColor> colorLetters = {
        {'w', StickerColor::White },
        {'y', StickerColor::Yellow},
        {'r', StickerColor::Red   },
        {'o', StickerColor::Orange},
        {'g', StickerColor::Green },
        {'b', StickerColor::Blue  }
    };

    if (key == 'q')
    {
        std::cout << "Colors data set for color " << toString(_currentColor) << ":\n";
        _imgManager->printCubeFaceColors(_currentColor);
        _imgStreamReceiver->stopStream();
        return;
    }
    else if (const auto it = colorLetters.find(static_cast<char>(key)); it != colorLetters.end())
    {
        if (it->second == _currentColor)
        {
            std::cout << "Color " << toString(_currentColor) << " already selected. Ignoring.\n";
            return;
        }
        std::cout << "Colors data set for color " << toString(_currentColor) << ":\n";
        _imgManager->printCubeFaceColors(_currentColor);
        _currentColor = it->second;
        std::cout << "Reading frame with color " << toString(_currentColor) << ".\n";
    }
}

void ImagesReader::readImages()
{
    std::cout << "Reading frame with color " << toString(_currentColor) << ".\n";
    _imgStreamReceiver->receiveImageStream();
}

void ImagesReader::printCubeColorsData() const
{
    _imgManager->printCubeColorsData();
}

const CubeColorsData& ImagesReader::getCubeColorsData() const
{
    return _imgManager->getCubeColorsData();
}
