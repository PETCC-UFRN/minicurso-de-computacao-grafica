#include "circle.hpp"
#include "common.hpp"
#include "interpoledBackground.hpp"
#include "object.hpp"

#include <cmath>

namespace pet {
    void Circle::drawObject(Canvas& canvas, RGBColor color, DrawMethod method) {
		switch (method) {
			case DrawMethod::XiaolinWu:
				drawXiaolinWu(this->radius, canvas, color);

				break;
			default:
				drawBresenhan(this->radius, canvas, color);
				break;
		}
    }

    void Circle::transform(const Transform& transformation)
    {
        this->center = transformation * this->center;
    }

    Point2 Circle::getAxis() const
    {
        return center;
    }

	void Circle::drawBresenhan(int radius, Canvas& canvas, RGBColor color) {
        int x = 0;
        int y = radius;
        int d = 1 - radius;

        Pixel p(x, y);
        canvas.add(p, color);
        while(y > x){
            for(int i = -1; i <= 1; i += 2){
                for(int j = -1; j <= 1; j += 2){
                    Pixel p(center.x() + x * i, center.y() + y * j);
                    canvas.add(p, color);
                    p = Pixel(center.x() + y * i, center.y() + x * j);
                    canvas.add(p, color);
                }
            }
            if(d < 0){
                d += 2 * x + 1;
                x++;
            }
            else{
                d += -2 * y + 1;
                y--;
            }
        }
	}

	void Circle::drawXiaolinWu(int radius, Canvas& canvas, RGBColor color) {
		auto plot = [&](Canvas& canvas, int x, int y, RGBColor color, double alpha)  {
			alpha = std::clamp(alpha, 0.0, 1.0);

			auto current_pixel = canvas.pixel(Pixel(x, y));
			RGBColor pixel_color;

			if(current_pixel.has_value()) {
        		pixel_color = InterpoledBackground::linear_interpolation(
            		current_pixel.value(), color, alpha);
				}

			canvas.add(Pixel(x, y), pixel_color);
		};
		auto fpart = [&](double x) -> double {
			return x - std::floor(x);
		};
		auto rfpart = [&](double x) -> double {
    		return 1.0 - fpart(x);
		};

		int r = radius;

		for (int x = 0; x <= r / std::sqrt(2.0); ++x) {

			double y = std::sqrt(double(r * r - x * x));

			int yInt = std::floor(y);

			double frac = fpart(y);

			// oito octantes
			plot(canvas, center.x() + x, center.y() + yInt, color, rfpart(frac));
			plot(canvas, center.x() + x, center.y() + yInt + 1, color, frac);

			plot(canvas, center.x() - x, center.y() + yInt, color, rfpart(frac));
			plot(canvas, center.x() - x, center.y() + yInt + 1, color, frac);

			plot(canvas, center.x() + x, center.y() - yInt, color, rfpart(frac));
			plot(canvas, center.x() + x, center.y() - yInt - 1, color, frac);

			plot(canvas, center.x() - x, center.y() - yInt, color, rfpart(frac));
			plot(canvas, center.x() - x, center.y() - yInt - 1, color, frac);

			plot(canvas, center.x() + yInt, center.y() + x, color, rfpart(frac));
			plot(canvas, center.x() + yInt + 1, center.y() + x, color, frac);

			plot(canvas, center.x() - yInt, center.y() + x, color, rfpart(frac));
			plot(canvas, center.x() - yInt - 1, center.y() + x, color, frac);

			plot(canvas, center.x() + yInt, center.y() - x, color, rfpart(frac));
			plot(canvas, center.x() + yInt + 1, center.y() - x, color, frac);

			plot(canvas, center.x() - yInt, center.y() - x, color, rfpart(frac));
			plot(canvas, center.x() - yInt - 1, center.y() - x, color, frac);
		}
	}

    void Circle::fillObject(Canvas& canvas, const RGBColor& color) {
        /// Span analítico: a fronteira tem forma fechada x = cx ± sqrt(r²-dy²),
        /// então o par de interseções de cada scanline sai de uma conta,
        /// sem precisar de tabela de arestas.
        int r = static_cast<int>(std::floor(radius));
        for (int dy = -r; dy <= r; ++dy) {
            int hw = static_cast<int>(
                std::floor(std::sqrt(radius * radius - double(dy) * dy)));
            int y = center.y() + dy;
            for (int x = center.x() - hw; x <= center.x() + hw; ++x)
                canvas.add(Pixel(x, y), color);
        }
    }
} // namespace pet
