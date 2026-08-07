#ifndef CUSTOM_FILTER_HPP
#define CUSTOM_FILTER_HPP

#include "canvas.hpp"
#include "common.hpp"
#include "filter.hpp"

namespace pet {
    class CustomFilter : public Filter {
        private:
            vector<vector<double>> filtro; //< Matriz responsável pelo Filtro

            /**
              * @brief Função que aplica o filtro em um pixel
              * @param p Pixel que será aplicado 
              * @param canvas Canvas que do pixel
              */
            virtual RGBColor filterFunction(Pixel& p, Canvas& canvas) override;
            
        public:
            CustomFilter(int size, vector<vector<double>> filtro) //< Construtor Parametrizado
            : Filter(size), filtro(filtro) {}; 

    };
}


#endif