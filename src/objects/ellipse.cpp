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
	//TODO: missing
}

void Ellipse::transform(const Transform& transformation)
{
	//TODO: missing
}


Point2 Ellipse::getAxis() const
{
	//TODO: missing
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
