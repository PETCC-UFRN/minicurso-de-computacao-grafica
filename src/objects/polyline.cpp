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
        for (auto& p : this->points)
        {
            p = transformation * p;
        }
    }

    Point2 Polyline::getAxis() const
    {
        Point2 sum{0, 0};
        
        for(const auto& point : points)
        {
            sum += point;
        }

        return sum / points.size();

    }
}
