#include "background.hpp"
#include "canvas.hpp"
#include "circle.hpp"
#include "common.hpp"
#include "interpoledBackground.hpp"
#include "iterm.hpp"
#include "line.hpp"
#include "parser.hpp"
#include <sys/types.h>

using namespace pet;

int main(int argc, char **argv) {
  // ITerm it(800, 400, "result.ppm");
  // it.run();

  if (argc > 1) {
    render(argv[1]);
    return 0;
  }

  return 0;
}
