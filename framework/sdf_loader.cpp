#include "sdf_loader.hpp"

const std::map<SdfLoader::ErrorType, std::string>
SdfLoader::error_messages {
  { SdfLoader::ErrorType::UNKNOWN,             "unknown error" },

  { SdfLoader::ErrorType::EXPECTED_UNSIGNED_INT,        "uint expected" },
  { SdfLoader::ErrorType::EXPECTED_FLOAT,        "float expected" },
  { SdfLoader::ErrorType::EXPECTED_STRING,       "string expected" },
  { SdfLoader::ErrorType::EXPECTED_EOF,          "end of line expected" },

  { SdfLoader::ErrorType::UNKNOWN_COMMAND,     "unknown command" },
  { SdfLoader::ErrorType::UNKNOWN_OBJECT,      "unknown object type" },
  { SdfLoader::ErrorType::UNKNOWN_SHAPE,       "unknown shape type" },
  { SdfLoader::ErrorType::UNKNOWN_TRANSFORM,   "unknown transform type" },

  { SdfLoader::ErrorType::UNKNOWN_OBJECT_NAME, "no object with this name" },
  { SdfLoader::ErrorType::UNKNOWN_SHAPE_NAME, "no shape with this name" },
  { SdfLoader::ErrorType::UNKNOWN_MATERIAL_NAME, "no material with this name" },
  { SdfLoader::ErrorType::NAME_ALREADY_TAKEN, "name already taken"},
  { SdfLoader::ErrorType::UNKNOWN_CAMERA_NAME, "no camera with this name" },
  { SdfLoader::ErrorType::UNKNOWN_LIGHT_NAME, "no lights with this name" },

  { SdfLoader::ErrorType::NO_TRANSFORM,        "object can not be transformed" },
  { SdfLoader::ErrorType::NO_TRANSLATION,      "object can not be translated" },
  { SdfLoader::ErrorType::NO_SCALING,            "object can not be scaled" },
  { SdfLoader::ErrorType::NO_ROTATION,           "object can not be rotated" }
};

const std::map<std::string, SdfLoader::CommandType>
SdfLoader::command_dict {
  {"define",    SdfLoader::CommandType::DEFINITION},
  {"transform", SdfLoader::CommandType::TRANSFORMATION},
  {"render",    SdfLoader::CommandType::RENDER},
  {"#",         SdfLoader::CommandType::COMMENT}
};

const std::map<std::string, SdfLoader::ObjectType>
SdfLoader::object_dict {
  {"shape",    SdfLoader::ObjectType::SHAPE},
  {"light",    SdfLoader::ObjectType::LIGHT},
  {"material", SdfLoader::ObjectType::MATERIAL},
  {"camera",   SdfLoader::ObjectType::CAMERA}
};

const std::map<std::string, SdfLoader::ShapeType>
SdfLoader::shape_dict {
  {"sphere",    SdfLoader::ShapeType::SPHERE},
  {"box",       SdfLoader::ShapeType::BOX},
  {"triangle",  SdfLoader::ShapeType::TRIANGLE},
  {"triangle_n", SdfLoader::ShapeType::TRIANGLE_WITH_NORMAL},
  {"cylinder",  SdfLoader::ShapeType::CYLINDER},
  {"cone",      SdfLoader::ShapeType::CONE},
  {"composite",      SdfLoader::ShapeType::COMPOSITE}
};

const std::map<std::string, SdfLoader::TransformationType>
SdfLoader::transform_dict {
  {"translate", SdfLoader::TransformationType::TRANSLATE},
  {"rotate",    SdfLoader::TransformationType::ROTATE},
  {"scale",     SdfLoader::TransformationType::SCALE}
};




