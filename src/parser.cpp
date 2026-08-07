
#include "parser.hpp"
#include "canvas.hpp"
#include "common.hpp"
#include "polyline.hpp"
#include "tinyxml2.h"

#include "averageFilter.hpp"
#include "bezier.hpp"
#include "circle.hpp"
#include "ellipse.hpp"
#include "line.hpp"
#include "medianFilter.hpp"
#include "polygon.hpp"
#include "rectangle.hpp"
#include "square.hpp"

#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace pet {

std::size_t widht = 800, height = 400;
std::string filename = "scene.png";
bool gc = false;
RGBColor background{1.0, 1.0, 1.0};
Scene sc;

std::optional<RGBColor> getFill(XMLElement *elem, const char *attr = "fill") {

  if (const char *bg = elem->Attribute(attr)) {
    std::istringstream iss{bg};
    int r, g, b;
    if (iss >> r >> g >> b)
      return RGBColor(r, g, b, "rgb"); // "rgb" normaliza /255
    else
      std::cerr << "  [WARN] Atributo " << attr << "inválido .\n";
  }

  return {};
}

std::vector<Point2> getPoints(XMLElement *elem, const char *attr,
                              bool unsign = true) {
  const char *raw = elem->Attribute(attr);
  if (raw == nullptr)
    return {};

  std::vector<Point2> points;
  std::istringstream iss{raw};
  int x, y;
  while (iss >> x >> y) {
    if (unsign && (x < 0 || y < 0)) {
      std::cerr << "  [WARN] Valores em '" << attr
                << "' não podem ser negativos.\n";
      return {};
    }
    points.push_back({x, y});
  }

  if (!iss.eof()) {
    std::cerr << "  [WARN] '" << attr << "' tem conteúdo inesperado.\n";
    return {};
  }

  return points;
}

void canvas(XMLElement *elem) {
  auto size = getPoints(elem, "size");
  if (size.size() != 1) {
    std::cerr
        << "  [WARN] <canvas> exige size=\"largura altura\"; usando padrão.\n";
  } else {
    widht = size.front().x();
    height = size.front().y();
  }

  if (const char *fn = elem->Attribute("filename"))
    filename = fn;

  elem->QueryBoolAttribute("gc", &gc); // gc do canvas

  background = getFill(elem, "background")
                   .value_or(RGBColor{
                       1,
                       1,
                       1,
                   });
}

// --- Transform
void rotate(XMLElement *elem) {
  Object *shape = sc.back().shape.get(); // a figura recem-empilhada
  double deg;
  if (elem->QueryDoubleAttribute("deg", &deg) != XML_SUCCESS)
    return;
  Transform t;
  t.rotate(deg, shape->getAxis());
  shape->transform(t);
}

void translate(XMLElement *elem) {
  Object *shape = sc.back().shape.get(); // a figura recem-empilhada
  auto by = getPoints(elem, "by", false);
  if (by.size() != 1)
    return;
  Transform t;
  t.translate(by.front());
  shape->transform(t);
}

void scale(XMLElement *elem) {
  Object *shape = sc.back().shape.get(); // a figura recem-empilhada

  const char *raw = elem->Attribute("by");
  if (raw == nullptr)
    return;
  std::istringstream iss{raw};
  double sx, sy;
  if (!(iss >> sx >> sy)) {
    std::cerr << "  [WARN] scale exige by=\"sx sy\".\n";
    return;
  }
  Transform t;
  t.scale(sx, sy, shape->getAxis());
  shape->transform(t);
}

// --- Object
void line(XMLElement *elem) {
  auto start = getPoints(elem, "start");
  auto end = getPoints(elem, "end");
  unsigned int thickness;

  if (start.empty() || end.empty()) {
    std::cerr << "[WARN] Erro ao ler line.\n";
    return;
  }

  if (elem->Attribute("fill"))
    std::cout << "  [WARN] Line não possui atributo fill.\n";

  SceneOp scop;

  if (elem->QueryUnsignedAttribute("thickness", &thickness) == XML_SUCCESS) {
    scop.shape = std::make_unique<Line>(start.front(), end.front(),
                                        Point2(1, 1), thickness);
    sc.push_back(std::move(scop));

    return;
  }

  scop.shape = std::make_unique<Line>(start.front(), end.front());
  sc.push_back(std::move(scop));
}

void circle(XMLElement *elem) {
  unsigned int radius;
  if (elem->QueryUnsignedAttribute("radius", &radius) != XML_SUCCESS) {
    std::cerr << "  [WARN] Erro ao ler circle.radius.\n";
    return;
  }

  auto center = getPoints(elem, "center");
  if (center.empty())
    return;

  SceneOp scop;
  scop.shape = std::make_unique<Circle>(radius, center.front());
  scop.fill = getFill(elem);
  sc.push_back(std::move(scop));
}

void square(XMLElement *elem) {
  SceneOp scop;
  auto top_left = getPoints(elem, "top_left");
  auto center = getPoints(elem, "center");
  unsigned int size;

  if ((top_left.empty() || center.empty()) &&
      (elem->QueryUnsignedAttribute("size", &size) != XML_SUCCESS)) {
    std::cerr << "  [WARN] Erro ao ler os argumentos do square.\n";
    return;
  }

  if (!top_left.empty()) {
    scop.shape =
        std::make_unique<Square>(Square::fromCorner(top_left.front(), size));
  }

  if (!center.empty()) {
    scop.shape =
        std::make_unique<Square>(Square::fromCenter(center.front(), size));
  }

  scop.fill = getFill(elem);
  sc.push_back(std::move(scop));
}

