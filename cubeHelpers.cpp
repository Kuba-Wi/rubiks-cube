#include "cubeHelpers.h"

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
