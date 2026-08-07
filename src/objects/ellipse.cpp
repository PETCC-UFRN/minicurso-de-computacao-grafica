#include "ellipse.hpp"

#include <algorithm>
#include <cmath>

namespace pet {

namespace {
    void plotSymmetricPoints(Canvas& canvas, const Point2& center, int x, int y,
                             const RGBColor& color) {
        canvas.add(Pixel(center.x() + x, center.y() + y), color);
        canvas.add(Pixel(center.x() - x, center.y() + y), color);
        canvas.add(Pixel(center.x() + x, center.y() - y), color);
        canvas.add(Pixel(center.x() - x, center.y() - y), color);
    }
}

void Ellipse::drawObject(Canvas& canvas, RGBColor color, DrawMethod method) {
    (void)method;

    int rx = std::max(1, static_cast<int>(std::round(std::abs(radiusX * scale.x()))));
    int ry = std::max(1, static_cast<int>(std::round(std::abs(radiusY * scale.y()))));

    int x = 0;
    int y = ry;

    double rx2 = static_cast<double>(rx) * static_cast<double>(rx);
    double ry2 = static_cast<double>(ry) * static_cast<double>(ry);

    double d1 = ry2 - rx2 * ry + 0.25 * rx2;
    double dx = 2.0 * ry2 * x;
    double dy = 2.0 * rx2 * y;

    while (dx < dy) {
        plotSymmetricPoints(canvas, center, x, y, color);

        if (d1 < 0.0) {
            ++x;
            dx += 2.0 * ry2;
            d1 += dx + ry2;
        } else {
            ++x;
            --y;
            dx += 2.0 * ry2;
            dy -= 2.0 * rx2;
            d1 += dx - dy + ry2;
        }
    }

    double d2 = ry2 * std::pow(static_cast<double>(x) + 0.5, 2.0) +
                rx2 * std::pow(static_cast<double>(y) - 1.0, 2.0) - rx2 * ry2;

    while (y >= 0) {
        plotSymmetricPoints(canvas, center, x, y, color);

        if (d2 > 0.0) {
            --y;
            dy -= 2.0 * rx2;
            d2 += rx2 - dy;
        } else {
            ++x;
            --y;
            dx += 2.0 * ry2;
            dy -= 2.0 * rx2;
            d2 += dx - dy + rx2;
        }
    }
}

void Ellipse::transform(const Transform& transformation)
{
    Point2 oldCenter = center;

    center = transformation * oldCenter;

    Point2 edgeX(oldCenter.x() + static_cast<int>(radiusX), oldCenter.y());
    Point2 edgeY(oldCenter.x(), oldCenter.y() + static_cast<int>(radiusY));

    Point2 transformedEdgeX = transformation * edgeX;
    Point2 transformedEdgeY = transformation * edgeY;

    radiusX = std::abs(transformedEdgeX.x() - center.x());
    radiusY = std::abs(transformedEdgeY.y() - center.y());
}


Point2 Ellipse::getAxis() const
{
    return center;
}

}
namespace pet {

    void Ellipse::fillObject(Canvas& canvas, const RGBColor& color) {
        /// Mesmo princípio do círculo: (x/rx)² + (y/ry)² = 1 resolve para
        /// meia-largura hw(dy) = rx * sqrt(1 - (dy/ry)²).
        if (radiusY <= 0.0 || radiusX <= 0.0)
            return;
        int ry = static_cast<int>(std::floor(radiusY));
        for (int dy = -ry; dy <= ry; ++dy) {
            double t = double(dy) / radiusY;
            int hw = static_cast<int>(
                std::floor(radiusX * std::sqrt(1.0 - t * t)));
            int y = center.y() + dy;
            for (int x = center.x() - hw; x <= center.x() + hw; ++x)
                canvas.add(Pixel(x, y), color);
        }
    }
}
