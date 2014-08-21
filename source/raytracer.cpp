#include <thread>
#include <fensterchen.hpp>


int main(int argc, char* argv[])
{
  unsigned const width = 600;
  unsigned const height = 600;
  std::string const filename = "./checkerboard.ppm";

  std::shared_ptr<StandardGridSampler> sampler = std::make_shared<StandardGridSampler>(width, height);

  Scene scene;

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