SdfLoader::SdfLoader(std::string const& filename)
: filename_(filename)
, finished_parsing_(false)
, used_names_()
, cameras_()
, lights_()
, shapes_()
, materials_()
, scene_()
, ifs_()
, current_line_()
, render_instructions_()
, unprocessed_shapes_()
{
  scene_ = std::make_shared<Scene>();
  ifs_.open (filename_, std::ifstream::in);
  finished_parsing_ = false ;
}

bool
SdfLoader::parse()
{
  if (ifs_.good()) {
    std::string line;
    std::getline (ifs_, line);
    current_line_ = std::stringstream (line);
    request_command();
  } else {
    finished_parsing_ = true;
    ifs_.close();
  }
  return !finished_parsing_;
}

bool
SdfLoader::finished_parsing() const
{
  return finished_parsing_;
}

bool SdfLoader::has_render_instructions() const
{
  return render_instructions_.size();
}

RenderInstruction
SdfLoader::get_render_instruction()
{
  auto result = render_instructions_.top();
  render_instructions_.pop();
  return result;
}




unsigned
SdfLoader::read_unsigned()
{
  int to = 0;
  auto err_pos = current_line_.tellg();
  if (!(current_line_ >> to)) {
    print_error(err_pos, SdfLoader::ErrorType::EXPECTED_UNSIGNED_INT);
  }
  return to;
}

float
SdfLoader::read_float()
{
  float to = 0;
  auto err_pos = current_line_.tellg();
  if (!(current_line_ >> to)) {
    print_error(err_pos, SdfLoader::ErrorType::EXPECTED_FLOAT);
  }
  return to;
}

std::string
SdfLoader::read_string()
{
  std::string to;
  auto err_pos = current_line_.tellg();
  if (!(current_line_ >> to)) {
    print_error(err_pos, SdfLoader::ErrorType::EXPECTED_STRING);
  }
  return to;
}

glm::vec3
SdfLoader::read_vector()
{
  glm::vec3 to;
  to.x = read_float();
  to.y = read_float();
  to.z = read_float();
  return to;
}

Color
SdfLoader::read_color()
{
  Color to;
  to.r = read_float();
  to.g = read_float();
  to.b = read_float();
  return to;
}

SdfLoader::CommandType
SdfLoader::read_command_type()
{
  auto it = command_dict.find(read_string());
  if (it == command_dict.end()) {
    print_error(SdfLoader::ErrorType::UNKNOWN_COMMAND);
    return SdfLoader::CommandType::UNKNOWN;
  }
  return (*it).second;
}

SdfLoader::ObjectType
SdfLoader::read_object_type()
{
  auto it = object_dict.find(read_string());
  if (it == object_dict.end()) {
    print_error(SdfLoader::ErrorType::UNKNOWN_OBJECT);
    return SdfLoader::ObjectType::UNKNOWN;
  }
  return (*it).second;
}

SdfLoader::ShapeType
SdfLoader::read_shape_type()
{
  auto it = shape_dict.find(read_string());
  if (it == shape_dict.end()) {
    print_error(SdfLoader::ErrorType::UNKNOWN_SHAPE);
    return SdfLoader::ShapeType::UNKNOWN;
  }
  return (*it).second;
}

SdfLoader::TransformationType
SdfLoader::read_transform_type()
{
  auto it = transform_dict.find(read_string());
  if (it == transform_dict.end()) {
    print_error(SdfLoader::ErrorType::UNKNOWN_TRANSFORM);
    return SdfLoader::TransformationType::UNKNOWN;
  }
  return (*it).second;
}







void
SdfLoader::register_object(std::string const& name, SdfLoader::ObjectType type)
{
  auto it = used_names_.find(name);
  if (it != used_names_.end()) {
    print_error(SdfLoader::ErrorType::NAME_ALREADY_TAKEN);
  } else {
    used_names_[name] = type;
  }
}

void
SdfLoader::register_shape(std::string const& name,
  std::shared_ptr<Shape> shape)
{
  register_object(name, SdfLoader::ObjectType::SHAPE);
  shapes_[name] = shape;
  shape->parent(scene_->root());
  unprocessed_shapes_.push(shape);
}

