#include "line.hpp"
#include <cmath>

namespace pet
{
    void Line::drawObject(Canvas &canvas, RGBColor color, DrawMethod method) {
        int x = start.x();
        int y = start.y();
        int dx = end.x() - start.x(); //> Diferencial de x
        int dy = end.y() - start.y(); //> Diferencial de y
        int stepX = dx >= 0 ? 1 : -1; //> Sentido de x (1: direita, -1: esquerda)
        int stepY = dy >= 0 ? 1 : -1; //> Sentido de y (1: baixo, -1: cima)

        switch (method) {

            case DrawMethod::BresenhanMidpoint: {
            /* TODO */
            }

            case DrawMethod::Bresenhan: {
            /* TODO */
            }
        }
    }
}
