#include "iterm.hpp"
#include "background.hpp"
#include "circle.hpp"
#include "common.hpp"
#include "interpoledBackground.hpp"
#include "line.hpp"
#include "polygon.hpp"
#include "square.hpp"
#include "rectangle.hpp"

#include "fills/fill.hpp"
#include "fills/floodFill.hpp"

#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <vector>

using namespace pet;

constexpr std::string_view CURSOR = " ➜ ";

/// Função genérica para extração de tokens
template <typename T>
[[nodiscard]] static std::optional<T> readNext(std::istringstream &iss,
                                               std::string_view errMsg) {
  T value;
  if (iss >> value)
    return value;

  std::cout << errMsg << '\n';
  return std::nullopt;
}

static std::optional<Point2> getPoint(std::istringstream &iss) {
  auto x = readNext<int>(iss, "  [LOG] Erro ao ler coordenada x.");
  if (!x.has_value())
    return std::nullopt;

  auto y = readNext<int>(iss, "  [LOG] Erro ao ler coordenada y.");
  if (!y.has_value())
    return std::nullopt;

  return std::make_optional<Point2>(x.value(), y.value());
}

/// Lê um canal de cor e valida o intervalo [0, 255].
static std::optional<double> readChannel(std::istringstream &iss,
                                         std::string_view name) {
  auto v = readNext<double>(iss, "  Erro ao ler canal.");
  if (!v.has_value())
    return std::nullopt;

  if (v.value() < 0 || v.value() > 255) {
    std::cout << "  [LOG] Canal " << name << " fora do intervalo [0, 255].\n";
    return std::nullopt;
  }
  return v;
}

/// Lê uma cor "R G B" (0-255) de uma stream e normaliza para [0, 1].
static std::optional<RGBColor> readColor(std::istringstream &iss) {
  auto r = readChannel(iss, "R");
  if (!r.has_value())
    return std::nullopt;

  auto g = readChannel(iss, "G");
  if (!g.has_value())
    return std::nullopt;

  auto b = readChannel(iss, "B");
  if (!b.has_value())
    return std::nullopt;

  /// O construtor com "rgb" divide por 255 (o buffer trabalha em [0, 1]).
  return RGBColor(r.value(), g.value(), b.value(), "rgb");
}

/// Prompt de cor em linha própria, no mesmo estilo dos create*.
static std::optional<RGBColor> promptColor() {
  std::cout << "Format: <R> <G> <B>  (0-255)\n\n  └ ";
  std::string line;
  std::getline(std::cin, line);
  std::istringstream iss{line};
  return readColor(iss);
}

void ITerm::setupCommands() {
  m_commandMap["create"] = &ITerm::create;
  m_commandMap["c"] = &ITerm::create;
  m_commandMap["help"] = &ITerm::help;
  m_commandMap["h"] = &ITerm::help;
  m_commandMap["exit"] = &ITerm::exit;
  m_commandMap["e"] = &ITerm::exit;
  m_commandMap["rollback"] = &ITerm::rollback;
  m_commandMap["r"] = &ITerm::rollback;
  m_commandMap["scanline"] = &ITerm::scanLineFill;
  m_commandMap["sl"] = &ITerm::scanLineFill;
  m_commandMap["flood"] = &ITerm::floodFill;
  m_commandMap["ff"] = &ITerm::floodFill;
  m_commandMap["background"] = &ITerm::background;
  m_commandMap["bg"] = &ITerm::background;
  m_commandMap["fill"] = &ITerm::fillShape;

  /// Funções create
  m_createMap["circle"] = &ITerm::createCircle;
  m_createMap["c"] = &ITerm::createCircle;
  m_createMap["circleAA"] = &ITerm::createAACircle;
  m_createMap["cAA"] = &ITerm::createAACircle;
  m_createMap["line"] = &ITerm::createLine;
  m_createMap["l"] = &ITerm::createLine;
  m_createMap["lineAA"] = &ITerm::createAALine;
  m_createMap["lAA"] = &ITerm::createAALine;
  m_createMap["ellipse"] = &ITerm::createEllipse;
  m_createMap["e"] = &ITerm::createEllipse;
  m_createMap["polygon"] = &ITerm::createPolygon;
  m_createMap["po"] = &ITerm::createPolygon;
  m_createMap["polyline"] = &ITerm::createPolyline;
  m_createMap["pe"] = &ITerm::createPolyline;
  m_createMap["rectangle"] = &ITerm::createRectangle;
  m_createMap["re"] = &ITerm::createRectangle;
  m_createMap["square"] = &ITerm::createSquare;
  m_createMap["sq"] = &ITerm::createSquare;
  m_createMap["squareCenter"] = &ITerm::createSquareCenter;
  m_createMap["sqc"] = &ITerm::createSquareCenter;
}

