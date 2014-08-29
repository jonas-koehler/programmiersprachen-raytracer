#include "sdf_loader.hpp"

SdfLoader::SdfLoader(std::string const& filename)
 : filename_(filename)
 , camera_()
 , lights_()
 , shapes_()
{}

void
SdfLoader::read_float(std::stringstream& from) const
{ 
  float to = 0;
  auto err_pos = from.tellg() + 1;
  if (!(from >> to)) {
    print_error(from, err_pos, EXPECTED_STRING)
  }
  return to;
}

std::string
SdfLoader::read_string(std::stringstream& from) const
{
  std::string to;
  auto err_pos = from.tellg() + 1;
  if (!(from >> to)) {
    print_error(from, err_pos, EXPECTED_STRING)
  }
  return to;
}

glm::vec3
SdfLoader::read_vector(std::stringstream& from) const
{ 
  glm::vec3 to;
  to.x = read_float(from);
  to.y = read_float(from);
  to.z = read_float(from);
  return to;
}

Color
SdfLoader::read_color(std::stringstream& from) const
{ 
  Color to;
  to.r = read_float(from);
  to.g = read_float(from);
  to.b = read_float(from);
  return to;
}

Command
SdfLoader::read_command_symbol(stringstream& from) const
{
  auto it = command_dict.find(read_string(from));
  if (it == command_dict.end()) {
    print_error(from, err_pos, UNKNOWN_COMMAND);
    return Command::UNKNOWN;
  }
  return (*it).second;
}

Object
SdfLoader::read_object_symbol(stringstream& from) const
{
  auto it = object_dict.find(read_string(from));
  if (it == object_dict.end()) {
    print_error(from, err_pos, UNKNOWN_OBJECT);
    return Object::UNKNOWN;
  }
  return (*it).second;
}

Shape
SdfLoader::read_shape_symbol(stringstream& from) const
{
  auto it = shape_dict.find(read_string(from));
  if (it == shape_dict.end()) {
    print_error(from, err_pos, UNKNOWN_SHAPE);
    return Shape::UNKNOWN;
  }
  return (*it).second;
}

Transformation
SdfLoader::read_transform_symbol(stringstream& from) const
{
  auto it = transform_dict.find(read_string(from));
  if (it == transform_dict.end()) {
    print_error(from, err_pos, UNKNOWN_TRANSFORM);
    return Transformation::UNKNOWN;
  }
  return (*it).second;
}

void
SdfLoader::request_definition(stringstream& from)
{
  Object obj = read_object_symbol(from);
  switch (obj) {
    case SHAPE:
      break;
    case LIGHT:
      break;
    case MATERIAL:
      break;
    case CAMERA:
      break;
    default:
      break;
  }
}

void
SdfLoader::request_command(stringstream& from)
{
  Command cmd = read_command_symbol(from);
  switch (cmd) {
    case DEFINITION:
      break;
    case TRANSFORMATION:
      break;
    case RENDER:
      break;
    default:
      break;
  }
}

void
SdfLoader::print_error(stringstream& stream, int err_pos,
  Error const& error) const
{
  std::string error_msg = *(error_messages.find(error));

  std::cout << "ERROR: '"<< error_msg << "'' at:" << std::endl;
  std::cout << stream.str() << std::endl;
  for (int i=0; i<p; ++i) {
    std::cout << " ";
  }
  std::cout << "^";
}