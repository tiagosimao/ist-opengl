#define GLFW_INCLUDE_GLCOREARB

#include "spdlog/spdlog.h"

#include "GLFW/glfw3.h"

#include "draw.hpp"
#include "window.hpp"

#include <sstream>
#include <iostream>

auto logger = spdlog::stdout_color_mt("main");

void loop()
{
  int frameCount = 0, width, height;
  double lastTime = glfwGetTime(), mouseX, mouseY;
  while (!window::shouldClose())
  {
    window::getSize(&width, &height);
    window::getCursorPos(&mouseX,&mouseY);
    glViewport(0, 0, width, height);
    draw::draw(width, height, mouseX, mouseY);
		window::swapBuffers();
    glfwPollEvents();
    ++frameCount;
    double elapsed = glfwGetTime() - lastTime;
    if(elapsed > 1.0){
      std::ostringstream sb;
      sb << "Hello Darkness My Old Friend: " << frameCount << " FPS @ (" << width << "x" << height << ")";
			window::setTitle(sb.str().c_str());
      frameCount=0;
      lastTime+=elapsed;
    }
  }
}

bool init()
{
  return glfwInit() &&
	window::init() &&
	draw::init();
}

void shutdown(int code)
{
  logger->info("Shutting down");
	draw::shutdown();
	window::shutdown();
	glfwTerminate();
  exit(code);
}

int main(int argc, char * argv[])
{
  logger->info("Booting app with {} arguments", argc -1);
  if(init()){
    logger->info("App is running");
    loop();
    shutdown(EXIT_SUCCESS);
  } else {
    logger->error("Error booting app");
    shutdown(EXIT_FAILURE);
  }
}
