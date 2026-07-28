#ifndef POLYLINE_HPP
#define POLYLINE_HPP

#include "common.hpp"
#include "canvas.hpp"
#include "object.hpp"

namespace pet{
    class Polyline : public Object {
        protected:
            vector<Point2> points; //> Lista de pontos da polirreta
        public:

            /**
             * @brief Construtor parametrizado
             * @param points Lista de pontos da polirreta.
             * @param scale Escalas x e y do objeto.
             * @param thick Grossura do objeto
             */
            Polyline(vector<Point2> points, Point2 scale = Point2(1, 1), double thick = 1)
            : Object(scale, thick), points(points) {};

            /**
             * @brief Função que desenha uma polirreta no Canvas
             * @param canvas Tela que será desenhada.
             * @param color Cor da polirreta.
             */
            void drawObject(Canvas& canvas, RGBColor color, DrawMethod method = DrawMethod::Bresenhan) override;
    };
}

#endif 