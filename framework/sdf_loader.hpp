#ifndef BUW_SDF_LOADER_HPP
#define BUW_SDF_LOADER_HPP

#include "material.hpp"
#include "light.hpp"

#include "sphere.hpp"
#include "box.hpp"
#include "triangle.hpp"
#include "cylinder.hpp"
#include "cone.hpp"
#include "composite.hpp"

#include "scene.hpp"
#include "render_instruction.hpp"

#include <fstream>
#include <sstream>
#include <map>
#include <stack>

class SdfLoader
{
public:
  SdfLoader(std::string const& filename);

  bool parse();
  bool finished_parsing() const;
  bool has_render_instructions() const;
  RenderInstruction get_render_instruction();

private:

  enum class ErrorType {
    UNKNOWN,

    EXPECTED_UNSIGNED_INT,
    EXPECTED_FLOAT,
    EXPECTED_STRING,
    EXPECTED_EOF,

    UNKNOWN_COMMAND,
    UNKNOWN_OBJECT,
    UNKNOWN_SHAPE,
    UNKNOWN_TRANSFORM,

    NAME_ALREADY_TAKEN,
    UNKNOWN_OBJECT_NAME,
    UNKNOWN_SHAPE_NAME,
    UNKNOWN_MATERIAL_NAME,
    UNKNOWN_CAMERA_NAME,

    NO_TRANSFORM,
    NO_TRANSLATION,
    NO_SCALING,
    NO_ROTATION
  };

  enum class CommandType {
    UNKNOWN,
    DEFINITION,
    TRANSFORMATION,
    RENDER
  };

  enum class ObjectType {
    UNKNOWN,
    SHAPE,
    LIGHT,
    MATERIAL,
    CAMERA
  };

  enum class ShapeType {
    UNKNOWN,
    SPHERE,
    BOX,
    TRIANGLE,
    CYLINDER,
    CONE,
    COMPOSITE
  };

  enum class TransformationType {
    UNKNOWN,
    TRANSLATE,
    ROTATE,
    SCALE
  };

  static const std::map<ErrorType, std::string> error_messages;
  static const std::map<std::string, CommandType> command_dict;
  static const std::map<std::string, ObjectType> object_dict;
  static const std::map<std::string, ShapeType> shape_dict;
  static const std::map<std::string, TransformationType> transform_dict;

  unsigned read_unsigned();
  float read_float();
  std::string read_string();
  glm::vec3 read_vector();
  Color read_color();
  CommandType read_command_type();
  ObjectType read_object_type();
  ShapeType read_shape_type();
  TransformationType read_transform_type();

  void register_object(std::string const& name, ObjectType type);
  void register_shape(std::string const& name, std::shared_ptr<Shape> shape);
  void register_material(std::string const& name,
    std::shared_ptr<Material>material);
  void register_camera(std::string const& name,
    std::shared_ptr<Camera> camera);
  void register_light(std::string const& name, std::shared_ptr<Light> light);

  ObjectType get_registered_object(std::string const& name);
  std::shared_ptr<Shape> get_shape(std::string const& name);
  std::shared_ptr<Material> get_material(std::string const& name);
  std::shared_ptr<Camera> get_camera(std::string const& name);

  template <typename T>
  void request_translation(std::shared_ptr<T> const& target);

  template <typename T>
  void request_rotation(std::shared_ptr<T> const& target);

  template <typename T>
  void request_scale(std::shared_ptr<T> const& target);

  void define_sphere();
  void define_box();
  void define_triangle();
  void define_cylinder();
  void define_cone();
  void define_composite();

  void define_shape();
  void define_light();
  void define_material();
  void define_camera();

  void request_definition();
  void request_transform();
  void request_render_instruction();
  void request_command();

  void print_error(ErrorType const& error);
  void print_error(int err_pos, ErrorType const& error);

  std::stringstream current_line_;

  std::map< std::string, ObjectType> used_names_;

  std::map< std::string, std::shared_ptr<Camera> > cameras_;
  std::map< std::string, std::shared_ptr<Light> > lights_;
  std::map< std::string, std::shared_ptr<Shape> > shapes_;
  std::map< std::string, std::shared_ptr<Material> > materials_;

  std::stack<RenderInstruction> render_instructions_;

  std::string const& filename_;
  std::shared_ptr<Scene> scene_;
  bool finished_parsing_;

  std::ifstream ifs_;


};

#endif // BUW_SDF_LOADER_HPP