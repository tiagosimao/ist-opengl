#include "spdlog/spdlog.h"

#include "GLFW/glfw3.h"

#include <iostream>
#include <chrono>
#include <thread>

int wWidth = 640, wHeight = 480;

GLFWwindow* window = NULL;

auto logger = spdlog::stdout_color_mt("main");

static void error_callback(int error, const char* description)
{
    logger->info("Error %s", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int createWindow(){
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
  glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
  glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

  window = glfwCreateWindow(wWidth, wHeight, "Hello Darkness My Old Friend", NULL, NULL);
  if (!window)
  {
      logger->error("Error opening window");
      return 0;
  }
  glfwSetKeyCallback(window, key_callback);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  return 1;
}

void loop(){
  while (!glfwWindowShouldClose(window))
  {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

int initApp()
{
  int result = 0;
  result = glfwInit();
  if(result){
    glfwSetErrorCallback(error_callback);
    result = createWindow();
  }
  return result;
}

int main(int argc, char * argv[])
{
  logger->info("Booting app");
  if(initApp()){
    logger->info("App is running");
    loop();
    logger->info("Shutting down");
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
  } else {
    logger->error("Error booting app");
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
}