void
SdfLoader::register_material(std::string const& name,
  std::shared_ptr<Material> material)
{
  register_object(name, SdfLoader::ObjectType::MATERIAL);
  materials_[name] = material;
}

void
SdfLoader::register_camera(std::string const& name,
  std::shared_ptr<Camera> camera)
{
  register_object(name, SdfLoader::ObjectType::CAMERA);
  cameras_[name] = camera;
}

void
SdfLoader::register_light(std::string const& name,
  std::shared_ptr<Light> light)
{
  register_object(name, SdfLoader::ObjectType::LIGHT);
  lights_[name] = light;
  scene_->add_light(light);
}






SdfLoader::ObjectType
SdfLoader::get_registered_object(std::string const& name)
{
  auto it = used_names_.find(name);
  if (it == used_names_.end()) {
    print_error(SdfLoader::ErrorType::UNKNOWN_OBJECT_NAME);
    return SdfLoader::ObjectType::UNKNOWN;
  }
  return (*it).second;
}

std::shared_ptr<Shape>
SdfLoader::get_shape(std::string const& name)
{
  auto it = shapes_.find(name);
  if (it == shapes_.end()) {
    print_error(SdfLoader::ErrorType::UNKNOWN_SHAPE_NAME);
    return std::shared_ptr<Shape>();
  }
  return (*it).second;
}

std::shared_ptr<Material>
SdfLoader::get_material(std::string const& name)
{
  auto it = materials_.find(name);
  if (it == materials_.end()) {
    print_error(SdfLoader::ErrorType::UNKNOWN_MATERIAL_NAME);
    return std::shared_ptr<Material>();
  }
  return (*it).second;
}

std::shared_ptr<Camera>
SdfLoader::get_camera(std::string const& name)
{
  auto it = cameras_.find(name);
  if (it == cameras_.end()) {
    print_error(SdfLoader::ErrorType::UNKNOWN_CAMERA_NAME);
    return std::shared_ptr<Camera>();
  }
  return (*it).second;
}

std::shared_ptr<Light>
SdfLoader::get_light(std::string const& name)
{
  auto it = lights_.find(name);
  if (it == lights_.end()) {
    print_error(SdfLoader::ErrorType::UNKNOWN_LIGHT_NAME);
    return std::shared_ptr<Light>();
  }
  return (*it).second;
}




template <typename T>
void
SdfLoader::request_translation(std::shared_ptr<T> const& target)
{
  auto v = read_vector();
  target->translate(v);
}

template <typename T>
void
SdfLoader::request_rotation(std::shared_ptr<T> const& target)
{
  auto deg = read_float();
  auto rad = deg / 180 * M_PI;
  auto axis = read_vector();
  target->rotate(rad, axis);
}

template <typename T>
void
SdfLoader::request_scale(std::shared_ptr<T> const& target)
{
  auto v = read_vector();
  target->scale(v);
}


void
SdfLoader::define_sphere()
{
  auto name = read_string();
  auto center = read_vector();
  auto radius = read_float();

  auto material = get_material(read_string());

  auto sphere = std::make_shared<Sphere>(material);
  sphere->scale(glm::vec3(radius));
  sphere->translate(center);

  register_shape(name, sphere);
}

void
SdfLoader::define_box()
{
  auto name = read_string();
  auto pmin = read_vector();
  auto pmax = read_vector();
  auto material = get_material(read_string());

  auto box = std::make_shared<Box>(material);

  auto scale = 0.5f * (pmax - pmin);
  box->scale(scale);
  box->translate(pmin - scale * glm::vec3(-1));

  register_shape(name, box);
}

