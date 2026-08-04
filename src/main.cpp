
#include "background.hpp"
#include "canvas.hpp"
#include "circle.hpp"
#include "common.hpp"
#include "interpoledBackground.hpp"
#include "line.hpp"
#include "parser.hpp"
#include <sys/types.h>

using namespace pet;

int main(int argc, char **argv) {

  if (argc > 1) {
    render(argv[1]);
    return 0;
  }

  RGBColor color1(0, 255, 255, "rgb");
  RGBColor color2(0, 0, 255, "rgb");

  Canvas canvas(800, 400, "file.png", false, ImgType::PNG);
  InterpoledBackground bkg({color1, color2});
  for (int j{0}; j < 400; ++j) {
    for (int i{0}; i < 800; ++i) {
      auto color = bkg.sample(double(i) / double(800), double(j) / double(400));
      Pixel pixel(static_cast<int>(i), static_cast<int>(j));
      canvas.add(pixel, color);
    }
  }

  canvas.export_img();

  return 0;
}
