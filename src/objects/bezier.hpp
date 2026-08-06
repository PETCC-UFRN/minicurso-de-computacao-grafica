#ifndef BEZIER_HPP
#define BEZIER_HPP

#include "object.hpp"

namespace pet {
    class Bezier : public Object {
        private:
            std::vector<Point2> control_points; //> Pontos de controle da curva de Bezier.
        public:

            /**
              * @brief Construtor parametrizado
              * @param control_points Vetor de pontos de controle da curva de Bezier.
              * @param scale Escalas x e y do objeto.
              * @param thick Grossura do objeto
              *
              */
            Bezier(std::vector<Point2> control_points, Point2 scale = Point2(1, 1), double thick = 1) :
            Object(scale, thick), control_points(control_points) {};

            /**
              * @brief Função que desenha uma curva de Bezier no Canvas
              * @param canvas Tela que será desenhada.
              * @param color Cor da curva de Bezier.
              */
            void drawObject(Canvas& canvas, RGBColor color, DrawMethod method = DrawMethod::Bresenhan) override ;

            /**
              * @brief Função que aplica uma matriz de rotação no objeto 
              * @param transformation Transformação que será aplicada
              */
            void transform(const Transform& transformation) override;

            /**
              * @brief Função que retorna o eixo de transformação do objeto  
              */
            Point2 getAxis() const override;
  };
};
#endif