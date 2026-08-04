/**
 * @file polygon.cpp
 * @brief Preenchimento geométrico por paridade (Edge Table / Active Edge Table)
 * @details Diferente do seed fill, este algoritmo deriva os spans dos VÉRTICES:
 * não usa seed, não lê o framebuffer, e por isso o preenchimento acompanha
 * qualquer transformação aplicada à geometria. Regra de paridade even-odd.
 */

#include "polygon.hpp"

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

namespace pet {

void Polygon::fillObject(Canvas &canvas, const RGBColor &color) {
    /// points já vem fechado pelo construtor: [p0, p1, ..., pn, p0].
    if (points.size() < 4) /// 3 vértices + fechamento; menos que isso não tem área.
        return;

    /// Uma aresta ativa: até que linha vive, onde cruza a scanline atual,
    /// e quanto o cruzamento anda por linha (1/m — incremento DDA).
    struct Edge {
        int yMax;
        double x;
        double invSlope;
    };

    /// 1) Constrói a lista de arestas, descartando horizontais.
    ///    Convenção CRUCIAL: cada aresta cobre [yMin, yMax) — o yMax é
    ///    exclusivo. É isso que faz um vértice compartilhado contar UMA vez
    ///    e mantém a paridade correta (o bug clássico do algoritmo).
    std::vector<std::pair<int, Edge>> edges; /// (yMin, aresta)
    int yStart = canvas.height(), yEnd = 0;

    for (std::size_t i = 0; i + 1 < points.size(); ++i) {
        Point2 a = points[i];
        Point2 b = points[i + 1];

        if (a.y() == b.y())
            continue; /// Horizontal: sem interseção útil; o contorno a cobre.

        if (a.y() > b.y())
            std::swap(a, b); /// a = extremo de menor y (topo na tela).

        Edge e{b.y(), double(a.x()),
               double(b.x() - a.x()) / double(b.y() - a.y())};
        edges.emplace_back(a.y(), e);

        yStart = std::min(yStart, a.y());
        yEnd = std::max(yEnd, b.y());
    }

    if (edges.empty())
        return;

    /// 2) Edge Table: bucket sort das arestas pelo yMin. Custa O(E) e faz a
    ///    varredura só "acordar" cada aresta na linha em que ela nasce.
    std::vector<std::vector<Edge>> ET(yEnd - yStart + 1);
    for (auto &[yMin, e] : edges)
        ET[yMin - yStart].push_back(e);

    /// 3) Varredura: a AET é o working set — só as arestas que cruzam a
    ///    scanline atual. Entra quem nasce, sai quem morre, ordena por x,
    ///    preenche entre pares (paridade), e avança x incrementalmente.
    std::vector<Edge> AET;
    for (int y = yStart; y < yEnd; ++y) {
        for (auto &e : ET[y - yStart]) /// nasce em y
            AET.push_back(e);

        AET.erase(std::remove_if(AET.begin(), AET.end(),
                                 [y](const Edge &e) { return e.yMax == y; }),
                  AET.end()); /// morre em y (regra [yMin, yMax))

        std::sort(AET.begin(), AET.end(),
                  [](const Edge &l, const Edge &r) { return l.x < r.x; });

        for (std::size_t i = 0; i + 1 < AET.size(); i += 2) {
            int xL = static_cast<int>(std::ceil(AET[i].x));
            int xR = static_cast<int>(std::floor(AET[i + 1].x));
            for (int x = xL; x <= xR; ++x)
                canvas.add(Pixel(x, y), color);
        }

        for (auto &e : AET)
            e.x += e.invSlope; /// DDA: acumula em vez de recomputar a reta.
    }
}

} // namespace pet
