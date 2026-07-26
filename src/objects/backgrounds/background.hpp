#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "common.hpp"

namespace pet
{
    /**
      * @class Background
      * @brief Classe primitiva que representa o fundo de tela do Canvas
      *
      */
    class Background 
    {
      private:
        RGBColor color; //< Cor do Background padrão
      public:
        ~Background() = default; //< Destrutor Padrão
        Background() = default;  //< Construtor Padrão
        Background(RGBColor);    //< Construtor Parametrizado
        //static constexpr byte max_channel_value{255}; //< Valor máximo de cada entrada RGB
        
         /**
           * @brief Função responsável por retornar a cor do fundo de tela no pixel (u, v)
           * @param u coordenada X do pixel
           * @param v coordenada Y do pixel
           *
           */
        virtual RGBColor sample(double u, double v) const;
        
    };
}

#endif //< BACKGROUND_HPP