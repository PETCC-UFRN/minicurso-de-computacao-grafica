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
		//TODO: missing
    }

    void Bezier::drawObject(Canvas& canvas, RGBColor color, DrawMethod method) {
		//TODO: missing
    };
    void Bezier::transform(const Transform& transformation) 
    {
		//TODO: missing
    }

    Point2 Bezier::getAxis() const
    {
		//TODO: missing
    }
};


