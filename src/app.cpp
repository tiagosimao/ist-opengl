#include "spdlog/spdlog.h"

#include "GLFW/glfw3.h"

#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>

int wWidth = 640, wHeight = 480;

GLFWwindow* window = NULL;

auto logger = spdlog::stdout_color_mt("main");

static void error_callback(int error, const char* description)
{
    logger->info("Error ({}) {}", error, description);
}

static void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
}

int createWindow()
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
      logger->error("Error opening window");
      return GL_FALSE;
  }
  glfwSetKeyCallback(window, key_callback);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // vSync

  return GL_TRUE;
}

void draw()
{
  float ratio;
  int width, height;
  glfwGetFramebufferSize(window, &wWidth, &wHeight);
  ratio = wWidth / (float) wHeight;
  glViewport(0, 0, wWidth, wHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void loop()
{
  int frameCount = 0;
  double lastTime = glfwGetTime();
  while (!glfwWindowShouldClose(window))
  {
    draw();
    glfwSwapBuffers(window);
    glfwPollEvents();
    ++frameCount;
    double elapsed = glfwGetTime() - lastTime;
    if(elapsed > 1.0){
      std::ostringstream sb;
      sb << "Hello Darkness My Old Friend: " << frameCount << " FPS @ (" << wWidth << "x" << wHeight << ")";
      glfwSetWindowTitle(window, sb.str().c_str());
      frameCount=0;
      lastTime+=elapsed;
    }
  }
}

void checkOpenGLInfo()
{
  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *vendor = glGetString(GL_VENDOR);
  const GLubyte *version = glGetString(GL_VERSION);
  const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
  logger->info("OpenGL Renderer:  ({})", renderer, vendor);
  logger->info("OpenGL version: {}", version);
  logger->info("GLSL version: {}", glslVersion);
}

int setupOpenGL()
{
	checkOpenGLInfo();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
  return GL_TRUE;
}

int init()
{
  int result = glfwInit();
  glfwSetErrorCallback(error_callback);
  result = result && createWindow() && setupOpenGL();
  return result;
}

void shutdown(int code)
{
  logger->info("Shutting down");
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(code);
}

int main(int argc, char * argv[])
{

  logger->info("Booting app");
  if(init()){
    logger->info("App is running");
    loop();
    shutdown(EXIT_SUCCESS);
  } else {
    logger->error("Error booting app");
    shutdown(EXIT_FAILURE);
  }
}