void ITerm::initMsg() const {
  std::string_view msg = R"(
    Welcome to the PEinT 2d Rasterizer©.

    Done by:
    Arthur Skeete,
	André Gomes,
	Alexandre Ribeiro,
	Carolina Carvalho,
	Gabriel Carvalho, 
	Heitor Campos, 
	Pedro Lucas,
	Yuri Santos

    Type 'help' ou 'h' for available commands.
    Type 'exit' to quit.
)";
  std::cout << msg << '\n';
}

void ITerm::fillCanvas(RGBColor color) {
  auto canvas = m_canvas.top().get();

  auto h = canvas->height();
  auto w = canvas->width();

  Background b(color);
  for (int j{0}; j < h; ++j) {
    for (int i{0}; i < w; ++i) {
      auto color =
          b.sample(static_cast<double>(i) / w, static_cast<double>(j) / h);
      Pixel pixel(i, j);
      canvas->add(pixel, color);
    }
  }
}

void ITerm::fillCanvas(const std::array<RGBColor, 4> &c) {
  auto &canvas = *m_canvas.top(); /// Referência. Copiar tornava isso um no-op.

  auto h = canvas.height();
  auto w = canvas.width();

  InterpoledBackground b(c);

  for (int j{0}; j < h; ++j) {
    for (int i{0}; i < w; ++i) {
      auto color =
          b.sample(static_cast<double>(i) / w, static_cast<double>(j) / h);
      Pixel pixel(i, j);
      canvas.add(pixel, color);
    }
  }
}

void ITerm::createCircle(std::string name) {
  std::cout << "Format: <raio> <centro.x centro.y>\n\n  └ ";
  std::string line;
  std::getline(std::cin, line);
  std::istringstream iss{line};

  auto r = readNext<double>(iss, "Erro ao ler raio.");
  auto c = getPoint(iss);

  if (!r.has_value() || !c.has_value())
    return;

  if (m_shapes.find(name) != m_shapes.end())
    std::cout << "  [LOG] Circle '" << name
              << "' já existe. Sobrescrevendo...\n";

  auto circle = std::make_unique<Circle>(r.value(), c.value());

  auto new_canvas = std::make_unique<Canvas>(*m_canvas.top());

  circle->drawObject(*new_canvas, {0, 0, 0});

  m_canvas.push(std::move(new_canvas));

  storeShape(std::move(name), std::move(circle)); /// Eficiencia extrema pai
}

void ITerm::createAACircle(std::string name) {
  std::cout << "Format: <raio> <centro.x centro.y>\n\n  └ ";
  std::string line;
  std::getline(std::cin, line);
  std::istringstream iss{line};

  auto r = readNext<double>(iss, "Erro ao ler raio.");
  auto c = getPoint(iss);

  if (!r.has_value() || !c.has_value())
    return;

  if (m_shapes.find(name) != m_shapes.end())
    std::cout << "  [LOG] Circle '" << name
              << "' já existe. Sobrescrevendo...\n";

  auto circle = std::make_unique<Circle>(r.value(), c.value());

  auto new_canvas = std::make_unique<Canvas>(*m_canvas.top());

  circle->drawObject(*new_canvas, {0, 0, 0}, DrawMethod::XiaolinWu);

  m_canvas.push(std::move(new_canvas));

  storeShape(std::move(name), std::move(circle)); /// Eficiencia extrema pai
}

