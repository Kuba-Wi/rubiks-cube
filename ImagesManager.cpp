#include "ImagesManager.h"

#include <filesystem>
#include <opencv2/opencv.hpp>
#include <vector>

namespace
{
// Classifies a single pixel's HSV color (OpenCV ranges: H 0-179, S/V 0-255)
StickerColor classifyHSV(double h, double s, double v);

// Finds the cube face: detects individual stickers (square contours of similar size) and returns their combined bounding
// box.
std::optional<cv::Rect> findFaceRect(const cv::Mat& image);

// Classifies a cell by per-pixel voting.
StickerColor classifyCell(const cv::Mat& hsvImage, const cv::Rect& cell);
} // namespace

ImagesManager::ImagesManager(const std::string& imagesDirPath) :
    _imagesDirPath(imagesDirPath)
{
}

const CubeColorsData& ImagesManager::getCubeColorsData() const
{
    return _cubeColorsData;
}

void ImagesManager::loadCubeColorsData()
{
    namespace fs = std::filesystem;
    fs::path dir = _imagesDirPath;

    for (const auto& entry : fs::directory_iterator(dir))
    {
        loadCubeColorsDataForImage(entry.path().string());
    }
}

void ImagesManager::loadCubeColorsDataForImage(const std::string& imagePath)
{
    cv::Mat image = cv::imread(imagePath);
    if (image.empty())
    {
        std::cerr << "Failed to load image: " << imagePath << "\n";
        return;
    }

    const std::optional<cv::Rect> faceRect = findFaceRect(image);
    if (!faceRect)
    {
        std::cerr << "Failed to find face in image: " << imagePath << "\n";
        return;
    }
    cv::Mat face = image(*faceRect);

    cv::Mat hsv;
    cv::cvtColor(face, hsv, cv::COLOR_BGR2HSV);

    constexpr int N = static_cast<int>(CubeColorsData::rowColumnSize);
    std::array<std::array<StickerColor, N>, N> colors{};

    const int cellW = face.cols / N;
    const int cellH = face.rows / N;

    for (int row = 0; row < N; ++row)
    {
        for (int col = 0; col < N; ++col)
        {
            const cv::Rect cell{col * cellW, row * cellH, cellW, cellH};
            colors[row][col] = classifyCell(hsv, cell);
        }
    }

    _cubeColorsData.addFaceColors(colors);

    std::cout << "Detected colors from " << imagePath << ":\n";
    for (const auto& rowColors : colors)
    {
        for (size_t col = 0; col < rowColors.size(); ++col)
        {
            std::cout << toString(rowColors[col]) << (col + 1 < rowColors.size() ? " | " : "");
        }
        std::cout << "\n";
    }
}

namespace
{
StickerColor classifyHSV(double h, double s, double v)
{
    if (s < 60 && v > 130)
    {
        // White: low saturation, high brightness
        return StickerColor::White;
    }
    if (v < 60 || s < 60)
    {
        // to dark/faded (black plastic, shadow)
        return StickerColor::Unknown;
    }
    if (h < 5 || h >= 165)
    {
        return StickerColor::Red;
    }
    if (h < 20)
    {
        return StickerColor::Orange;
    }
    if (h < 40)
    {
        return StickerColor::Yellow;
    }
    if (h < 100)
    {
        return StickerColor::Green;
    }
    if (h < 135)
    {
        return StickerColor::Blue;
    }
    return StickerColor::Unknown;
}

std::optional<cv::Rect> findFaceRect(const cv::Mat& image)
{
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, {5, 5}, 0);

    cv::Mat edges;
    cv::Canny(gray, edges, 30, 90);
    cv::dilate(edges, edges, cv::getStructuringElement(cv::MORPH_RECT, {7, 7}));

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    const double imageArea = static_cast<double>(image.cols) * image.rows;

    // Kandydaci na naklejki: kwadratowe, wypełnione kontury rozsądnej wielkości
    std::vector<cv::Rect> stickers;
    for (const auto& contour : contours)
    {
        const cv::Rect rect = cv::boundingRect(contour);
        const double rectArea = static_cast<double>(rect.area());
        const double aspect = static_cast<double>(rect.width) / rect.height;

        if (rectArea < imageArea / 400.0 || rectArea > imageArea / 9.0)
        {
            continue;
        }
        if (aspect < 0.65 || aspect > 1.55)
        {
            continue;
        }
        const double fill = cv::contourArea(contour) / rectArea;
        if (fill < 0.6)
        {
            continue;
        }

        stickers.push_back(rect);
    }

    constexpr size_t minStickersCount = 9;
    if (stickers.size() < minStickersCount)
    {
        std::cerr << "findFaceRect: not enough sticker candidates\n";
        return std::nullopt;
    }

    // Discard sizes that are too far from the median (e.g., hand, background elements)
    std::vector<double> areas;
    areas.reserve(stickers.size());
    for (const auto& r : stickers)
    {
        areas.push_back(static_cast<double>(r.area()));
    }
    std::nth_element(areas.begin(), areas.begin() + areas.size() / 2, areas.end());
    const double medianArea = areas[areas.size() / 2];

    cv::Rect face;
    size_t accepted = 0;
    for (const auto& r : stickers)
    {
        const double a = static_cast<double>(r.area());
        if (a < 0.4 * medianArea || a > 2.5 * medianArea)
        {
            continue;
        }
        face = (accepted == 0) ? r : (face | r);
        ++accepted;
    }

    if (accepted < minStickersCount)
    {
        std::cerr << "findFaceRect: not enough accepted stickers\n";
        return std::nullopt;
    }
    return face & cv::Rect{0, 0, image.cols, image.rows};
}

StickerColor classifyCell(const cv::Mat& hsvImage, const cv::Rect& cell)
{
    const int marginX = cell.width / 4;
    const int marginY = cell.height / 4;
    const cv::Rect inner{cell.x + marginX, cell.y + marginY, cell.width - 2 * marginX, cell.height - 2 * marginY};

    std::array<size_t, static_cast<size_t>(StickerColor::ColorsCount)> votes{};
    const cv::Mat region = hsvImage(inner);
    StickerColor color;
    for (int y = 0; y < region.rows; ++y)
    {
        const auto* rowPtr = region.ptr<cv::Vec3b>(y);
        for (int x = 0; x < region.cols; ++x)
        {
            color = classifyHSV(rowPtr[x][0], rowPtr[x][1], rowPtr[x][2]);
            if (color != StickerColor::Unknown)
            {
                ++votes[static_cast<size_t>(color)];
            }
        }
    }

    const size_t total = static_cast<size_t>(region.rows * region.cols);
    const auto maxIt = std::max_element(votes.begin(), votes.end());

    const size_t minVotesThreshold = total / 10;
    if (*maxIt < minVotesThreshold)
    {
        return StickerColor::Unknown;
    }
    return static_cast<StickerColor>(maxIt - votes.begin());
}
} // namespace
