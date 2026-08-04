#include "line.hpp"
#include <cmath>

namespace pet
{
    void Line::drawObject(Canvas &canvas, RGBColor color, DrawMethod method) {
        int x = start.x();
        int y = start.y();

        switch (method) {

            case DrawMethod::Bresenhan: {
            /* TODO */
            }

            case DrawMethod::BresenhanMidpoint: {
            /* TODO */
            }

        }
    }
}
