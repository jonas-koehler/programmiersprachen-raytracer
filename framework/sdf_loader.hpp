#ifndef BUW_SDF_LOADER_HPP
#define BUW_SDF_LOADER_HPP

#include "scene.hpp"

#include <sstream>

class SdfLoader
{
public:
  SdfLoader(std::string const& filename);

private:

  bool read_float(std::stringstream& from, float& to);
  bool read_color(std::stringstream& from, Color& to);
  bool read_vector(std::stringstream& from, glm::vec3& to);
  bool read_string(std::stringstream& from, std::string& to);

  bool parse_line(stringstream& from);
  bool parse_definition(stringstream& from);

  std::map< std::string, std::shared_ptr<Camera> > cameras_;
  std::map< std::string, std::shared_ptr<Light> > lights_;
  std::map< std::string, std::shared_ptr<Shape> > shapes_;

  std::string const& filename_;
};

#endif // BUW_SDF_LOADER_HPP