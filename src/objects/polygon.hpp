#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "canvas.hpp"
#include "common.hpp"
#include "object.hpp"
#include "polyline.hpp"

namespace pet {
class Polygon : public ? {
public:
  /**
   * @brief Construtor parametrizado
   * @param points Lista de pontos do polígono.
   * @param scale Escalas x e y do objeto.
   * @param thick Grossura do objeto
   */
  Polygon(vector<Point2> points, Point2 scale = Point2(1, 1), double thick = 1)
      : Polyline(points, scale, thick) {
          /*TODO: uma única linha de código "fecha" a polirreta. Qual?*/
        };
};
} // namespace pet

#endif