void rectangle(XMLElement *elem) {
  auto top_left = getPoints(elem, "top_left");
  auto bottom_right = getPoints(elem, "bottom_right");

  if (top_left.empty() || bottom_right.empty()) {
    std::cerr << "  [WARN] Erro ao ler os corners.\n";
    return;
  }

  SceneOp scop;
  scop.shape =
      std::make_unique<Rectangle>(top_left.front(), bottom_right.front());
  scop.fill = getFill(elem);
  sc.push_back(std::move(scop));
}

void ellipse(XMLElement *elem) {
  auto radii = getPoints(elem, "radii");
  auto center = getPoints(elem, "center");

  if (radii.empty() || center.empty())
    return;

  SceneOp scop;
  scop.shape = std::make_unique<Ellipse>(radii.front().x(), radii.front().y(),
                                         center.front());
  scop.fill = getFill(elem);
  sc.push_back(std::move(scop));
}

void polygon(XMLElement *elem) {
  auto points = getPoints(elem, "points");
  if (points.size() < 3) { // poligono precisa de area
    std::cerr << "  [WARN] <polygon> exige >= 3 pontos.\n";
    return;
  }

  SceneOp scop;
  scop.shape = std::make_unique<Polygon>(points);
  scop.fill = getFill(elem);
  sc.push_back(std::move(scop));
}

void bezier(XMLElement *elem) {
  auto points = getPoints(elem, "points");
  if (points.size() != 4) {
    std::cerr << "  [WARN] <bezier> exige 4 pontos.\n";
    return;
  }

  if (elem->Attribute("fill"))
    std::cout << "  [WARN] bezier não possui atributo fill.\n";

  SceneOp scop;
  scop.shape = std::make_unique<Bezier>(points);
  sc.push_back(std::move(scop));
}

void polyline(XMLElement *elem) {
  auto points = getPoints(elem, "points");
  if (points.size() < 2) { // poligono precisa de area
    std::cerr << "  [WARN] <polyline> exige >= 2 pontos.\n";
    return;
  }

  SceneOp scop;
  unsigned int thickness;

  if (elem->QueryUnsignedAttribute("thickness", &thickness) == XML_SUCCESS) {
    scop.shape = std::make_unique<Polyline>(points, Point2(1,1), thickness);
    sc.push_back(std::move(scop));
    return;
  }

  scop.shape = std::make_unique<Polyline>(points);
  sc.push_back(std::move(scop));
}

// --- Filters
void average(XMLElement *elem) {
  int kernel = 3; // default 3x3
  elem->QueryIntAttribute("kernel", &kernel);
  if (kernel < 1 || kernel % 2 == 0) { // vizinhanca simetrica exige impar
    std::cerr << "  [WARN] average kernel deve ser ímpar >= 1.\n";
    return;
  }
  SceneOp scop;
  scop.filter = std::make_unique<AverageFilter>(kernel);
  sc.push_back(std::move(scop));
}

void median(XMLElement *elem) {
  int kernel = 3;
  elem->QueryIntAttribute("kernel", &kernel);
  if (kernel < 1 || kernel % 2 == 0) {
    std::cerr << "  [WARN] median kernel deve ser ímpar >= 1.\n";
    return;
  }
  SceneOp scop;
  scop.filter = std::make_unique<MedianFilter>(kernel);
  sc.push_back(std::move(scop));
}

std::unordered_map<std::string, std::function<void(XMLElement *)>> tags{
    {"line", line},         {"circle", circle},   {"rectangle", rectangle},
    {"square", square},     {"ellipse", ellipse}, {"polygon", polygon},
    {"polyline", polyline}, {"canvas", canvas},   {"bezier", bezier},
    {"average", average},   {"median", median},   {"translate", translate},
    {"rotate", rotate},     {"scale", scale},
};

void load(const string &filename) {
  XMLDocument doc;

  if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
    std::cerr << "  [ERROR] Error loading the '" << filename << "' XML file!\n";
    doc.PrintError();
    return;
  }

  auto root = doc.FirstChildElement("PEinT");
  if (!root) {
    std::cout << "  [ERROR] Erro ao ler tag raiz 'PEinT'.\n";
    return;
  }

  for (auto elem = root->FirstChildElement(); elem;
       elem = elem->NextSiblingElement()) {
    auto name = elem->Name();

    if (auto it = tags.find(name); it != tags.end()) {
      it->second(elem);
      continue;
    }

    std::cout << "  [WARN] Tag '" << name << "' inválida.\n";
  }
}

void render(const std::string &inputFile) {
  load(inputFile);

  ImgType type =
      filename.size() >= 4 && filename.substr(filename.size() - 4) == ".png"
          ? ImgType::PNG
          : ImgType::PPM3;

  Canvas canvas(widht, height, filename, gc, type);

  for (int y = 0; y < canvas.height(); ++y)
    for (int x = 0; x < canvas.width(); ++x)
      canvas.add(Pixel(x, y), background);

  const RGBColor black{0, 0, 0};

  for (const auto &sceneOp : sc) {
    /*
     * >> Se não tiver filtros:
     * if (sceneOp.fill.has_value() && sceneOp.shape->hasInterior())
     *   sceneOp.shape->fillObject(canvas, sceneOp.fill.value());
     * sceneOp.shape->drawObject(canvas, black);
     *
     * >> Se não houver filtros nem fill para cada figura:
     *
     */

    if (sceneOp.isFilter()) {
      sceneOp.filter->applyFilter(canvas);
    } else {
      if (sceneOp.fill.has_value() && sceneOp.shape->hasInterior())
        sceneOp.shape->fillObject(canvas, sceneOp.fill.value());
      sceneOp.shape->drawObject(canvas, black);
    }
  }

  canvas.export_img();
}

} // namespace pet
