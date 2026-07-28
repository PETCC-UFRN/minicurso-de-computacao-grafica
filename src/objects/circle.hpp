#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "common.hpp"
#include "canvas.hpp"
#include "object.hpp"

namespace pet
{
    class Circle : public Object {
        protected:
            double radius; //> Raio da circunferência.
            Point2 center; //> Centro da circunferência.
        public:

            /**
             * @brief Construtor parametrizado
             * @param radius Raio da circunferência.
             * @param center Centro da circunferência.
             * @param scale Escalas x e y do objeto.
             * @param thick Grossura do objeto
             */
            Circle(double radius, Point2 center, Point2 scale = Point2(1, 1), double thick = 1) :
            Object(scale, thick), radius(radius), center(center) {};

            /**
             * @brief Função que desenha uma circunferência no Canvas
             * @param canvas Tela que será desenhada.
             * @param color Cor da circunferência.
             */
            void drawObject(Canvas& canvas, RGBColor color, DrawMethod method = DrawMethod::Bresenhan) override;
    };
} 

#endif 