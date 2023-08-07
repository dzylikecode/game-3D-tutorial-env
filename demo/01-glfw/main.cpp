
#include <bx/bx.h>
#include <spdlog/spdlog.h>
#include <stdio.h>

#include "app.h"

class GLFWApp : public BaseApp {
 public:
  GLFWApp(std::string title, int width, int height)
      : m_title(title), m_width(width), m_height(height) {}
  virtual void init() override {
    if (!InitGLFW()) {
      exit(bx::kExitFailure);
    }

    m_window =
        glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

    if (!m_window) {
      SPDLOG_ERROR("Failed to create GLFW window.");
      glfwTerminate();
      exit(bx::kExitFailure);
    }
  }

  virtual bool update() override {
    glfwPollEvents();
    return !glfwWindowShouldClose(m_window);
  }

  virtual int shutdown() override {
    glfwDestroyWindow(m_window);
    return ShutdownGLFW();
  }

 private:
  std::string m_title;
  int m_width;
  int m_height;
  GLFWwindow* m_window;
};

int RunApp(BaseApp* app) {
  app->init();
  while (app->update()) {
  }

  return app->shutdown();
}

int main(int argc, const char* const* argv) {
  SPDLOG_INFO("Hello, world!\n");
  GLFWApp app("Hello, world!", 1280, 720);
  return RunApp(&app);
}
