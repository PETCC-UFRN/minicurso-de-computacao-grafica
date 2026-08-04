#include "circle.hpp"
#include "common.hpp"
#include "interpoledBackground.hpp"
#include "object.hpp"

#include <cmath>

namespace pet {
    void Circle::drawObject(Canvas& canvas, RGBColor color, DrawMethod method) {
        int x = 0;
        int y = radius;
        int d = 1 - radius;

        Pixel p(x, y);
        canvas.add(p, color);
        while(y > x){
            for(int i{-1}; i <= 1; i += 2){
                for(int j {-1}; j <= 1; j += 2){
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

	// TODO: missing Bresenhan
	
	// TODO: missing XiaolinWu

    void Circle::fillObject(Canvas& canvas, const RGBColor& color) {
		// TODO: missing
    }
} // namespace pet
