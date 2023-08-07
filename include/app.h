/**
 * @file app.h
 * @author your name (you@domain.com)
 * @brief
 * [Hello, BGFX! - That One Game
 * Dev](https://thatonegamedev.com/cpp/hello-bgfx/)
 * @version 0.1
 * @date 2023-08-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <string>

#include "bgfx_glfw.h"

class BaseApp {
 public:
  virtual void init() = 0;
  virtual bool update() = 0;
  virtual int shutdown() = 0;
};

bool InitGLFW();
int ShutdownGLFW();
bool InitBGFX(GLFWwindow* _window, int _width, int _height);
int ShutdownBGFX();
