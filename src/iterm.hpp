#ifndef ITERM_HPP
#define ITERM_HPP

#include "canvas.hpp"
#include "common.hpp"
#include "object.hpp"

#include <array>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>

namespace pet {

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

  void fillCanvas(RGBColor = {255, 255, 255}); /// BRANCO
  void fillCanvas(const std::array<RGBColor, 2> &);

  // --- Funções de Comando ---
  void commandCreate(std::istringstream &);
  void commandHelp(std::istringstream &);
  void commandExit(std::istringstream &);
  void commandRollback(std::istringstream &);

  void createCircle(std::string);
  void createLine(std::string);
  void createPolygon(std::string);
  void createPolyline(std::string);

  // setup
  void setupCommands();

  std::stack<std::unique_ptr<Canvas>> m_canvas;
  // Canvas m_canvas;
  bool m_running;
  std::string_view m_filename;

  std::unordered_map<std::string, std::unique_ptr<Object>> m_shapes;

  using CommandHandler = void (ITerm::*)(std::istringstream &);
  std::unordered_map<std::string_view, CommandHandler> m_commandMap;

  using CreateHandler = void (ITerm::*)(std::string);
  std::unordered_map<std::string_view, CreateHandler> m_createMap;
};

} // namespace pet

#endif // !ITERM_HPP
