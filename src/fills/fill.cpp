#include "fill.hpp"

namespace pet 
{
    bool Fill::colorsMatch(const RGBColor& color1, const RGBColor& color2, double tolerance) const {
        double rDiff = std::abs(color1.red - color2.red);
        double gDiff = std::abs(color1.green - color2.green);
        double bDiff = std::abs(color1.blue - color2.blue);

        return (rDiff <= tolerance && gDiff <= tolerance && bDiff <= tolerance);
    }

}