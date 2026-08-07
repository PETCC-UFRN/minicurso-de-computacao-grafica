#ifndef MEDIAN_FILTER_HPP
#define MEDIAN_FILTER_HPP

#include "canvas.hpp"
#include "common.hpp"
#include "filter.hpp"

namespace pet {
    class MedianFilter : public Filter {
        public:
            MedianFilter(int size) //< Construtor Parametrizado 
            : Filter(size) {};
        private:
            /**
              * @brief Função que aplica o filtro em um pixel
              * @param p Pixel que será aplicado 
              * @param canvas Canvas que do pixel
              */
            virtual RGBColor filterFunction(Pixel& p, Canvas& canvas) override;
    };
}


#endif