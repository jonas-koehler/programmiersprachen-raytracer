#include <thread>
#include <fensterchen.hpp>

#include <random>
#include <sstream>

int main(int argc, char* argv[])
{
  unsigned const width = 600;
  unsigned const height = 600;
  std::string const filename = "./checkerboard.ppm";

  if (argc < 2) {
    std::cerr << "usage: raytracer <sdf-file>" << std::endl;
    return 0;
  }

  std::string sdf_file(argv[1]);

  bool window = true;

  auto sampler = std::make_shared<RotatedGridSampler>(width, height, 29.5f);

  auto dark_red = std::make_shared<Material>(
    Color(1.0f),
    Color(0.6f, 0.3f, 0.2f),
    Color(0.1f),
    200.0f,
    1.00f,
    1.0f
  );

  auto yellow = std::make_shared<Material>(
    Color(1.0f),
    Color(1.0f, 0.8f, 0.0f),
    Color(0.3f),
    200.0f,
    1.0f,
    1.0f
  );

  auto water = std::make_shared<Material>(
    Color(1.0f),
    Color(0.0f, 0.8f, 1.0f),
    Color(0.5f),
    200.0f,
    0.3,
    1.33f
  );

  auto scene = std::make_shared<Scene>();





  /*auto boxes = std::make_shared<Composite>();
  for (signed i=0; i<=1000; ++i) {
    auto box = std::make_shared<Box>(dark_red);
    box->scale(glm::vec3(1));
    box->translate(glm::vec3 (4* i +2, 0, 0));
    boxes->add_child(std::move(box));
  }

  boxes->rotate(1.1, glm::vec3(0,1,0));
  boxes->scale(glm::vec3(0.2));
  boxes->optimize();


  auto spheres = std::make_shared<Composite>();
  for (signed i=0; i<=1000; ++i) {
    auto sphere = std::make_shared<Sphere>(dark_red);
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




  auto balls = std::make_shared<Composite>();

  auto ball_1 = std::make_shared<Sphere>(blue);
  ball_1->translate(glm::vec3(2,1, -2));

  auto ball_2 = std::make_shared<Sphere>(glas);
  ball_2->scale(glm::vec3(1, 1, 0.1));
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
  scene.add_shape(boxes);*/

  /*auto

  */

  // unsigned lw = 6;
  // unsigned lh = 6;

  // auto landscape = std::make_shared<Composite>();

  // std::vector<glm::vec3> points(lw*lh);

  // std::default_random_engine generator;
  // std::uniform_real_distribution<double> distribution(0.0,1.0);

  // unsigned test = 0;

  // for (unsigned x=0; x < lw; ++x) {
  //   for (unsigned y=0; y < lh; ++y) {

  //     unsigned i_3 = y * lw + x;

  //     points[i_3] = glm::vec3(1.0f / (double) (lw-1), 0.3f, 1.0f / (double) (lh-1)) * glm::vec3(x, distribution(generator), y);
  //     //points[i_3] = 1.0f * glm::vec3(x, 3, y);

  //     if (x == 0 || y == 0) { continue; }

  //     unsigned i_0 = (y-1) * lw + (x-1);
  //     unsigned i_1 = (y-1) * lw + x;
  //     unsigned i_2 = y * lw + (x-1);


  //     std::array<glm::vec3, 3> v_1{
  //       points[i_0],
  //       points[i_3],
  //       points[i_1]
  //     };

  //     std::array<glm::vec3, 3> v_2{
  //      points[i_0],
  //      points[i_2],
  //      points[i_3]
  //     };

  //     auto t_1 = std::make_shared<Triangle>(water, v_1);
  //     auto t_2 = std::make_shared<Triangle>(water, v_2);

  //     test += 1;

  //     landscape->add_child(std::move(t_1));
  //     landscape->add_child(std::move(t_2));
  //   }
  // }

  // landscape->optimize();

  // landscape->scale(glm::vec3(20, 1 , 20));

  // landscape->translate(glm::vec3(-10.0f, -1.0f, -10.0f));


  // auto floor = std::make_shared<Box>(dark_red);
  // floor->scale(glm::vec3(10,1,10));
  // floor->translate(glm::vec3(0,-4,0));

  // auto all = std::make_shared<Composite>();

  // auto box = std::make_shared<Box>(yellow);
  // box->translate(glm::vec3(4,1,0));

  // auto sphere = std::make_shared<Sphere>(yellow);
  // sphere->translate(glm::vec3(-4,1,0));
  // sphere->scale(glm::vec3(1,5,1));


  // std::array<glm::vec3, 3> v{
  //   glm::vec3(-1,1,0),
  //   glm::vec3(0,2,0),
  //   glm::vec3(1,1,0)
  // };

  /*auto cone = std::make_shared<Cone>(yellow);
  cone->scale(glm::vec3(2, 2, 2));
  cone->rotate(1.7, glm::vec3(0, 1, 0));
  cone->translate(glm::vec3(0, 0, -9));*/



  //all->add_child(box);
  //all->add_child(sphere);
  //all->add_child(cone);
  //all->add_child(landscape);
  //all->add_child(floor);



  //all->optimize();

  //scene.add_shape(all);


  auto cone = std::make_shared<Cone>(yellow);
  cone->scale(glm::vec3(10, 10, 10));
  cone->scale(glm::vec3(0.2));
  cone->rotate(-1.6 + 3.14, glm::vec3(1,0,0));
  cone->translate(glm::vec3(0, 0, -11));

  //cone->rotate(M_PI / 4, glm::vec3(0,0,1));

  scene->add_shape(cone);

  auto light = std::make_shared<Light>(
    Color(0.1f),
    Color(1.0f)
  );

  light->translate(glm::vec3(0, 5, -5));

  /*auto camera = std::make_shared<Camera>(
    glm::vec3(0.0f),
    glm::vec3(0.0f, 0.0f, -1.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    60.0f
  );*/

  auto camera = std::make_shared<Camera>();

  //camera->rotate(-0.3, glm::vec3(1, 0,0));

  //glm::vec3 cam_pos(0, 10.0f, 25.0f);
  //camera->translate(cam_pos);

  scene->camera(camera);
  scene->add_light(light);

  unsigned char options = Renderer::Option::MultiThreading /*| Renderer::Option::SuperSampling4x | Renderer::Option::RGSS */;


  Renderer app(options);

  bool done = false;

  SdfLoader loader(sdf_file);

  std::thread thr([&app, &loader, &done]() {
    int i = 0;
    while(loader.parse()) {
      if (loader.has_render_instructions()) {
        auto ri = loader.get_render_instruction();
        auto render_time = app.render(ri);
        std::cout << "rendered frame " << ++i << " to " << ri.outfile << " in " << render_time << "ms" << std::endl;
      }
    }
    done = true;
  });

  if (window) {
    Window win(glm::ivec2(width,height));
    while (!win.shouldClose()) {
      if (win.isKeyPressed(GLFW_KEY_ESCAPE) || done) {
        win.stop();
      }
      glDrawPixels( width, height, GL_RGB, GL_FLOAT
                  , app.colorbuffer().data());
      win.update();
    }
  }

  thr.join();

  return 0;
}
