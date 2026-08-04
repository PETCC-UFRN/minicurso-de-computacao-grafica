/**
 * @file line.hpp
 * @brief Implementação da classe que descreve o desenho de uma Reta na entidade Canvas.
 * @author Heitor Campos() 
 * @author Arthur Skeete()
 * @author Gabriel Carvalho()
 * @details Esta classe extende o conceito de Object (visto em object.hpp) e implementa o desenho de uma linha reta,
 * Armazenando os pontos de começo e fim e desenhando utilizando o algoritmo do Mid-Point.
 * @see https://www.geeksforgeeks.org/dsa/mid-point-line-generation-algorithm/ 
 * @date 25/05/2026()
 */

#ifndef LINE_HPP
#define LINE_HPP

#include "common.hpp"
#include "object.hpp"
#include <utility>

namespace pet{

            /**
              * @class Line
              * @brief Extensão da classe Object que implementa o desenho de um segmento de reta
              *
              */
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
              *
              */
            Line(Point2 start, Point2 end, Point2 scale = Point2(1, 1), double thick = 1) :
            Object(scale, thick), start(start), end(end) {};

            /**
              * @brief Função que desenha uma linha reta no Canvas
              * @param canvas Tela que será desenhada.
              * @param color Cor da reta.
              */
            void drawObject(Canvas& canvas, RGBColor color, DrawMethod method = DrawMethod::Bresenhan) override ;

		private:
			// TODO: missing 3 functions
    };

}; //> namespace pet


#endif //> LINE_HPP
