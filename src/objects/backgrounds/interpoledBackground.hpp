#ifndef INTERPOLED_BACKGROUND_HPP
#define INTERPOLED_BACKGROUND_HPP

#include <array>
#include <initializer_list>

#include "background.hpp"
#include "common.hpp"
// #include "paramset.hpp"

namespace pet {

class InterpoledBackground : public Background 
{
  private:
    array<RGBColor, 2> m_corners{RGBColor(), RGBColor()}; //< Array que armazena a cor das 2 bordas do Background
  public:

    InterpoledBackground(const RGBColor color);                  //< Construtor Parametrizado com 1 cor
    InterpoledBackground(const std::array<RGBColor, 2> &colors); //< Construtor Parametrizado com 2 cores
    ~InterpoledBackground() = default;                           //< Destrutor Padrão
    
    /**
      * @brief Função responsável por retornar a cor do fundo de tela no pixel (u, v)
      * @param u coordenada X do pixel
      * @param v coordenada Y do pixel
      *
      */
    RGBColor sample(double, double) const override;
    
    /**
      * @brief Função responsável por retornar a interpolação linear entre duas cores [A, B] dado um parâmetro t
      * @param A Uma das cores que será a interpolada 
      * @param B Uma das cores que será interpolada
      * @param t Parâmetro responsável por definir o tempo da interpolação
      *
      */
    static RGBColor linear_interpolation(const RGBColor &A, const RGBColor &B,
                                        double t);


  };

} // namespace pet

#endif // INTERPOLED_BACKGROUND_HPP
