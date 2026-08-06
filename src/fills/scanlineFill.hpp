/**
 * @file scanlineFill.hpp
 * @brief Scanline seed fill algorithm implementation
 * @author Generated
 * @date 2026-06-22
 */

#ifndef SCANLINE_FILL_HPP
#define SCANLINE_FILL_HPP

#include "fill.hpp"

namespace pet {
    /**
     * @class ScanlineFill
     * @brief Implements a scanline seed fill algorithm
     * @details Uses horizontal scanline spans to fill connected regions more efficiently
     * than a simple pixel-by-pixel flood fill.
     */
    class ScanlineFill : public Fill {
    public:
        /**
         * @brief Fill a region using the scanline seed fill algorithm
         * @param canvas The canvas to fill
         * @param seed Starting pixel coordinates
         * @param fillColor Color to fill with
         */
        void fill(Canvas& canvas, const Pixel& seed, const RGBColor& fillColor) override;

    };
}

#endif
