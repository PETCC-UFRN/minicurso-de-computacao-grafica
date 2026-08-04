#include "line.hpp"
#include "canvas.hpp"
#include "common.hpp"
#include "interpoledBackground.hpp"
#include <cstdlib>
#include <utility>

namespace pet {
void Line::drawObject(Canvas &canvas, RGBColor color, DrawMethod method) {
  Point2 p1 = start;
  Point2 p2 = end;

  switch (method) {
  case DrawMethod::Bresenhan: {
    // TODO: missing thickness
    int x0 = p1.x(), y0 = p1.y();
    int x1 = p2.x(), y1 = p2.y();
    bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);

	// ======
    // Swap axes if steep
    if (steep) {
      std::swap(x0, y0);
      std::swap(x1, y1);
    }

    // Ensure left->right
    if (x0 > x1) {
      std::swap(x0, x1);
      std::swap(y0, y1);
    }
	// ======

    int dx = x1 - x0;
    int dy = std::abs(y1 - y0);
    int error = dx / 2;
    int ystep = (y0 < y1) ? 1 : -1;
    int y = y0;

    for (int x = x0; x <= x1; x++) {
      if (steep) {
        canvas.add(Pixel(y, x), color);
      } else {
        canvas.add(Pixel(x, y), color);
      }
      error -= dy;
      if (error < 0) {
        y += ystep;
        error += dx;
      }
    }
    break;
  }

  case DrawMethod::BresenhanMidpoint: {
    // TODO: missing thickness
    int x = p1.x(); //> "Erro"
    int y = p1.y(); //> "Erro"

    int dx = p2.x() - p1.x(); //> Define o "diferencial" dos x
    int dy = p2.y() - p1.y(); //> Define o "diferencial" dos y

    int stepX = dx >= 0 ? 1 : -1; //> Define o passo que x irá percorrer (1:
                                  // direita, -1: esquerda)
    int stepY =
        dy >= 0
            ? 1
            : -1; //> Define o passo que y irá percorrer (1: baixo, -1: cima)
    dx = std::abs(dx);
    dy = std::abs(dy);

    canvas.add(Pixel(x, y), color);

    if (dy <= dx) {
      int d = 2 * dy - dx;

      int stepE = 2 * dy;         //> se leste for escolhido
      int stepNE = 2 * (dy - dx); //> se nordeste for escolhido

      while (x != p2.x()) {
        x += stepX;

        if (d < 0) { //> se leste for escolhido
          d += stepE;
        } else { //> se nordeste for escolhido
          d += stepNE;
          y += stepY;
        }
        canvas.add(Pixel(x, y), color);
      }
    }

    else {
      int d = 2 * dx - dy;

      int stepN = 2 * dx;
      int stepNE = 2 * (dx - dy);

      while (y != p2.y()) {
        y += stepY;

        if (d < 0) { //> se leste for escolhido
          d += stepN;
        } else { //> se nordeste for escolhido
          d += stepNE;
          x += stepX;
        }
        canvas.add(Pixel(x, y), color);
      }
    }

    break;
  }
    // TODO: XiaolinWu
  }
}
// TODO: missing Bresenhan
// TODO: missing Midpoint
// TODO: missing XiaolinWu

} // namespace pet
