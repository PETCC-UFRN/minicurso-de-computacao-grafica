/**
 * @file scanlineFill.cpp
 * @brief Scanline seed fill algorithm implementation
 * @author Generated
 * @date 2026-06-22
 */

#include "scanlineFill.hpp"
#include <stack>


namespace pet {
    void ScanlineFill::fill(Canvas& canvas, const Pixel& seed, const RGBColor& fillColor) {
        if (seed.x() < 0 || seed.x() >= canvas.width() ||
            seed.y() < 0 || seed.y() >= canvas.height()) {
            return;
        }

        auto originalColorOpt = canvas.pixel(seed);
        if (!originalColorOpt.has_value()) {
            return;
        }

        RGBColor originalColor = originalColorOpt.value();
        if (colorsMatch(originalColor, fillColor)) {
            return;
        }

        std::stack<Pixel> stack;
        stack.push(seed);

        while (!stack.empty()) {
            Pixel current = stack.top();
            stack.pop();

            int x = current.x();
            int y = current.y();

            // Move to left boundary of the current span
            int left = x;
            while (left >= 0) {
                Pixel p(left, y);
                auto colorOpt = canvas.pixel(p);
                if (!colorOpt.has_value() || !colorsMatch(colorOpt.value(), originalColor)) {
                    break;
                }
                left -= 1;
            }
            left += 1;

            // Move to right boundary of the current span
            int right = x;
            while (right < canvas.width()) {
                Pixel p(right, y);
                auto colorOpt = canvas.pixel(p);
                if (!colorOpt.has_value() || !colorsMatch(colorOpt.value(), originalColor)) {
                    break;
                }
                right += 1;
            }
            right -= 1;

            // Fill the span
            for (int fillX = left; fillX <= right; ++fillX) {
                canvas.add(Pixel(fillX, y), fillColor);
            }

            // Push the spans above and below the current line
            for (int nextY : {y - 1, y + 1}) {
                if (nextY < 0 || nextY >= canvas.height()) {
                    continue;
                }

                int scanX = left;
                while (scanX <= right) {
                    bool shouldFill = false;
                    while (scanX <= right) {
                        Pixel p(scanX, nextY);
                        auto colorOpt = canvas.pixel(p);
                        if (colorOpt.has_value() && colorsMatch(colorOpt.value(), originalColor)) {
                            shouldFill = true;
                            break;
                        }
                        ++scanX;
                    }

                    if (!shouldFill) {
                        break;
                    }

                    Pixel seedSpan(scanX, nextY);
                    stack.push(seedSpan);

                    while (scanX <= right) {
                        Pixel p(scanX, nextY);
                        auto colorOpt = canvas.pixel(p);
                        if (!colorOpt.has_value() || !colorsMatch(colorOpt.value(), originalColor)) {
                            break;
                        }
                        ++scanX;
                    }
                }
            }
        }
    }
}
