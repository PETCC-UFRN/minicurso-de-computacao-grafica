#ifndef AVERAGE_FILTER_HPP
#define AVERAGE_FILTER_HPP

#include "canvas.hpp"
#include "common.hpp"
#include "filter.hpp"

namespace pet {
    class AverageFilter : public Filter {
        private:
            /**
              * @brief Função que aplica o filtro em um pixel
              * @param p Pixel que será aplicado 
              * @param canvas Canvas que do pixel
              */
            virtual RGBColor filterFunction(Pixel& p, Canvas& canvas) override;
        public:

            AverageFilter(int size) : Filter(size) {}; //< Construtor Parametrizado
    };
}


#endif