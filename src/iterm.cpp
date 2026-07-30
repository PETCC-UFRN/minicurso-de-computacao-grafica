#include "iterm.hpp"
#include "background.hpp"
#include "circle.hpp"
#include "common.hpp"
#include "interpoledBackground.hpp"
#include "line.hpp"
#include "polygon.hpp"

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

void ITerm::setupCommands() {
  m_commandMap["create"] = &ITerm::commandCreate;
  m_commandMap["c"] = &ITerm::commandCreate;
  m_commandMap["help"] = &ITerm::commandHelp;
  m_commandMap["h"] = &ITerm::commandHelp;
  m_commandMap["exit"] = &ITerm::commandExit;
  m_commandMap["e"] = &ITerm::commandExit;
  m_commandMap["rollback"] = &ITerm::commandRollback;
  m_commandMap["r"] = &ITerm::commandRollback;

  /// Funções create
  m_createMap["circle"] = &ITerm::createCircle;
  m_createMap["c"] = &ITerm::createCircle;
  m_createMap["line"] = &ITerm::createLine;
  m_createMap["l"] = &ITerm::createLine;
  m_createMap["polygon"] = &ITerm::createPolygon;
  m_createMap["po"] = &ITerm::createPolygon;
  m_createMap["polyline"] = &ITerm::createPolyline;
  m_createMap["pe"] = &ITerm::createPolyline;
}

void ITerm::initMsg() const {
  std::string_view msg = R"(
    Welcome to the PEinT 2d Rasterizer©.

    Done by:
    Arthur Skeete, Gabriel Carvalho, Heitor Campos, Yuri Santos, (COMPLETAR)...

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

void ITerm::fillCanvas(const std::array<RGBColor, 2> &c) {
  auto canvas = *m_canvas.top();

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

  m_shapes[std::move(name)] = std::move(circle); /// Eficiencia extrema pai
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

  m_shapes[std::move(name)] = std::move(lin);
}

void ITerm::commandRollback(std::istringstream &) {

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

  m_shapes[std::move(name)] = std::move(polygon); /// Eficiencia extrema pai
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

  m_shapes[std::move(name)] = std::move(polyline); /// Eficiencia extrema pai

}

void ITerm::commandCreate(std::istringstream &iss) {
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

void ITerm::commandHelp(std::istringstream &) {
  std::string_view msg = R"(

    Usage: <command> [params]

    - create: create a shape.
        create <shape> <name>

        shape can be: circle, line, ellipse or polygon.

    - exit: exit the program.

    - help: show this message.

    - rollback: come back to the last canvas.

)";
  std::cout << msg << '\n';
}

void ITerm::commandExit(std::istringstream &) {
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
