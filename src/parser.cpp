
#include "parser.hpp"
#include "canvas.hpp"
#include "common.hpp"
#include "tinyxml2.h"

#include "circle.hpp"
#include "line.hpp"
#include "polygon.hpp"

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
                   .value_or({
                       1,
                       1,
                       1,
                   });
}

// --- Object
void line(XMLElement *elem) {
  auto start = getPoints(elem, "start");
  auto end = getPoints(elem, "end");

  if (start.empty() || end.empty()) {
    std::cerr << "[WARN] Erro ao ler line.\n";
    return;
  }

  if (elem->Attribute("fill"))
    std::cout << "  [WARN] Line não possui atributo fill.\n";

  SceneOp scop;
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

std::unordered_map<std::string, std::function<void(XMLElement *)>> tags{
    {"line", line},
    {"circle", circle},
    {"polygon", polygon},
    {"canvas", canvas},

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

  for (const auto &sceneOp : sc)
    sceneOp.shape->drawObject(canvas, black);

  canvas.export_img();
}

} // namespace pet
