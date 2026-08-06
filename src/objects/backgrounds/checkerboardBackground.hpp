#ifndef CHECKERBOARD_BACKGROUND_HPP
#define CHECKERBOARD_BACKGROUND_HPP

#include "common.hpp"
#include "background.hpp"

namespace pet {

    /**
      * @class ChackerboardBackground
      * @brief Classe que extende o Background primitivo para um background xadrez
      *
      */
    class CheckerboardBackground : public Background {
        public:
            static constexpr byte max_channel_value{255};

            // receives the color of the "white" and "black" squares, and the size of each square
            CheckerboardBackground(const RGBColor color1, const RGBColor color2, double square_size = 0.1) 
                : m_color1(color1), m_color2(color2), m_square_size(square_size) {};

            /**
              * @brief Função responsável por retornar a cor do fundo de tela no pixel (u, v)
              * @param u coordenada X do pixel
              * @param v coordenada Y do pixel
              *
              */
            RGBColor sample(double u, double v) const;

        private:
            RGBColor m_color1;    //< Primeira cor do fundo xadrez
            RGBColor m_color2;    //< Segunda cor do fundo xadrez
            double m_square_size; //< Tamanho de cada quadrado do xadrez
    
    };

}

#endif //< CHECKERBOARD_BACKGROUND_HPP