#include <thread>
#include <fensterchen.hpp>


int main(int argc, char* argv[])
{
  unsigned const width = 600;
  unsigned const height = 600;
  std::string const filename = "./checkerboard.ppm";

  bool multithreading = true;

  auto sampler = std::make_shared<RotatedGridSampler>(width, height, 29.5f);

  auto dark_red = std::make_shared<Material>(
    Color(1.0f),
    Color(0.6f, 0.3f, 0.2f),
    Color(0.0f),
    200.0f,
    0.0f,
    1.0f
  );

  auto blue = std::make_shared<Material>(
    Color(1.0f),
    Color(0.2f, 0.6f, 1.0f),
    Color(0.2f),
    200.0f,
    0.0f,
    1.0f
  );

  auto glas = std::make_shared<Material>(
    Color(0.0f),
    Color(0.0f),
    Color(0.1f),
    200.0f,
    0.9f,
    1.6f
  );

  Scene scene;





  auto boxes = std::make_shared<Composite>();
  for (signed i=0; i<=1000; ++i) {
    auto box = std::make_shared<Box>(blue);
    box->scale(glm::vec3(1));
    box->translate(glm::vec3 (4* i +2, 0, 0));
    boxes->add_child(std::move(box));
  }

  boxes->rotate(1.1, glm::vec3(0,1,0));
  boxes->scale(glm::vec3(0.2));
  boxes->optimize();


  auto spheres = std::make_shared<Composite>();
  for (signed i=0; i<=1000; ++i) {
    auto sphere = std::make_shared<Sphere>(blue);
    sphere->scale(glm::vec3(1));
    sphere->translate(glm::vec3 (-4* i -2, 0, 0));
    spheres->add_child(std::move(sphere));
  }

  spheres->rotate(-1.1, glm::vec3(0,1,0));
  spheres->scale(glm::vec3(0.2));
  spheres->optimize();

  auto big = std::make_shared<Composite>();
  big->add_child(boxes);
  big->add_child(spheres);


  auto all = std::make_shared<Composite>();

  auto floor = std::make_shared<Box>(dark_red);
  floor->scale(glm::vec3(100,1,100));
  floor->translate(glm::vec3(0,-2,0));


  auto balls = std::make_shared<Composite>();

  auto ball_1 = std::make_shared<Sphere>(blue);
  ball_1->translate(glm::vec3(2,1, -2));

  auto ball_2 = std::make_shared<Sphere>(glas);
  //ball_2->scale(glm::vec3(1, 1, 0.1));
  ball_2->rotate(2.65f, glm::vec3(0,1,0));
  ball_2->translate(glm::vec3(-5,1, -5));

  balls->add_child(ball_2);
  balls->add_child(ball_1);
  //balls->optimize();

  all->add_child(balls);
  all->add_child(floor);
  all->add_child(big);
  //all->add_child(boxes);
  //all->optimize();

  scene.add_shape(all);
  scene.add_shape(boxes);

  Light light(
    Color(0.1f),
    Color(1.0f)
  );

  light.translate(glm::vec3(100, 100, 100));

  Camera camera(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 60.0f);
  // Camera camera;
  camera.rotate(-0.9, glm::vec3(1, 0,0));
  camera.translate(glm::vec3(0, 10.0f, 10));



  scene.camera(camera);
  scene.add_light(light);

  Renderer app(width, height, filename, scene, sampler, multithreading);

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
