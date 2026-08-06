#include "interpoledBackground.hpp"
#include "common.hpp"

namespace pet{

    InterpoledBackground::InterpoledBackground(const RGBColor color) : m_corners({color, color, color, color}) {};
    InterpoledBackground::InterpoledBackground(const std::array<RGBColor, 4> &colors) : m_corners(colors) {};
    
    // Sample and returns a color, based on the raster coordinate.
    RGBColor InterpoledBackground::sample(double u, double v) const {  
        const auto bottom_horizontal =
            linear_interpolation(m_corners[BOTTOM_LEFT], m_corners[BOTTOM_RIGHT], u);
        const auto top_horizontal =
            linear_interpolation(m_corners[TOP_LEFT], m_corners[TOP_RIGHT], u);

        const auto bilerp =
            linear_interpolation(top_horizontal, bottom_horizontal, v);

         return bilerp;
    };

    // Return the linearly interpolated color in [A;B], based on the parameter
    RGBColor InterpoledBackground::linear_interpolation(const RGBColor &A, const RGBColor &B,
                                          double t) {    
                                                    
        return RGBColor{static_cast<double>((1 - t) * A.red + t * B.red),
                        static_cast<double>((1 - t) * A.green + t * B.green),
                        static_cast<double>((1 - t) * A.blue + t * B.blue)};
            };

}