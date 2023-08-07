
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

 public:
  std::string m_title;
  int m_width;
  int m_height;
  GLFWwindow* m_window;
};

class BGFXApp : public BaseApp {
 public:
  BGFXApp(std::string title, int width, int height)
      : m_glfw(title, width, height) {}
  virtual void init() override {
    m_glfw.init();
    if (!InitBGFX(m_glfw.m_window, m_glfw.m_width, m_glfw.m_height)) {
      exit(bx::kExitFailure);
    }

    // Set view 0 clear state.
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00ff00ff, 1.0f,
                       0);
  }

  virtual bool update() override {
    // Set view 0 default viewport.
    bgfx::setViewRect(0, 0, 0, uint16_t(m_glfw.m_width),
                      uint16_t(m_glfw.m_height));

    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::touch(0);
    // Advance to next frame. Rendering thread will be kicked to
    // process submitted rendering primitives.
    bgfx::frame();
    return m_glfw.update();
  }

  virtual int shutdown() override {
    if (int exitCode = ShutdownBGFX(); exitCode != 0) return exitCode;
    return m_glfw.shutdown();
  }

 private:
  GLFWApp m_glfw;
};

int RunApp(BaseApp* app) {
  app->init();
  while (app->update()) {
  }

  return app->shutdown();
}

int main(int argc, const char* const* argv) {
  SPDLOG_INFO("Hello, world!\n");
  BGFXApp app("Hello, world!", 1280, 720);
  return RunApp(&app);
}
