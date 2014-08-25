#include <thread>
#include <fensterchen.hpp>


int main(int argc, char* argv[])
{
  unsigned const width = 600;
  unsigned const height = 600;
  std::string const filename = "./checkerboard.ppm";

  auto sampler = std::make_shared<RotatedGridSampler>(width, height, 29.5f);

  auto white = std::make_shared<Material>(
    Color(0.1f, 0.1f, 0.1f),
    Color(0.7f, 0.7f, 0.7f),
    Color(0.3f, 0.3f, 0.3f),
    200.0f,
    0.0f,
    1.0f
  );

  Scene scene;

  auto boxes = std::make_shared<Composite>();

  for (signed i=0; i<=2000; ++i) {
    auto box = std::make_shared<Box>(white);
    box->translate(glm::vec3 (4* i -2, 0, 0));
    boxes->add_child(std::move(box));
  }

  boxes->optimize();

  boxes->rotate(2.4, glm::vec3(0,1,0));
  boxes->translate(glm::vec3(0,0,-10));

  auto box = std::make_shared<Box>(white);
  box->translate(glm::vec3(0,0,-5));

  scene.add_shape(std::move(boxes));

  //scene.add_shape(box);

  Light light(
    Color(0.1f, 0.1f, 0.1f),
    Color(1.0f, 1.0f, 1.0f)
  );

  light.translate(glm::vec3(0, 100, 0));

  Camera camera(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 60.0f);
  // Camera camera;
  // camera.rotate(0.2, glm::vec3(1, 0,0));
  camera.translate(glm::vec3(0, 10.0f, 20));



  scene.camera(camera);
  scene.add_light(light);

  Renderer app(width, height, filename, scene, sampler);

  std::thread thr([&app]() { app.render(); });

  Window win(glm::ivec2(width,height));

  while (!win.shouldClose()) {
    if (win.isKeyPressed(GLFW_KEY_ESCAPE)) {
      win.stop();
    }

    glDrawPixels( width, height, GL_RGB, GL_FLOAT
                , app.colorbuffer().data());

    win.update();
  }

  thr.join();

  return 0;
}
