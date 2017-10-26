#include "GLFW/glfw3.h"
#include "window.hpp"
#include "draw.hpp";

namespace window {
  int wWidth = 400, wHeight = 400;
  GLFWwindow* window = NULL;

  static void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
  {
    draw::keyPressed(key,action);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
  }

  bool init()
  {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

    window = glfwCreateWindow(wWidth, wHeight, "Hello Darkness My Old Friend", NULL, NULL);
    if (!window)
    {
        return false;
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vSync
    return true;
  }

  bool shutdown()
  {
    glfwDestroyWindow(window);
    return true;
  }

  bool shouldClose()
  {
    return glfwWindowShouldClose(window);
  }

  void getSize(int *width,int *height)
  {
    glfwGetFramebufferSize(window,width,height);
  }

  void getCursorPos(double * xPos, double * yPos)
  {
    glfwGetCursorPos(window,xPos,yPos);
  }

  void swapBuffers()
  {
    glfwSwapBuffers(window);
  }

  void setTitle(const char *title)
  {
    glfwSetWindowTitle(window, title);
  }
}
