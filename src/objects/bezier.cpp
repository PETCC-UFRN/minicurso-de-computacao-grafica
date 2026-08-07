#include "bezier.hpp"
#include "common.hpp"
#include "line.hpp"

namespace pet {
    // Função auxiliar para calcular o fatorial de um número inteiro.
    int factorial(int n) {
        int result = 1;

        for (int i = 1; i <= n; i++) {
            result *= i;
        }

        return result;
    }
    // Função auxiliar para calcular o coeficiente binomial "n choose k".
    double binomial(int n, int k) {
        if (k < 0 || k > n) return 0.0;
        if (k > n - k) k = n - k;
        double res = 1.0;
        for (int i = 1; i <= k; ++i) {
            res *= (double)(n - k + i) / (double)i;
        }
        return res;
    }
    // Função auxiliar para calcular o valor da curva de Bezier em um dado t.
    float bezier2(const vector<float>& v, int n, float t) {
        float p = 0.0;
        int N = n - 1;
        for (int i = 0; i <= N; ++i) {
            double b = binomial(N, i) * pow(t, i) * pow(1.0 - t, N - i);
            p += b * v[i];
        }
        return p;
    }

    void Bezier::drawObject(Canvas& canvas, RGBColor color, DrawMethod method) {
        int n = control_points.size() - 1;
        vector<float> vx(n + 1);
        vector<float> vy(n + 1);
        for(int i{0}; i <= n; ++i){
            vx[i] = control_points[i].x();
            vy[i] = control_points[i].y();
        }
        int x = bezier2(vx, n + 1, 0.0);
        int y = bezier2(vy, n + 1, 0.0);
        Point2 p1(x, y);
        for(double t{0}; t <= 1; t += 0.01){
            x = bezier2(vx, n + 1, t);
            y = bezier2(vy, n + 1, t);
            // Pixel p(x, y);
            // canvas.add(p, color);
            Point2 p2(x, y);
            Line line(p1, p2);
            line.drawObject(canvas, color, method);
            p1 = p2;
        }
    };
    void Bezier::transform(const Transform& transformation) 
    {
        for (auto& p : control_points)
        {
            p = transformation * p;
        }
    }

    Point2 Bezier::getAxis() const
    {
        Point2 sum{0, 0};
        
        for(const auto& point : control_points)
        {
            sum += point;
        }

        return sum / control_points.size();

    }
};


