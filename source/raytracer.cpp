#include <thread>
#include <fensterchen.hpp>

#include <random>
#include <sstream>

void print_help_msg() {
  std::cerr << "usage: raytracer <sdf-file> ";
  std::cerr << "[-ss 4 | 16] [-s grid | random | rgss] ";
  std::cerr << "[-w width height] [--multithreading] [--no-close]" << std::endl;
}

int main(int argc, char* argv[])
{
  unsigned width = 100;
  unsigned height = 100;
  bool window = false;
  bool no_close = false;

  unsigned char options = 0;

  if (argc < 2) {
    print_help_msg();
    return 1;
  }

  for (int i=2; i<argc; ++i) {
    if (strcmp(argv[i], "-ss") == 0 && i+1 < argc) {
      if (!strcmp(argv[i+1], "4")) {
        options = options | Renderer::Option::SuperSampling4x;
      } else if (!strcmp(argv[i+1], "16")) {
        options = options | Renderer::Option::SuperSampling16x;
      } else {
        std::cerr << "unknown argument" << std::endl;
        print_help_msg();
        return 1;
      }
      ++i;
    } else if (!strcmp(argv[i], "-s") && i+1 < argc) {
      if (!strcmp(argv[i+1], "grid")) {
        options = options | Renderer::Option::GridSampling;
      } else if (!strcmp(argv[i+1], "random")) {
        options = options | Renderer::Option::RandomSampling;
      } else if (!strcmp(argv[i+1], "rgss")) {
        options = options | Renderer::Option::RGSS;
      } else {
        std::cerr << "unknown argument" << std::endl;
        print_help_msg();
        return 1;
      }
      ++i;
    } else if (!strcmp(argv[i], "--multithreading")) {
      options = options | Renderer::Option::MultiThreading;
    } else if (!strcmp(argv[i], "--no-close")) {
      no_close = true;
    } else if (!strcmp(argv[i], "-w") && i+2 < argc) {
      std::stringstream ss(argv[i+1]);
      if (!(ss >> width)) {
        std::cerr << "invalid window width" << std::endl;
        print_help_msg();
        return 1;
      }
      ss = std::stringstream(argv[i+2]);
      if (!(ss >> height)) {
        std::cerr << "invalid window height" << std::endl;
        print_help_msg();
        return 1;
      }
      window = true;
      i+=2;
    } else {
      std::cerr << "unknown flag or empty argument" << std::endl;
      print_help_msg();
      return 1;
    }
  }

  std::string sdf_file(argv[1]);
  SdfLoader loader(sdf_file);

  Renderer app(width, height, options);

  bool done = false;
  bool parsing_message = true;

  std::thread thr([&app, &loader, &done, &window, &width, &height, &parsing_message, &no_close]() {
    int i = 0;
    int j = 0;
    while(loader.parse()) {

      if (parsing_message) {
        std::cout << "parsing scene..." << std::endl;
        parsing_message = false;
      }

      if (loader.has_render_instructions()) {
        parsing_message = true;

        auto ri = loader.get_render_instruction();
        if (window) {
          ri.res_x = width;
          ri.res_y = height;
        }

        auto render_time = app.render(ri);

        std::cout << "rendered frame " << ++i << " to " << ri.outfile << " in " << render_time << "ms" << std::endl;
      }
    }
    if (!no_close) {
      done = true;
    }
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
