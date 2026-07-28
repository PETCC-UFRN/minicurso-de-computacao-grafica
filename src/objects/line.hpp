#ifndef LINE_HPP
#define LINE_HPP

#include "common.hpp"
#include "canvas.hpp"
#include "object.hpp"

namespace pet
{
    class Line : public Object {
        private:
            Point2 start; //> Ponto inicial da reta.
            Point2 end;   //> Ponto final da reta.
        public:

            /**
             * @brief Construtor parametrizado
             * @param start Ponto inicial da reta.
             * @param end Ponto final da reta.
             * @param scale Escalas x e y do objeto.
             * @param thick Grossura do objeto
             */
            Line(Point2 start, Point2 end, Point2 scale = Point2(1, 1), double thick = 1) :
            Object(scale, thick), start(start), end(end) {};

            /**
             * @brief Função que desenha uma linha reta no Canvas
             * @param canvas Tela que será desenhada.
             * @param color Cor da reta.
             */
            void drawObject(Canvas& canvas, RGBColor color, DrawMethod method = DrawMethod::Bresenhan) override;
    };
}

#endif 