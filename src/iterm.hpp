#ifndef ITERM_HPP
#define ITERM_HPP

#include "canvas.hpp"
#include "common.hpp"
#include "object.hpp"

#include <array>
#include <memory>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace pet {

class Fill; /// Forward declarations (evitam includes no header)
class Transform;

/*
 * @class Classe responsável pela interface de terminal interativo.
 */
class ITerm {
public:
  ITerm(std::size_t w, std::size_t h, const std::string &f)
      : m_running(false), m_filename(f) {
    m_canvas.push(std::make_unique<Canvas>(w, h, f));
    fillCanvas();
    setupCommands();
  }

  /*
   * @brief Função que inicia o terminal.
   */
  void run();

private:
  void initMsg() const;

  void fillCanvas(RGBColor = {1.0, 1.0, 1.0}); /// BRANCO (buffer em [0,1])
  void fillCanvas(const std::array<RGBColor, 4> &);

  // --- Funções de Comando ---
  void create(std::istringstream &);
  void help(std::istringstream &);
  void exit(std::istringstream &);
  void rollback(std::istringstream &);
  void scanLineFill(std::istringstream &);
  void floodFill(std::istringstream &);
  void filter(std::istringstream &);
  void background(std::istringstream &);

  /// Lógica comum aos fills raster: lê seed + cor, aplica e empilha.
  void applyFill(std::istringstream &, Fill &);

  // --- Comandos de cena (mutam geometria/atributos e re-renderizam) ---
  void fillShape(std::istringstream &);
  void rotate(std::istringstream &);
  void translate(std::istringstream &);
  void scale(std::istringstream &);

  void createCircle(std::string);
  void createAACircle(std::string);
  void createLine(std::string);
  void createAALine(std::string);
  void createEllipse(std::string);
  void createPolygon(std::string);
  void createPolyline(std::string);
  void createRectangle(std::string);
  void createSquare(std::string);
  void createSquareCenter(std::string);

  // setup
  void setupCommands();

  struct SceneItem {
    std::unique_ptr<Object> shape;
    std::optional<RGBColor> fill;
  };

  /// Registra/sobrescreve um shape preservando a ordem de criação.
  void storeShape(std::string name, std::unique_ptr<Object> shape);

  /// Busca por nome; imprime erro e retorna nullptr se não existir.
  SceneItem *findItem(const std::string &name);

  void renderShapes();

  /// Aplica uma Transform a um item e re-renderiza.
  void applyTransform(SceneItem &, const Transform &);

  std::stack<std::unique_ptr<Canvas>> m_canvas;
  bool m_running;
  std::string m_filename;

  std::unordered_map<std::string, SceneItem> m_shapes;
  std::vector<std::string> m_order;  /// z-order determinístico: o
                                     /// unordered_map não tem ordem.
  RGBColor m_bgColor{1.0, 1.0, 1.0}; /// cor de fundo da cena (clear color)

  using CommandHandler = void (ITerm::*)(std::istringstream &);
  std::unordered_map<std::string_view, CommandHandler> m_commandMap;

  using CreateHandler = void (ITerm::*)(std::string);
  std::unordered_map<std::string_view, CreateHandler> m_createMap;
};

} // namespace pet

#endif // !ITERM_HPP