void ITerm::createLine(std::string name) {
  std::cout << "Format: <p1.x p1.y> <p2.x p2.y>\n\n  └ ";
  std::string line;
  std::getline(std::cin, line);
  std::istringstream iss{line};

  auto p1 = getPoint(iss);

  if (!p1.has_value()) {
    std::cout << "  [LOG] Erro ao ler P1.\n";
    return;
  }

  auto p2 = getPoint(iss);

  if (!p2.has_value()) {
    std::cout << "  [LOG] Erro ao ler P2.\n";
    return;
  }

  if (m_shapes.find(name) != m_shapes.end())
    std::cout << "  [LOG] Line '" << name << "' já existe. Sobrescrevendo...\n";

  auto lin = std::make_unique<Line>(p1.value(), p2.value());

  auto new_canvas = std::make_unique<Canvas>(*m_canvas.top());

  lin->drawObject(*new_canvas, {0, 0, 0});

  m_canvas.push(std::move(new_canvas));

  storeShape(std::move(name), std::move(lin));
}

void ITerm::createAALine(std::string name) {
  std::cout << "Format: <p1.x p1.y> <p2.x p2.y>\n\n  └ ";
  std::string line;
  std::getline(std::cin, line);
  std::istringstream iss{line};

  auto p1 = getPoint(iss);

  if (!p1.has_value()) {
    std::cout << "  [LOG] Erro ao ler P1.\n";
    return;
  }

  auto p2 = getPoint(iss);

  if (!p2.has_value()) {
    std::cout << "  [LOG] Erro ao ler P2.\n";
    return;
  }

  if (m_shapes.find(name) != m_shapes.end())
    std::cout << "  [LOG] Line '" << name << "' já existe. Sobrescrevendo...\n";

  auto lin = std::make_unique<Line>(p1.value(), p2.value());

  auto new_canvas = std::make_unique<Canvas>(*m_canvas.top());

  lin->drawObject(*new_canvas, {0, 0, 0}, DrawMethod::XiaolinWu);

  m_canvas.push(std::move(new_canvas));

  storeShape(std::move(name), std::move(lin));
}

void ITerm::createRectangle(std::string name) {
  std::cout << "Format: <p1.x> <p1.y> , <p2.x p2.y>\n\n  └ ";

  std::string line;
  std::getline(std::cin, line);
  std::istringstream iss{line};

    auto p1 = getPoint(iss);

    if (!p1.has_value()) {
      std::cout << "  [LOG] Erro ao ler p1." << std::endl;
      return;
	}

    auto p2 = getPoint(iss);

    if (!p2.has_value()) {
      std::cout << "  [LOG] Erro ao ler p1." << std::endl;
      return;
	}

  auto rectangle = std::make_unique<Rectangle>(p1.value(),p2.value());

  auto new_canvas = std::make_unique<Canvas>(*m_canvas.top());

  rectangle->drawObject(*new_canvas, {0, 0, 0});

  m_canvas.push(std::move(new_canvas));

  storeShape(std::move(name), std::move(rectangle));
}

void ITerm::createSquare(std::string name) {
  std::cout << "Format: <size> <top_left.x top_left.y> \n\n  └ ";
  std::string line;
  std::getline(std::cin, line);
  std::istringstream iss{line};

  auto size = readNext<double>(iss, "Erro ao ler size.");
  auto top_left = getPoint(iss);

  if (!size.has_value() || !size.has_value())
    return;

  if (!top_left.has_value()) {
    std::cout << "  [LOG] Erro ao ler top_left.\n";
    return;
  }

  cout << size.value() << endl;
  cout << top_left.value().x() << " " << top_left.value().y() << endl;

  if (m_shapes.find(name) != m_shapes.end())
    std::cout << "  [LOG] Line '" << name << "' já existe. Sobrescrevendo...\n";

  auto square = std::make_unique<Square>(Square::fromCorner(top_left.value(), size.value()));

  auto new_canvas = std::make_unique<Canvas>(*m_canvas.top());

  square->drawObject(*new_canvas, {0, 0, 0});

  m_canvas.push(std::move(new_canvas));

  storeShape(std::move(name), std::move(square));
}

void ITerm::createSquareCenter(std::string name) {
  std::cout << "Format: <size> <center.x center.y> \n\n  └ ";
  std::string line;
  std::getline(std::cin, line);
  std::istringstream iss{line};

  auto size = readNext<double>(iss, "Erro ao ler size.");
  auto center = getPoint(iss);

  if (!size.has_value() || !size.has_value())
    return;

  if (!center.has_value()) {
    std::cout << "  [LOG] Erro ao ler center.\n";
    return;
  }

  if (m_shapes.find(name) != m_shapes.end())
    std::cout << "  [LOG] Line '" << name << "' já existe. Sobrescrevendo...\n";

  auto square = std::make_unique<Square>(Square::fromCenter(center.value(), size.value()));

  auto new_canvas = std::make_unique<Canvas>(*m_canvas.top());

  square->drawObject(*new_canvas, {0, 0, 0});

  m_canvas.push(std::move(new_canvas));

  storeShape(std::move(name), std::move(square));
}

