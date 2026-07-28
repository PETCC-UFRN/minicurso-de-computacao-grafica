#ifndef PARSER_HPP
#define PARSER_HPP

#include "object.hpp"

#include <memory>
#include <string>
#include <vector>

namespace pet {

struct SceneOp {
  std::unique_ptr<Object> shape;
  std::optional<RGBColor> fill;
};

using Scene = std::vector<SceneOp>;

void render(const std::string &filename);

} // namespace pet

#endif // !PARSER_HPP
