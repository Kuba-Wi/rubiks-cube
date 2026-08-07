#pragma once

#include "cubeHelpers.h"

#include <string>

class ImagesManager
{
public:
    ImagesManager(const std::string& imagesDirPath);

    void loadCubeColorsData();
    const CubeColorsData& getCubeColorsData() const;
private:
    void loadCubeColorsDataForImage(const std::string& imagePath);

    CubeColorsData _cubeColorsData;
    std::string _imagesDirPath;
};
