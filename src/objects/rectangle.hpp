#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "common.hpp"
#include "polygon.hpp"

namespace pet {

            /**
              * @class Rectangle
              * @brief Extensão da classe Polygon para implementar um retângulo
              *
              */
    class Rectangle : public Polygon  {
        public:

            /**
              * @brief Construtor parametrizado
              * @param top_left Ponto mais acima e a esquerda do retângulo
              * @param bottom_right Ponto mais abaixo e a direita do retângulo
              * @param scale Escalas x e y do objeto.
              * @param thick Grossura do objeto
              *
              */
            Rectangle(Point2 top_left, Point2 bottom_right, Point2 scale = Point2(1, 1), double thick = 1) 
            : Polygon({
					top_left,
					Point2(bottom_right.x(), top_left.y()),
					bottom_right,
					Point2(top_left.x(), bottom_right.y())
					},
					scale,
					thick
					) {};
    };
};


#endif //> RECTANGLE_HPP
