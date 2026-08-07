#ifndef FILTER_HPP
#define FILTER_HPP

#include "canvas.hpp"
#include "common.hpp"
#include <vector>

namespace pet {
    class Filter {
        protected:
            int kernel_size; //< Tamanho do Kernel do Filtro

            /**
              * @brief Função que aplica o filtro em um pixel
              * @param p Pixel que será aplicado 
              * @param canvas Canvas que do pixel
              */
            virtual RGBColor filterFunction(Pixel& p, Canvas& canvas) = 0;
        public:
            Filter(int size) : kernel_size(size) {}; //< Construtor Parametrizado  
            virtual ~Filter() = default;             //< Destrutor Padrão

            /**
              * @brief Função que aplica a função do filtro em cada pixel do canvas 
              * @param canvas Canvas em que o filtro será aplicado
              */
            virtual void applyFilter(Canvas& canvas);
    };
}


#endif