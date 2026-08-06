#include "checkerboardBackground.hpp"
#include <cmath>

namespace pet {

    RGBColor CheckerboardBackground::sample(double u, double v)  const {

        int col =  static_cast<int>(std::floor(u / m_square_size));
        int row = static_cast<int>(std::floor(v/m_square_size));


        //checkerboard pattern main logic
        if ((row + col) % 2 == 0){
            return m_color1;
        } else {
            return m_color2;
        }
    }
}