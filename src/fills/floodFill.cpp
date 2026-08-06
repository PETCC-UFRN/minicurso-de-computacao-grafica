#include "floodFill.hpp"
#include "common.hpp"
#include <cstdlib>
#include <stack>
namespace pet{
    
    void FloodFill::fill(Canvas& canvas, const Pixel& seed, const RGBColor& fillColor){
        auto og_color = canvas.pixel(seed);
        if(!og_color.has_value()){
            return;
        }
        if(colorsMatch(og_color.value(), fillColor)) return;

        std::stack<Pixel> visit_stack;

        canvas.add(seed, fillColor);
        visit_stack.push(seed);
        while(!visit_stack.empty()){
            Pixel to_visit = visit_stack.top();
            visit_stack.pop();

            for(int dy = -1; dy<=1; dy++){
                for(int dx = -1; dx<=1; dx++){
                    if(dy==0 && dx == 0) continue;
                    if(abs(dy)+abs(dx) == 2) continue; 
                    
                    int nx = to_visit.x() + dx;
                    int ny = to_visit.y() + dy;

                    if(nx < 0 || ny < 0 || nx >= canvas.width() || ny >= canvas.height()){
                        continue;
                    }
                    
                    if(colorsMatch(canvas.pixel({nx, ny}).value(), og_color.value())){
                        canvas.add(Pixel(nx, ny), fillColor);
                        visit_stack.emplace(nx, ny);
                    }
                }
            }
        }
    }
    
};