void
SdfLoader::define_triangle()
{
  auto name = read_string();
  std::array<glm::vec3, 3> v {
    read_vector(), read_vector(), read_vector()
  };
  auto material = get_material(read_string());

  auto triangle = std::make_shared<Triangle>(material, v);

  register_shape(name, triangle);
}
int i= 0;
void
SdfLoader::define_triangle_with_normal()
{
  auto name = read_string();
  std::array<glm::vec3, 3> v {
    read_vector(), read_vector(), read_vector()
  };
  std::array<glm::vec3, 3> n {
    read_vector(), read_vector(), read_vector()
  };

  auto material = get_material(read_string());

  auto triangle = std::make_shared<Triangle>(material, v, n);
  register_shape(name, triangle);
}

void
SdfLoader::define_cylinder()
{
  auto name = read_string();
  auto center = read_vector();
  auto radius = read_float();
  auto height = read_float();
  auto material = get_material(read_string());

  auto cylinder = std::make_shared<Cylinder>(material);
  cylinder->scale(glm::vec3(radius, radius, height));
  cylinder->translate(center);

  register_shape(name, cylinder);
}

void
SdfLoader::define_cone()
{
  auto name = read_string();
  auto tip = read_vector();
  auto radius = read_float();
  auto height = read_float();
  auto material = get_material(read_string());

  auto cone = std::make_shared<Cone>(material);
  cone->scale(glm::vec3(radius, radius, height));
  cone->translate(tip);

  register_shape(name, cone);
}

void
SdfLoader::define_composite()
{
  auto name = read_string();
  auto composite = std::make_shared<Composite>();

  std::cout << "filling composite '" << name << "'...";
  while (!current_line_.eof()) {
    auto child_name = read_string();
    auto child = get_shape(child_name);
    composite->add_child(child);
    child->parent(composite);
  }
  std::cout << "done" << std::endl;

  std::cout << "packing composite '" << name << "'...";
  composite->optimize();
  std::cout << "done" << std::endl;

  register_shape(name, composite);
}

void
SdfLoader::create_scene_graph()
{
  while (!unprocessed_shapes_.empty()) {
    auto shape = unprocessed_shapes_.top();
    unprocessed_shapes_.pop();
    if (auto p = shape->parent().lock()) {
      if (p == scene_->root()) {
        scene_->add_shape(shape);
      }
    }
  }
}

void
SdfLoader::define_shape()
{
  SdfLoader::ShapeType type = read_shape_type();
  switch (type) {
    case SdfLoader::ShapeType::SPHERE:
    define_sphere();
    break;
    case SdfLoader::ShapeType::BOX:
    define_box();
    break;
    case SdfLoader::ShapeType::TRIANGLE:
    define_triangle();
    break;
    case SdfLoader::ShapeType::TRIANGLE_WITH_NORMAL:
    define_triangle_with_normal();
    break;
    case SdfLoader::ShapeType::CYLINDER:
    define_cylinder();
    break;
    case SdfLoader::ShapeType::CONE:
    define_cone();
    break;
    case SdfLoader::ShapeType::COMPOSITE:
    define_composite();
    default:
    break;
  }
}

void
SdfLoader::define_light()
{
  auto name = read_string();
  auto pos = read_vector();
  auto la = read_color();
  auto ld = read_color();

  auto light = std::make_shared<Light>(la, ld);
  light->translate(pos);

  register_light(name, light);
}

void
SdfLoader::define_material()
{
  auto name = read_string();
  auto ka = read_color();
  auto kd = read_color();
  auto ks = read_color();
  auto m = read_float();
  auto opacity = read_float();
  auto n = read_float();

  auto material = std::make_shared<Material>(ka, kd, ks, m, opacity, n);

  register_material(name, material);
}

void
SdfLoader::define_camera()
{
  auto name = read_string();
  std::shared_ptr<Camera> camera;
  if (current_line_.eof()) {
    camera = std::make_shared<Camera>();
  } else {
    auto fov_x = read_float();
    auto eye = read_vector();
    auto dir = read_vector();
    auto up = read_vector();
    camera = std::make_shared<Camera>(eye, dir, up, fov_x);
  }
  register_camera(name, camera);

}

