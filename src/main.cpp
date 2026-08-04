
#include "canvas.hpp"
#include "common.hpp"
#include "floodFill.hpp"
#include "parser.hpp"

#include <string>

using namespace pet;

void demo();

int main(int argc, char **argv) {
  // ITerm it(800, 400, "result.ppm");
  // it.run();

  const std::string arg = argv[1];

  if (arg == "demo") {
    demo();
    return 0;
  }

  render(argv[1]);
  return 0;
}

#include "background.hpp"
#include "circle.hpp"
#include "line.hpp"

void demo() {
  Canvas canvas(800, 400, "file.png", false, ImgType::PNG);
  Background bkg(RGBColor(255, 255, 255, "rgb"));
  for (int j = 0; j < 400; ++j)
    for (int i = 0; i < 800; ++i)
      canvas.add(Pixel(i, j), bkg.sample(double(i) / 800.0, double(j) / 400.0));

  Line line(Point2(400, 0), Point2(800, 400));
  Line line2(Point2(400, 400), Point2(600, 0));
  Circle circle(80, Point2(400, 200));
  Circle circle2(90, Point2(400, 200));

  line.drawObject(canvas, RGBColor(255, 255, 255, "rgb"),
                  DrawMethod::XiaolinWu);
  line2.drawObject(canvas, RGBColor(255, 255, 255, "rgb"),
                   DrawMethod::XiaolinWu);
  circle.drawObject(canvas, RGBColor(188, 0, 45, "rgb"));
  circle2.drawObject(canvas, RGBColor(0, 0, 0, "rgb"), DrawMethod::XiaolinWu);

  canvas.export_img();

} // namespace
