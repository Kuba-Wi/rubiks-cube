#pragma once

#include <array>
#include <map>
#include <string>

enum class StickerColor
{
    White = 0,
    Yellow,
    Red,
    Orange,
    Green,
    Blue,
    Unknown,
    ColorsCount = Blue + 1 // colors considered for classification (excluding Unknown)
};

class CubeColorsData
{
public:
    static constexpr size_t rowColumnSize = 3;
    using FaceColors = std::array<std::array<StickerColor, rowColumnSize>, rowColumnSize>;

    static StickerColor getCenterColor(const FaceColors& colors)
    {
        return colors[rowColumnSize / 2][rowColumnSize / 2];
    }

    std::map<StickerColor, FaceColors> getFaceColors() const
    {
        return _faceColors;
    }

    bool addFaceColors(const FaceColors& colors);

private:
    std::map<StickerColor, FaceColors> _faceColors;
};

std::string toString(StickerColor color);
