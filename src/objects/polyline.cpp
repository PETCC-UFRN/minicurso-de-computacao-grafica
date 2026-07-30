#include "polyline.hpp"
#include "line.hpp"

namespace pet{
    void Polyline::drawObject(Canvas& canvas, RGBColor color, DrawMethod method) {
        for (size_t i{1}; i < points.size(); ++i) { //> Percorre os pares consecutivos
            /*TODO: crie uma Line de points[i - 1] até points[i]
                    (repassando scale, thick e method) e desenhe-a*/
        }
    }
}