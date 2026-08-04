#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "common.hpp"
#include "rectangle.hpp"

namespace pet {

            /**
              * @class Square
              * @brief Extensão da classe Rectangle para implementar um quadrado
              *
              */
			class Square : public Rectangle {
			public:
            /**
              * @brief Função construtora pública
              * @param top_left Ponto mais acima e a esquerda
              * @param size Tamanho do lado do quadrado
              *
              */
				static Square fromCorner(const Point2& top_left,
										  unsigned int size,
										  Point2 scale = Point2(1,1),
										  double thick = 1);

            /**
              * @brief Função construtora pública
              * @param center Coordenada do centro do quadrado
              * @param size Tamanho do lado do quadrado
              *
              */
				static Square fromCenter(const Point2& center,
										 unsigned int size,
										 Point2 scale = Point2(1,1),
										 double thick = 1);

			private:
            /**
              * @brief Construtor privado
              * @param top_left Ponto mais acima e a esquerda do retângulo
              * @param bottom_right Ponto mais abaixo e a direita do retângulo
              * @param scale Escalas x e y do objeto.
              * @param thick Grossura do objeto
              *
              */
				Square(const Point2& p1,
					   const Point2& p2,
					   Point2 scale,
					   double thick)
					: Rectangle(p1, p2, scale, thick) {}
			};
};


#endif //> SQUARE_HPP