void
SdfLoader::request_definition()
{
  SdfLoader::ObjectType obj = read_object_type();
  switch (obj) {
    case SdfLoader::ObjectType::SHAPE:
      define_shape();
      break;
    case SdfLoader::ObjectType::LIGHT:
      define_light();
      break;
    case SdfLoader::ObjectType::MATERIAL:
      define_material();
      break;
    case SdfLoader::ObjectType::CAMERA:
      define_camera();
      break;
    default:
      break;
  }
}

void
SdfLoader::request_transform()
{
  auto target_name = read_string();
  auto object_type = get_registered_object(target_name);
  auto transform_type = read_transform_type();

  switch (transform_type) {

    case SdfLoader::TransformationType::TRANSLATE: {
      switch (object_type) {
        case SdfLoader::ObjectType::SHAPE: {
          auto target = get_shape(target_name);
          request_translation<Shape>(target);
          break;
        }
        case SdfLoader::ObjectType::CAMERA: {
          auto target = get_camera(target_name);
          request_translation<Camera>(target);
          break;
        }
        case SdfLoader::ObjectType::LIGHT: {
          auto target = get_light(target_name);
          request_translation<Light>(target);
          break;
        }
        default :
          print_error(SdfLoader::ErrorType::NO_TRANSLATION);
          break;
      }
      break;
    }

    case SdfLoader::TransformationType::ROTATE: {
      switch (object_type) {
        case SdfLoader::ObjectType::SHAPE: {
          auto target = get_shape(target_name);
          request_rotation<Shape>(target);
          break;
        }
        case SdfLoader::ObjectType::CAMERA: {
          auto target = get_camera(target_name);
          request_rotation<Camera>(target);
          break;
        }
        case SdfLoader::ObjectType::LIGHT: {
          auto target = get_light(target_name);
          request_rotation<Light>(target);
          break;
        }
        default : {
          print_error(SdfLoader::ErrorType::NO_ROTATION);
          break;
        }
      }
      break;
    }

    case SdfLoader::TransformationType::SCALE: {
      switch (object_type) {
        case SdfLoader::ObjectType::SHAPE: {
          auto target = get_shape(target_name);
          request_scale<Shape>(target);
          break;
        }
        default: {
          print_error( SdfLoader::ErrorType::NO_SCALING);
          break;
        }
      }
      break;
    }

    default:
      break;
  }
}

void
SdfLoader::request_render_instruction()
{
  auto cam_name = read_string();
  auto target_path = read_string();
  auto res_x = read_unsigned();
  auto res_y = read_unsigned();

  scene_->camera(get_camera(cam_name));

  create_scene_graph();

  render_instructions_.push(RenderInstruction(
    scene_,
    target_path,
    res_x,
    res_y
    ));
}

void
SdfLoader::request_command()
{
  SdfLoader::CommandType cmd = read_command_type();
  switch (cmd) {
    case SdfLoader::CommandType::DEFINITION:
      request_definition();
      break;
    case SdfLoader::CommandType::TRANSFORMATION:
      request_transform();
      break;
    case SdfLoader::CommandType::RENDER:
      request_render_instruction();
      break;
    default:
      break;
  }
}

void
SdfLoader::print_error(SdfLoader::ErrorType const& error)
{
  print_error(current_line_.tellg(), error);
}

void
SdfLoader::print_error(int err_pos, SdfLoader::ErrorType const& error)
{
  std::string error_msg = error_messages.at(error);

  std::cerr << "ERROR: '"<< error_msg << "' at:" << std::endl;
  std::cerr << current_line_.str() << std::endl;
  for (int i=0; i<err_pos; ++i) {
    std::cerr << " ";
  }
  std::cerr << "^" << std::endl;

  finished_parsing_ = true;
}