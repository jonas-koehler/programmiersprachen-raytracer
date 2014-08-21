#ifndef BUW_LIGHT_HPP
#define BUW_LIGHT_HPP

class Light
{
public:
  Light(Color ambient, Color diffuse);

  void translate(glm::vec3 const& t);
  void scale(glm::vec3 const& s);
  void rotate(float deg, glm::vec3 const& axis);

  Color const& ld() const;
  Color const& la() const;

private:
  glm::mat4 t_;
  glm::mat4 t_inv_;
  Color la_;
  Color ld_;
};

#endif // BUW_LIGHT_HPP
