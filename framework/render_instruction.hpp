#ifndef BUW_RENDER_INSTRUCTION_HPP
#define BUW_RENDER_INSTRUCTION_HPP

#include "scene.hpp"

#include <string>

struct RenderInstruction
{
  RenderInstruction(std::shared_ptr<Scene> sc, std::string const& file, unsigned rx, unsigned ry)
   : scene(sc)
   , outfile(file)
   , res_x(rx)
   , res_y(ry)
  {}

  std::shared_ptr<Scene> scene;
  std::string outfile;
  unsigned res_x, res_y;
};

#endif // BUW_RENDER_INSTRUCTION_HPP