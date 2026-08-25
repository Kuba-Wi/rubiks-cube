#include "cubeHelpers.h"

#include <algorithm>

bool CubeColorsData::addFaceColors(const FaceColors& colors)
{
    static constexpr size_t centerIndex = 1;
    for (const auto& row : colors)
    {
        if (std::any_of(row.begin(),
                        row.end(),
                        [](StickerColor c)
                        {
                            return c == StickerColor::Unknown;
                        }))
        {
            return false;
        }
    }

    if (_faceColors.contains(colors[centerIndex][centerIndex]) && _faceColors[colors[centerIndex][centerIndex]] == colors)
    {
        return false;
    }

    _faceColors[colors[centerIndex][centerIndex]] = colors;
    return true;
}

std::string toString(StickerColor color)
{
    switch (color)
    {
        case StickerColor::White:
            return "White";
        case StickerColor::Yellow:
            return "Yellow";
        case StickerColor::Red:
            return "Red";
        case StickerColor::Orange:
            return "Orange";
        case StickerColor::Green:
            return "Green";
        case StickerColor::Blue:
            return "Blue";
        default:
            return "Unknown";
    }
}
