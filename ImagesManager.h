#pragma once

#include "cubeHelpers.h"

#include <opencv2/opencv.hpp>
#include <string>

class ImagesManager
{
public:
    bool loadCubeColorsData(const std::string& imagesDirPath);
    const CubeColorsData& getCubeColorsData() const;

    /*!
     * Loads the cube colors data from a single image. The image is expected to contain a single face of the cube.
     * @param image The image containing a single face of the cube.
     * @param centerColor The expected center color of the face in the image. If set to StickerColor::Unknown,
     * the data from any image will be accepted. If set to a specific color, the data from the image will only be accepted if
     * the center color matches.
     * @return true if the colors data was successfully loaded from the image, false otherwise.
     */
    bool loadCubeColorsDataForImage(const cv::Mat& image, StickerColor centerColor = StickerColor::Unknown);
    bool loadCubeColorsDataForImage(const std::string& imagePath);
    void printCubeColorsData() const;
    void printCubeFaceColors(StickerColor centerColor) const;

private:
    CubeColorsData _cubeColorsData;
};
