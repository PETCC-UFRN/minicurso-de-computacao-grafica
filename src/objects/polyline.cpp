#include "polyline.hpp"
#include "common.hpp"

namespace pet{

    void Polyline::drawObject(Canvas& canvas, RGBColor color, DrawMethod method) {
        for(size_t i{1}; i < points.size(); ++i){ //> Percorre todos os pontos da polirreta

            auto ini = points[i - 1];
            auto end = points[i];

            Line line(ini, end, scale, thick); //> Cria uma nova reta de ini para end.
            
            line.drawObject(canvas, color, method);
        }
    }


    void Polyline::transform(const Transform& transformation) 
    {
      /*TODO: Aplicar a transformação em todos os Pontos*/
    }

    Point2 Polyline::getAxis() const
    {
      /*TODO: Retornar o ponto de controle da Polyline */
      /*      Nesse caso, o ponto centroide (ou "ponto central do objeto")*/
    }
}
