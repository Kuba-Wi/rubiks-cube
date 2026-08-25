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

// Groups sorted 1D values into clusters (gap > tol starts a new cluster) and returns the cluster centers.
std::vector<double> cluster1D(std::vector<double> values, double tol);

// Returns average edge intensity in a rectangle - a measure of "is there a cube here"
// Returns -1.0 if the rectangle is outside of the edges bounds.
double edgeDensity(const cv::Mat& edges, cv::Rect rect);

// Sets the center of the first (extreme) row/column of a 3x3 grid based on detected clusters. When the position is ambiguous
// (less than 3 clusters detected), it chooses the hypothesis with the highest edge density in the missing strip.
// axis=0: columns (X-axis), axis=1: rows (Y-axis).
double gridStart(const std::vector<double>& centers, double pitch, const cv::Mat& edges, int axis);
} // namespace

const CubeColorsData& ImagesManager::getCubeColorsData() const
{
    return _cubeColorsData;
}

bool ImagesManager::loadCubeColorsData(const std::string& imagesDirPath)
{
    namespace fs = std::filesystem;
    fs::path dir = imagesDirPath;

    try
    {
        for (const auto& entry : fs::directory_iterator(dir))
        {
            if (!loadCubeColorsDataForImage(entry.path().string()))
            {
                return false;
            }
        }
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Filesystem error: " << e.what() << "\n";
        return false;
    }
    return true;
}

bool ImagesManager::loadCubeColorsDataForImage(const std::string& imagePath)
{
    std::cout << "Loading colors for image " << imagePath << ".\n";

    cv::Mat image = cv::imread(imagePath);
    if (image.empty())
    {
        std::cerr << "Failed to load image " << imagePath << ".\n";
        return false;
    }
    if (!loadCubeColorsDataForImage(image))
    {
        std::cerr << "Failed to load colors from image " << imagePath << ".\n";
        return false;
    }

    return true;
}

bool ImagesManager::loadCubeColorsDataForImage(const cv::Mat& image, StickerColor centerColor)
{
    const std::optional<cv::Rect> faceRect = findFaceRect(image);
    if (!faceRect)
    {
        return false;
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
            if (colors[row][col] == StickerColor::Unknown)
            {
                return false;
            }
        }
    }

    if (centerColor != StickerColor::Unknown && CubeColorsData::getCenterColor(colors) != centerColor)
    {
        return false;
    }

    if (_cubeColorsData.addFaceColors(colors))
    {
        for (const auto& rowColors : colors)
        {
            for (size_t col = 0; col < rowColors.size(); ++col)
            {
                std::cout << toString(rowColors[col]) << (col + 1 < rowColors.size() ? " | " : "");
            }
            std::cout << "\n";
        }
        return true;
    }

    return false;
}

void ImagesManager::printCubeColorsData() const
{
    const auto faceColorsMap = _cubeColorsData.getFaceColors();
    for (const auto& [centerColor, faceColors] : faceColorsMap)
    {
        printCubeFaceColors(centerColor);
        std::cout << "\n";
    }
}

void ImagesManager::printCubeFaceColors(StickerColor centerColor) const
{
    const auto faceColorsMap = _cubeColorsData.getFaceColors();
    const auto it = faceColorsMap.find(centerColor);
    if (it != faceColorsMap.end())
    {
        const auto& faceColors = it->second;
        for (const auto& rowColors : faceColors)
        {
            for (size_t col = 0; col < rowColors.size(); ++col)
            {
                std::cout << toString(rowColors[col]) << (col + 1 < rowColors.size() ? " | " : "");
            }
            std::cout << "\n";
        }
    }
    else
    {
        std::cout << "No face found with center color " << toString(centerColor) << ".\n";
    }
}

