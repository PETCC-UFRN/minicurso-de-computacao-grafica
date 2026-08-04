#include "interpoledBackground.hpp"
#include "common.hpp"

namespace pet{

    InterpoledBackground::InterpoledBackground(const RGBColor color) : m_corners({color, color}) {};
    InterpoledBackground::InterpoledBackground(const std::array<RGBColor, 2> &colors) : m_corners(colors) {};
    
    // Sample and returns a color, based on the raster coordinate.
    RGBColor InterpoledBackground::sample(double u, double v) const {  
        const auto lerp =
            linear_interpolation(m_corners[0], m_corners[1], u);

         return lerp;
    };

    // Return the linearly interpolated color in [A;B], based on the parameter
    RGBColor InterpoledBackground::linear_interpolation(const RGBColor &A, const RGBColor &B,
                                          double t) {    
                                                    
        return RGBColor{static_cast<double>((1 - t) * A.red + t * B.red),
                        static_cast<double>((1 - t) * A.green + t * B.green),
                        static_cast<double>((1 - t) * A.blue + t * B.blue)};
            };

}