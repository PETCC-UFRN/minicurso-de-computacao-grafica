/**
 * @file fill.hpp
 * @brief Base class for fill algorithms
 * @author Generated
 * @date 2026-06-22
 */

#ifndef FILL_HPP
#define FILL_HPP

#include "canvas.hpp"
#include "common.hpp"

namespace pet {
    /**
     * @class Fill
     * @brief Abstract base class for fill algorithms
     * @details Defines the interface for different fill algorithms used to fill
     * closed regions in a canvas with a specific color.
     */
    class Fill {
    public:
        virtual ~Fill() = default;
        
        /**
         * @brief Fill a region starting from a seed point
         * @param canvas The canvas to fill
         * @param seed Starting pixel coordinates
         * @param fillColor Color to fill with
         */
        virtual void fill(Canvas& canvas, const Pixel& seed, const RGBColor& fillColor) = 0;

    protected:

        /**
          * @brief Function that checks if a pixel has a color similar to that of another 
          * @param color1 Color to be compared
          * @param color2 Color to be compared
          * @param tolerance Error rate
          */
        virtual bool colorsMatch(const RGBColor& color1, const RGBColor& color2, double tolerance = 0.0) const;

    };
}

#endif
