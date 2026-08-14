#pragma once

#include "cubeHelpers.h"

#include <string>

class ImagesManager
{
public:
    ImagesManager(const std::string& imagesDirPath);

    bool loadCubeColorsData();
    const CubeColorsData& getCubeColorsData() const;
private:
    bool loadCubeColorsDataForImage(const std::string& imagePath);

    CubeColorsData _cubeColorsData;
    std::string _imagesDirPath;
};