namespace
{
StickerColor classifyHSV(double h, double s, double v)
{
    if (s < 110 && v > 130)
    {
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

std::vector<double> cluster1D(std::vector<double> values, double tol)
{
    std::sort(values.begin(), values.end());
    std::vector<double> centers;
    double sum = values[0];
    int count = 1;
    for (size_t i = 1; i < values.size(); ++i)
    {
        if (values[i] - values[i - 1] > tol)
        {
            centers.push_back(sum / count);
            sum = 0.0;
            count = 0;
        }
        sum += values[i];
        ++count;
    }
    centers.push_back(sum / count);
    return centers;
}

double edgeDensity(const cv::Mat& edges, cv::Rect rect)
{
    rect &= cv::Rect{0, 0, edges.cols, edges.rows};
    if (rect.area() <= 0)
    {
        return -1.0;
    }
    return cv::mean(edges(rect))[0];
}

double gridStart(const std::vector<double>& centers, double pitch, const cv::Mat& edges, int axis)
{
    if (centers.size() >= 3)
    {
        return centers.front();
    }
    if (centers.size() == 2 && centers[1] - centers[0] > 1.5 * pitch)
    {
        return centers.front(); // detected clusters are the extreme rows/columns
    }

    // Possible positions of the first row/column relative to the detected clusters
    std::vector<double> hypotheses;
    if (centers.size() == 2)
    {
        hypotheses = {centers[0], centers[0] - pitch};
    }
    else
    {
        hypotheses = {centers[0], centers[0] - pitch, centers[0] - 2.0 * pitch};
    }

    double bestStart = hypotheses.front();
    double bestScore = -1.0;
    const int strip = static_cast<int>(std::lround(pitch));
    for (double start : hypotheses)
    {
        double score = 0.0;
        int strips = 0;
        for (int i = 0; i < 3; ++i)
        {
            const double center = start + i * pitch;
            // Skip strips covered by detected clusters - only missing ones differentiate
            bool covered = false;
            for (double c : centers)
            {
                if (std::abs(c - center) < 0.5 * pitch)
                    covered = true;
            }
            if (covered)
            {
                continue;
            }
            const int lo = static_cast<int>(std::lround(center - pitch / 2.0));
            const cv::Rect r = (axis == 0) ? cv::Rect{lo, 0, strip, edges.rows} : cv::Rect{0, lo, edges.cols, strip};
            const double d = edgeDensity(edges, r);
            if (d >= 0.0)
            {
                score += d;
                ++strips;
            }
            else
            {
                score -= 1.0E9; // strip outside the image - impossible hypothesis
            }
        }
        if (strips > 0 && score > bestScore)
        {
            bestScore = score;
            bestStart = start;
        }
    }
    return bestStart;
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
    const cv::Rect fullImage{0, 0, image.cols, image.rows};

    // Candidates for stickers: square, filled contours of reasonable size
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

    constexpr size_t minStickersCount = 4;
    if (stickers.size() < minStickersCount)
    {
        // std::cerr << "findFaceRect: not enough sticker candidates\n";
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

    std::vector<cv::Rect> accepted;
    for (const auto& r : stickers)
    {
        const double a = static_cast<double>(r.area());
        if (a < 0.4 * medianArea || a > 2.5 * medianArea)
        {
            continue;
        }
        accepted.push_back(r);
    }

    if (accepted.size() < minStickersCount)
    {
        // std::cerr << "findFaceRect: not enough accepted stickers\n";
        return std::nullopt;
    }

    // Fit a 3x3 grid: cluster the centers of the stickers into rows and columns.
    // Duplicate contours of the same sticker fall into the same cluster,
    // and missing rows/columns are extrapolated from the grid pitch.
    const double medianSize = std::sqrt(medianArea);
    std::vector<double> xs, ys;
    for (const auto& r : accepted)
    {
        xs.push_back(r.x + r.width / 2.0);
        ys.push_back(r.y + r.height / 2.0);
    }
    const std::vector<double> colCenters = cluster1D(xs, 0.5 * medianSize);
    const std::vector<double> rowCenters = cluster1D(ys, 0.5 * medianSize);

    // Fallback: nietypowy układ klastrów - wspólny obrys naklejek
    if (colCenters.size() > 3 || rowCenters.size() > 3)
    {
        cv::Rect face = accepted[0];
        for (const auto& r : accepted)
        {
            face |= r;
        }

        // std::cerr << "findFaceRect: too many clusters (" << colCenters.size() << "x" << rowCenters.size()
        //           << ")\n";

        return std::nullopt; // to avoid detecting wrong colors on a non-cube object
    }

    // Grid step: distance between neighboring rows/columns
    auto pitchFrom = [&](const std::vector<double>& centers) -> double
    {
        if (centers.size() == 3)
        {
            return (centers[2] - centers[0]) / 2.0;
        }
        if (centers.size() == 2)
        {
            const double gap = centers[1] - centers[0];
            return gap > 1.5 * medianSize ? gap / 2.0 : gap;
        }
        return 0.0;
    };

    double pitch = std::max(pitchFrom(colCenters), pitchFrom(rowCenters));
    if (pitch <= 0.0)
    {
        pitch = medianSize; // only one cluster on both axes
    }

    const double startX = gridStart(colCenters, pitch, edges, 0);
    const double startY = gridStart(rowCenters, pitch, edges, 1);

    const int x0 = static_cast<int>(std::lround(startX - pitch / 2.0));
    const int y0 = static_cast<int>(std::lround(startY - pitch / 2.0));
    const int side = static_cast<int>(std::lround(3.0 * pitch));
    const cv::Rect face = cv::Rect{x0, y0, side, side} & fullImage;

    if (face.area() <= 0)
    {
        return std::nullopt;
    }
    return face;
}

StickerColor classifyCell(const cv::Mat& hsvImage, const cv::Rect& cell)
{
    const int marginX = cell.width / 4;
    const int marginY = cell.height / 4;
    const cv::Rect inner{cell.x + marginX, cell.y + marginY, cell.width - 2 * marginX, cell.height - 2 * marginY};

    std::array<size_t, static_cast<size_t>(StickerColor::ColorsCount)> votes{};
    double roHueSum = 0.0;
    double roValSum = 0.0;
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
                if (color == StickerColor::Red || color == StickerColor::Orange)
                {
                    const int h = rowPtr[x][0];
                    roHueSum += h >= 165 ? h - 180 : h;
                    roValSum += rowPtr[x][2];
                }
            }
        }
    }

    const size_t total = static_cast<size_t>(region.rows * region.cols);
    auto maxIt = std::max_element(votes.begin(), votes.end());

    // Special case: if Red is the most voted color, check if Orange is also present in significant amount
    // If Orange has at least 1/3 of Red's votes, classify as Orange instead
    // Because Red can wrap around the hue spectrum, we want to avoid misclassifying Orange as Red
    if (maxIt - votes.begin() == static_cast<size_t>(StickerColor::Red) ||
        maxIt - votes.begin() == static_cast<size_t>(StickerColor::Orange))

    {
        const size_t redVotes = votes[static_cast<size_t>(StickerColor::Red)];
        const size_t orangeVotes = votes[static_cast<size_t>(StickerColor::Orange)];
        const size_t votesCombined = redVotes + orangeVotes;
        const double meanH = roHueSum / double(votesCombined);
        const double meanV = roValSum / double(votesCombined);
        if (meanH > 10.0)
        {
            maxIt = votes.begin() + static_cast<size_t>(StickerColor::White);
            *maxIt = votes[static_cast<size_t>(StickerColor::White)] + votesCombined;
        }
        else if (meanH < 3.3)
        {
            maxIt = votes.begin() + static_cast<size_t>(StickerColor::Red);
            *maxIt = votesCombined;
        }
        else if (meanH > 4.5)
        {
            maxIt = votes.begin() + static_cast<size_t>(StickerColor::Orange);
            *maxIt = votesCombined;
        }
        else
        {
            maxIt = votes.begin() +
                    (meanV > 180.0 ? static_cast<size_t>(StickerColor::Orange) : static_cast<size_t>(StickerColor::Red));
            *maxIt = votesCombined; // Update the max votes to the combined count for threshold check
        }
    }

    const size_t minVotesThreshold = total / 10;
    if (*maxIt < minVotesThreshold)
    {
        return StickerColor::Unknown;
    }
    return static_cast<StickerColor>(maxIt - votes.begin());
}
} // namespace
