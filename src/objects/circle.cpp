#include "circle.hpp"

namespace pet{
        void Circle::drawObject(Canvas& canvas, RGBColor color, DrawMethod) {
        int x = 0;
        int y = radius;
        int d = /*TODO: valor inicial da decisão*/;

        while (y > x) { //> Percorremos um único octante — a simetria faz o resto!
            for (int i{-1}; i <= 1; i += 2) {
                for (int j{-1}; j <= 1; j += 2) {
                    canvas.add(Pixel(center.x() + x * i, center.y() + y * j), color);
                    canvas.add(Pixel(center.x() + y * i, center.y() + x * j), color);
                }
            }

            if (/*TODO: ainda estamos dentro?*/) {
                d += /* TODO */;
                /*TODO: passo E — atualize d e avance com x*/
            }
            else {
                d += /* TODO */;
                /*TODO: passo S — atualize d e desça com y*/
            }
        }
    }
}
