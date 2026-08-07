#ifndef PARSER_HPP
#define PARSER_HPP

#include "filter.hpp"
#include "object.hpp"

#include <memory>
#include <string>
#include <vector>

namespace pet {

struct SceneOp {
  std::unique_ptr<Object> shape;
  std::optional<RGBColor> fill;
  std::unique_ptr<Filter> filter;

  bool isFilter() const { return filter != nullptr; }
};

using Scene = std::vector<SceneOp>;

void render(const std::string &filename);

} // namespace pet

#endif // !PARSER_HPP
