#include "app.h"

#include <spdlog/spdlog.h>

bool InitGLFW() {
  glfwSetErrorCallback([](int error, const char* description) {
    SPDLOG_ERROR("GLFW error: {} - {}", error, description);
  });

  if (!glfwInit()) {
    SPDLOG_ERROR("Failed to initialize GLFW.");
    return false;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  return true;
}

int ShutdownGLFW() {
  glfwTerminate();
  return 0;
}

bool InitBGFX(GLFWwindow* _window, int _width, int _height) {
  bgfx::renderFrame();

  bgfx::Init init;
  init.type = bgfx::RendererType::Count;
  init.vendorId = BGFX_PCI_ID_NONE;
  init.platformData.nwh = glfwNativeWindowHandle(_window);
  init.platformData.ndt = getNativeDisplayHandle();
  init.resolution.width = _width;
  init.resolution.height = _height;
  init.resolution.reset = BGFX_RESET_VSYNC;
  bgfx::init(init);

  // Enable debug text.
  bgfx::setDebug(BGFX_DEBUG_TEXT);

  return true;
}

int ShutdownBGFX() {
  // see it in `entry_window.cpp` `entry_sdl.cpp` except entry_glfw.cpp, I don't
  // know its function. I had added it to program, but it will trap in while
  // loop, so I comment it
  // while (bgfx::RenderFrame::NoContext != bgfx::renderFrame()) {
  // };
  bgfx::shutdown();
  return 0;
}