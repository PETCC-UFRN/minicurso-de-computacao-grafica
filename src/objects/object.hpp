#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "common.hpp"
#include "canvas.hpp"

namespace pet
{    
    enum class DrawMethod {
      Bresenhan = 0,
      BresenhanMidpoint,
    };

    class Object {
        protected:
            Point2 scale; //> Escala dos valores x e y do objeto
            double thick; //> Grossura das linhas do objeto que será desenhado
        public:

              /**
              * @brief Construtor parametrizado
              * @param scale Escalas x e y do objeto.
              * @param thick Grossura do objeto
              *
              */
            Object(Point2 scale = Point2(1, 1), double thick = 1) : scale(scale), thick(thick) {};

              /**
              * @brief Destrutor Padrão
              */
            virtual ~Object() = default;

              /**
              * @brief Função que desenha o objeto no Canvas
              * @param canvas Tela que será desenhada.
              * @param color Cor do objeto.
              */
            virtual void drawObject(Canvas& canvas, RGBColor color, DrawMethod method = DrawMethod::Bresenhan) = 0;
    };
}

#endif