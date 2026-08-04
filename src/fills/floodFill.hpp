/**
 * @file floodFill.hpp
 * @brief FloodFill algorithm implementation
 * @author Generated
 * @date 2026-06-22
 */

#ifndef FLOOD_FILL_HPP
#define FLOOD_FILL_HPP

#include "fill.hpp"

namespace pet {
    /**
     * @class FloodFill
     * @brief Implements a FloodFill algorithm
     */
    class FloodFill : public Fill {
    public:
        /**
         * @brief Fill a region using the floodFill algorithm
         * @param canvas The canvas to fill
         * @param seed Starting pixel coordinates
         * @param fillColor Color to fill with
         */
        void fill(Canvas& canvas, const Pixel& seed, const RGBColor& fillColor) override;
    };
}
#endif