void ITerm::rollback(std::istringstream &) {

  if (m_canvas.size() < 2) {
    std::cout << "Sem 'Canvas' anterior.\n";
    return;
  }

  m_canvas.pop();
  std::cout << "Done.";
}

void ITerm::createPolygon(std::string name) {
  std::cout << "Format: <p1.x> <p1.y> ... <pn.x pn.y>\n\n  └ ";

  std::string line;
  std::getline(std::cin, line);
  std::istringstream iss{line};

  std::vector<Point2> points;
  while (!iss.eof()) {
    auto p = getPoint(iss);

    if (!p.has_value()) {
      std::cout << "  [LOG] Erro ao ler point de número " << points.size() + 1
                << ".";
      return;
    }

    points.push_back(p.value());
  }

  auto polygon = std::make_unique<Polygon>(points);

  auto new_canvas = std::make_unique<Canvas>(*m_canvas.top());

  polygon->drawObject(*new_canvas, {0, 0, 0});

  m_canvas.push(std::move(new_canvas));

  storeShape(std::move(name), std::move(polygon)); /// Eficiencia extrema pai
}

void ITerm::createPolyline(std::string name) {

  std::cout << "Format: <p1.x> <p1.y> ... <pn.x pn.y>\n\n  └ ";

  std::string line;
  std::getline(std::cin, line);
  std::istringstream iss{line};

  std::vector<Point2> points;
  while (!iss.eof()) {
    auto p = getPoint(iss);

    if (!p.has_value()) {
      std::cout << "  [LOG] Erro ao ler point de número " << points.size() + 1
                << ".";
      return;
    }

    points.push_back(p.value());
  }

  auto polyline = std::make_unique<Polyline>(points);

  auto new_canvas = std::make_unique<Canvas>(*m_canvas.top());

  polyline->drawObject(*new_canvas, {0, 0, 0});

  m_canvas.push(std::move(new_canvas));

  storeShape(std::move(name), std::move(polyline)); /// Eficiencia extrema pai
}

void ITerm::create(std::istringstream &iss) {
  auto shapeType = readNext<std::string>(iss, "Faltando parâmetro (forma)!");
  if (!shapeType.has_value())
    return;

  if (auto it = m_createMap.find(shapeType.value()); it != m_createMap.end()) {

    auto shapeName = readNext<std::string>(iss, "Faltando nome do objeto!");
    if (!shapeName.has_value())
      return;

    auto createFunc = it->second;
    (this->*createFunc)(shapeName.value());
  } else {
    std::cout << "Parâmetro '" << shapeType.value() << "' inválido.\n";
  }

  std::cout << "\n  [LOG] Done.\n";
}

/// Datapath comum aos dois fills; o chamador escolhe o algoritmo (Fill&).
void ITerm::applyFill(std::istringstream &iss, Fill &algorithm) {
  auto seed = getPoint(iss);
  if (!seed.has_value())
    return;

  auto color = promptColor();
  if (!color.has_value())
    return;

  /// Mesmo protocolo dos create*: copia -> modifica -> empilha.
  /// É isso que faz o rollback funcionar igual para todo comando.
  auto new_canvas = std::make_unique<Canvas>(*m_canvas.top());
  algorithm.fill(*new_canvas, seed.value(), color.value());
  m_canvas.push(std::move(new_canvas));

  std::cout << "\n  [LOG] Done.\n";
}

void ITerm::floodFill(std::istringstream &iss) {
  FloodFill algorithm;
  applyFill(iss, algorithm);
}

void ITerm::background(std::istringstream &iss) {
  auto color = readColor(iss);
  if (!color.has_value()) {
    std::cout << "  [LOG] Use: background <R> <G> <B>  (0-255)\n";
    return;
  }

  m_bgColor = color.value();
  renderShapes();
  std::cout << "Done.\n";
}

