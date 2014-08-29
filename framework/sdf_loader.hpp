#ifndef BUW_SDF_LOADER_HPP
#define BUW_SDF_LOADER_HPP

#include "scene.hpp"

#include <sstream>

class SdfLoader
{
public:
  SdfLoader(std::string const& filename);

private:

  enum class Error {
    UNKNOWN,

    EXPECT_FLOAT
    EXPECT_STRING
    EXPECT_EOF,

    UNKNOWN_COMMAND,
    UNKNOWN_OBJECT,
    UNKNOWN_SHAPE,
    UNKNOWN_TRANSFORM
  };

  static std::map<std::string, Command> error_messages {
    {UNKNOWN,       "unknown error"},
    {EXPECT_FLOAT,  "float expected"},
    {EXPECT_STRING, "string expected"},
    {EXPECT_EOF,    "end of line expected"}
  };

  enum class Command {
    UNKNOWN,
    DEFINITION,
    TRANSFORMATION,
    RENDER
  };

  static std::map<std::string, Command> command_dict {
    {"define",    DEFINITION},
    {"transform", TRANSFORMATION},
    {"render",    RENDER}
  };

  enum class Object {
    UNKNOWN,
    SHAPE,
    LIGHT,
    MATERIAL,
    CAMERA
  };

  static std::map<std::string, Object> object_dict {
    {"shape",    DEFINITION},
    {"light",    TRANSFORMATION},
    {"material", RENDER},
    {"camera",   CAMERA}
  };

  enum class Shape {
    UNKNOWN,
    SPHERE,
    BOX,
    TRIANGLE,
    CYLINDER,
    CONE
  };

  static std::map<std::string, Object> shape_dict {
    {"sphere",    SPHERE},
    {"box",       BOX},
    {"triangle",  TRIANGLE},
    {"cylinder",  CYLINDER},
    {"cone",      CONE}
  };

  enum class Transformation {
    UNKNOWN,
    TRANSLATE,
    ROTATE,
    SCALE
  };

  static std::map<std::string, Object> transform_dict {
    {"translate", TRANSLATE},
    {"rotate",    ROTATE},
    {"scale",     SCALE}
  };

  float read_float(std::stringstream& from) const;
  Color read_color(std::stringstream& from) const;
  glm::vec3 read_vector(std::stringstream& from) const;
  std::string read_string(std::stringstream& from) const;

  Command read_command_symbol(stringstream& from) const;
  Object read_object_symbol(stringstream& from) const;
  Shape read_shape_symbol(stringstream& from) const;
  Transformation read_transform_symbol(stringstream& from) const;

  void request_command(stringstream& from) const;

  void print_error(stringstream& stream, int err_pos,
    Error const& error) const;

  std::map< std::string, std::shared_ptr<Camera> > cameras_;
  std::map< std::string, std::shared_ptr<Light> > lights_;
  std::map< std::string, std::shared_ptr<Shape> > shapes_;

  std::string const& filename_;
};

#endif // BUW_SDF_LOADER_HPP