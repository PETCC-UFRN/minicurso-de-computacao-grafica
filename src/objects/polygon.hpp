#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "polyline.hpp"

namespace pet {

            /**
              * @class Polygon
              * @brief Extensão da classe Polyline que implementa o desenho de um polígono genérico
              *
              */
    class Polygon : public Polyline {
        public:

            /**
              * @brief Construtor parametrizado
              * @param points Lista de pontos do polígono.
              * @param scale Escalas x e y do objeto.
              * @param thick Grossura do objeto
              *
              */
            Polygon(vector<Point2> points, Point2 scale = Point2(1, 1), double thick = 1)
            : Polyline(points, scale, thick)
            {
                this->points.push_back(points.front());
            };

            bool hasInterior() const override { return true; }

            /**
              * @brief Preenche o polígono pelo scanline geométrico de paridade
              * (Edge Table / Active Edge Table). Regra even-odd; não usa seed.
              */
            void fillObject(Canvas& canvas, const RGBColor& color) override;
    };
};


#endif //> POLYGON_HPP