void ITerm::storeShape(std::string name, std::unique_ptr<Object> shape) {
  if (m_shapes.find(name) == m_shapes.end())
    m_order.push_back(name); /// nome novo entra no topo da z-order

  /// Sobrescrever mantém a posição na ordem e zera o atributo de fill.
  m_shapes[std::move(name)] = SceneItem{std::move(shape), std::nullopt};
}

ITerm::SceneItem *ITerm::findItem(const std::string &name) {
  auto it = m_shapes.find(name);
  if (it == m_shapes.end()) {
    std::cout << "Objeto '" << name << "' não existe.\n";
    return nullptr;
  }
  return &it->second;
}

void ITerm::renderShapes() {
  auto w = m_canvas.top()->width();
  auto h = m_canvas.top()->height();

  m_canvas.push(std::make_unique<Canvas>(w, h, m_filename));
  fillCanvas(m_bgColor);

  /// Painter's algorithm na ordem de criação: preenche e contorna cada
  /// shape; quem vem depois cobre quem veio antes.
  for (const auto &name : m_order) {
    auto &item = m_shapes.at(name);
    if (item.fill.has_value())
      item.shape->fillObject(*m_canvas.top(), item.fill.value());
    item.shape->drawObject(*m_canvas.top(), {0, 0, 0});
  }
}

void ITerm::fillShape(std::istringstream &iss) {
  auto name =
      readNext<std::string>(iss, "Faltando nome! Use: fill <nome> <R> <G> <B>");
  if (!name.has_value())
    return;

  auto item = findItem(name.value());
  if (item == nullptr)
    return;

  if (!item->shape->hasInterior()) {
    std::cout << "'" << name.value()
              << "' é uma forma aberta (sem interior). "
                 "fill vale para circle, ellipse e polygon.\n";
    return;
  }

  auto color = readColor(iss);
  if (!color.has_value())
    return;

  /// O fill vira atributo do shape: transforma junto com a geometria.
  item->fill = color.value();
  renderShapes();
  std::cout << "Done.\n";
}

void ITerm::help(std::istringstream &) {
  std::string_view msg = R"(

    Usage: <command> [params]

    == Cena (re-renderizada da geometria; sobrevive a transformações) ==

    - create (c): create a shape.
        create <shape> <name>
        shape: circle (c), line (l), ellipse (e), polygon (po), polyline (pe).

    - fill: attach a fill color to a CLOSED shape (circle/ellipse/polygon).
        fill <name> <R> <G> <B>        (0-255; transforma junto com a figura)

    - rotate (rot):    rotate <name> <degrees> [pivot.x pivot.y]
    - translate (tr):  translate <name> <dx> <dy>
    - scale (sc):      scale <name> <sx> <sy> [pivot.x pivot.y]
        default pivot: the shape's own axis (getAxis).

    - background (bg): scene clear color.
        background <R> <G> <B>         (0-255)

    == Raster (pintura sobre pixels; some em qualquer re-render de cena) ==

    - scanline (sl): scanline seed fill.   scanline <seed.x> <seed.y>
    - flood (ff): flood fill.       seed <seed.x> <seed.y>
        (ambos pedem <R> <G> <B> na linha seguinte)
    - filter (f): filter <average|median> [kernel]   (kernel odd, default 3)

    == Meta ==

    - rollback (r): come back to the last canvas.
    - help (h): show this message.
    - exit (e): save and quit.

)";
  std::cout << msg << '\n';
}

void ITerm::exit(std::istringstream &) {
  m_running = false;
  std::cout << "Salvando arquivo '" << m_filename << "'.\n";
  std::cout << "  [LOG] Saindo...\n";
}

void ITerm::run() {
  std::string opt;
  m_running = true;

  initMsg();

  while (m_running) {
    m_canvas.top()->export_img();

    std::cout << CURSOR;

    if (!std::getline(std::cin, opt))
      break; /// Lida com EOF (Ctrl+D)

    std::istringstream iss{opt};
    std::string cmd;

    if (!(iss >> cmd))
      continue; /// Enter vazio pressionado

    std::cout << "\n  [LOG] ";

    auto it = m_commandMap.find(cmd);
    if (it == m_commandMap.end()) {
      std::cout << "Comando '" << cmd << "' inexistente!\n\n";
      continue;
    }

    /// Invoca a função membro mapeada
    auto commandFunc = it->second;
    (this->*commandFunc)(iss);

    std::cout << "\n";
  }
}
