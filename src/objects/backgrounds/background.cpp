#include "background.hpp"
#include "common.hpp"

namespace pet
{
    Background::Background(RGBColor color) : color(color) {};
    RGBColor Background::sample(double, double) const
    {
        return color;
    }
}