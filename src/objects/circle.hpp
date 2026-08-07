#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "object.hpp"
namespace pet {
  /**
    * @class Circle
    * @brief Extensão da classe Object que implementa o desenho de um Circle
    *
    */
    class Circle : public Object {
        protected:
            double radius;
            Point2 center;
        public:

            /**
              * @brief Construtor parametrizado
              * @param radius Raio da circunferência.
              * @param center Centro da circunferência.
              * @param scale Escalas x e y do objeto.
              * @param thick Grossura do objeto
              *
              */
            Circle(double radius, Point2 center, Point2 scale = Point2(1, 1), double thick = 1) :
            Object(scale, thick), radius(radius), center(center) {};

            /**
              * @brief Função que desenha uma circunferência no Canvas
              * @param canvas Tela que será desenhada.
              * @param color Cor da polirreta.
              */
            void drawObject(Canvas& canvas, RGBColor color, DrawMethod method = DrawMethod::Bresenhan) override ;

            bool hasInterior() const override { return true; }

            /**
              * @brief Preenche o círculo por spans analíticos: para cada
              * scanline, meia-largura = sqrt(r² - dy²).
              */
            void fillObject(Canvas& canvas, const RGBColor& color) override;

            /**
              * @brief Função que aplica uma matriz de rotação no objeto
              * @param transformation Transformação que será aplicada
              */
            void transform(const Transform& transformation) override;

            /**
              * @brief Função que retorna o eixo de transformação do objeto
              */
            Point2 getAxis() const override;
		private:
			void drawBresenhan(int radius, Canvas& canvas, RGBColor color);
			void drawXiaolinWu(int radius, Canvas& canvas, RGBColor color);
    };
};


#endif
