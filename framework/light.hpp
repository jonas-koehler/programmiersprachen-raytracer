#ifndef BUW_LIGHT_HPP
#define BUW_LIGHT_HPP

class Light
{
public:
  Light();
  Light(Color ambient, Color diffuse);

  void translate(glm::vec3 const& t);

  glm::vec3 const& position() const;

  Color const& ld() const;
  Color const& la() const;

private:
  glm::vec3 p_;
  glm::mat4 t_;
  glm::mat4 t_inv_;
  Color la_;
  Color ld_;
};

#endif // BUW_LIGHT_HPP
