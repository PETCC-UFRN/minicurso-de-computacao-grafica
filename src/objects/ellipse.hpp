#ifndef ELLIPSE_HPP
#define ELLIPSE_HPP

#include "object.hpp"

namespace pet {

    /**
     * @class Ellipse
     * @brief Extensao da classe Object que implementa o desenho de uma elipse
     *
     */
    class Ellipse : public Object {
    protected:
        double radiusX;
        double radiusY;
        Point2 center;

    public:

        /**
          * @brief Construtor parametrizado
          * @param radiusX Raio horizontal da elipse.
          * @param radiusY Raio vertical da elipse.
          * @param center Centro da elipse.
          * @param scale Escalas x e y do objeto.
          * @param thick Grossura do objeto
          *
          */
        Ellipse(double radiusX, double radiusY, Point2 center,
                Point2 scale = Point2(1, 1), double thick = 1)
            : Object(scale, thick), radiusX(radiusX), radiusY(radiusY), center(center) {};

        /**
          * @brief Função que desenha uma elipse no Canvas
          * @param canvas Tela que será desenhada.
          * @param color Cor da elipse.
          */
        void drawObject(Canvas& canvas, RGBColor color,
                        DrawMethod method = DrawMethod::Bresenhan) override;

        bool hasInterior() const override { return true; }

        /**
          * @brief Preenche a elipse por spans analíticos: para cada scanline,
          * meia-largura = rx * sqrt(1 - (dy/ry)²).
          */
        void fillObject(Canvas& canvas, const RGBColor& color) override;

        /**
          * @brief Função que aplica uma matriz de rotação no objeto
          * @param transformation Transformação que será aplicada
          * @note Rotação nas nossas elipses não funcionaria.
          */
        void transform(const Transform& transformation) override;

        /**
          * @brief Função que retorna o eixo de transformação do objeto
          */
        Point2 getAxis() const override;
    };

};

#endif
