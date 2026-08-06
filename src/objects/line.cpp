#include "line.hpp"
#include "canvas.hpp"
#include "common.hpp"
#include "interpoledBackground.hpp"
#include <cstdlib>
#include <utility>

namespace pet {
void Line::drawObject(Canvas &canvas, RGBColor color, DrawMethod method) {
  switch (method) {
  case DrawMethod::Bresenhan: {
    int half = thick / 2;

    // desenha a linha original
    drawBresenhan(start, end, canvas, color);

    // desenha linhas paralelas
    for (int t = 1; t <= half; t++) {

      // para cima
      drawBresenhan(Point2(start.x(), start.y() - t),
                    Point2(end.x(), end.y() - t), canvas, color);

      // para baixo
      drawBresenhan(Point2(start.x(), start.y() + t),
                    Point2(end.x(), end.y() + t), canvas, color);
    }

    break;
  }

  case DrawMethod::BresenhanMidpoint: {
    int half = thick / 2;

    // desenha a linha original
    drawMidpoint(start, end, canvas, color);

    // desenha linhas paralelas
    for (int t = 1; t <= half; t++) {

      // para cima
      drawMidpoint(Point2(start.x(), start.y() - t),
                   Point2(end.x(), end.y() - t), canvas, color);

      // para baixo
      drawMidpoint(Point2(start.x(), start.y() + t),
                   Point2(end.x(), end.y() + t), canvas, color);
    }

    break;
  }
  case DrawMethod::XiaolinWu: {
    drawXiaolinWu(start, end, canvas, color);

    break;
  }
  }
}

void Line::drawBresenhan(Point2 p1, Point2 p2, Canvas &canvas, RGBColor color) {
  int x0 = p1.x(), y0 = p1.y();
  int x1 = p2.x(), y1 = p2.y();
  bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);

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
}

void Line::drawMidpoint(Point2 p1, Point2 p2, Canvas &canvas, RGBColor color) {
  int x = p1.x(); //> "Erro"
  int y = p1.y(); //> "Erro"

  int dx = p2.x() - p1.x(); //> Define o "diferencial" dos x
  int dy = p2.y() - p1.y(); //> Define o "diferencial" dos y

  int stepX = dx >= 0 ? 1 : -1; //> Define o passo que x irá percorrer (1:
                                // direita, -1: esquerda)
  int stepY =
      dy >= 0 ? 1
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
}

void Line::drawXiaolinWu(Point2 p1, Point2 p2, Canvas &canvas, RGBColor color) {
  auto plot = [&](int px, int py, double intensity) {
    intensity = std::clamp(intensity, 0.0, 1.0);

    auto pixel_opt = canvas.pixel(Pixel(px, py));
    if (!pixel_opt)
      return;

    RGBColor blended = InterpoledBackground::linear_interpolation(
        pixel_opt.value(), color, intensity);

    canvas.add(Pixel(px, py), blended);
  };

  bool steep = std::abs(p2.y() - p1.y()) > std::abs(p2.x() - p1.x());

  double x0 = p1.x();
  double y0 = p1.y();
  double x1 = p2.x();
  double y1 = p2.y();

  if (steep) {
    std::swap(x0, y0);
    std::swap(x1, y1);
  }

  if (x0 > x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }

  double dx = x1 - x0;
  double dy = y1 - y0;
  double gradient = (dx == 0) ? 0.0 : dy / dx;

  // Primeiro endpoint
  int xEnd = std::round(x0);
  double yEnd = y0 + gradient * (xEnd - x0);
  double xGap = 1.0 - std::fmod(x0 + 0.5, 1.0);

  int xPixel1 = xEnd;
  int yPixel1 = static_cast<int>(std::floor(yEnd));

  if (steep) {
    plot(yPixel1, xPixel1, (1 - (yEnd - yPixel1)) * xGap);
    plot(yPixel1 + 1, xPixel1, (yEnd - yPixel1) * xGap);
  } else {
    plot(xPixel1, yPixel1, (1 - (yEnd - yPixel1)) * xGap);
    plot(xPixel1, yPixel1 + 1, (yEnd - yPixel1) * xGap);
  }

  double intery = yEnd + gradient;

  // Segundo endpoint
  xEnd = std::round(x1);
  yEnd = y1 + gradient * (xEnd - x1);
  xGap = std::fmod(x1 + 0.5, 1.0);

  int xPixel2 = xEnd;
  int yPixel2 = static_cast<int>(std::floor(yEnd));

  if (steep) {
    plot(yPixel2, xPixel2, (1 - (yEnd - yPixel2)) * xGap);
    plot(yPixel2 + 1, xPixel2, (yEnd - yPixel2) * xGap);
  } else {
    plot(xPixel2, yPixel2, (1 - (yEnd - yPixel2)) * xGap);
    plot(xPixel2, yPixel2 + 1, (yEnd - yPixel2) * xGap);
  }

  // Corpo principal
  for (int x = xPixel1 + 1; x < xPixel2; x++) {
    int y = static_cast<int>(std::floor(intery));
    double frac = intery - y;

    if (steep) {
      plot(y, x, 1.0 - frac);
      plot(y + 1, x, frac);
    } else {
      plot(x, y, 1.0 - frac);
      plot(x, y + 1, frac);
    }

    intery += gradient;
  }
}

void Line::transform(const Transform &transformation) {
		//TODO: missing
}

Point2 Line::getAxis() const {
		//TODO: missing
}

} // namespace pet
