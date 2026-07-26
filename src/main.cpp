
#include "background.hpp"
#include "interpoledBackground.hpp"
// #include "circle.hpp"
// #include "ellipse.hpp"
#include "common.hpp"
#include "canvas.hpp"
// #include "line.hpp"
// #include "bezier.hpp"
// #include "fills/scanlineFill.hpp"
#include <sys/types.h>


using namespace pet;

int main() {

    RGBColor color1(0, 255, 255, "rgb");
    RGBColor color2(0, 0, 255, "rgb");

    Canvas canvas(800, 400, "file.png", false, ImgType::PNG);
    InterpoledBackground bkg({color1, color2});
    for(int j{0}; j < 400; ++j){
        for(int i{0}; i < 800; ++i){
            auto color = bkg.sample(double(i)/double(800), double(j)/double(400));
            Pixel pixel(static_cast<int>(i), static_cast<int>(j));
            canvas.add(pixel, color);
        }
    }
    // Line line(Point2(400, 0), Point2(800, 400));
    // Line line2(Point2(400, 400), Point2(600, 0));

    // Circle circle(80, Point2(400,200));

    // Bezier bezier({Point2(0, 0), Point2(400, 0), Point2(400, 400), Point2(0, 400)});
    // Bezier bezier2({Point2(40, 200), Point2(100, 300), Point2(160, 100), Point2(220, 200)});

    // line.drawObject(canvas, RGBColor(255, 255, 255, "rgb"), DrawMethod::XiaolinWu);
    // line2.drawObject(canvas, RGBColor(255, 255, 255, "rgb"), DrawMethod::XiaolinWu);
    // circle.drawObject(canvas, RGBColor(188, 0, 45, "rgb"), DrawMethod::XiaolinWu);

    // pet::ScanlineFill scanlineFill;
    // scanlineFill.fill(canvas, Pixel(400, 200), RGBColor(188, 0, 45, "rgb"));

    // bezier.drawObject(canvas, RGBColor(255, 255, 255, "rgb"));
    // bezier2.drawObject(canvas, RGBColor(255, 255, 255, "rgb"), DrawMethod::XiaolinWu);

    canvas.export_img();

    return 0;
}
