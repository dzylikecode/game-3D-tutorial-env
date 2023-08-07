
#include <bx/bx.h>
#include <spdlog/spdlog.h>
#include <stdio.h>

#include "app.h"
#include "logo.h"

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
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f,
                       0);
  }

  virtual bool update() override {
    // Set view 0 default viewport.
    bgfx::setViewRect(0, 0, 0, uint16_t(m_glfw.m_width),
                      uint16_t(m_glfw.m_height));

    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::touch(0);
    // Use debug font to print information about this example.
    bgfx::dbgTextClear();
    const bgfx::Stats* stats = bgfx::getStats();

    bgfx::dbgTextImage(
        bx::max<uint16_t>(uint16_t(stats->textWidth / 2), 20) - 20,
        bx::max<uint16_t>(uint16_t(stats->textHeight / 2), 6) - 6, 40, 12,
        s_logo, 160);
    bgfx::dbgTextPrintf(0, 1, 0x0f,
                        "Color can be changed with ANSI "
                        "\x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b["
                        "14;me\x1b[0m code too.");

    bgfx::dbgTextPrintf(
        80, 1, 0x0f,
        "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; "
        "5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
    bgfx::dbgTextPrintf(
        80, 2, 0x0f,
        "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    "
        "\x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");

    bgfx::dbgTextPrintf(
        0, 2, 0x0f,
        "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.",
        stats->width, stats->height, stats->textWidth, stats->textHeight);

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
