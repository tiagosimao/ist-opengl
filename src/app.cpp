#define GLFW_INCLUDE_GLCOREARB

#include "spdlog/spdlog.h"

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>

int wWidth = 640, wHeight = 480;

GLFWwindow* window = NULL;

#define VERTICES 0
#define COLORS 1
GLuint VaoId, VboId[2];
GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;

auto logger = spdlog::stdout_color_mt("main");

/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		logger->error("OpenGL ERROR [{}].", errString);
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if(isOpenGLError()) {
		logger->error("{}",error);
		exit(EXIT_FAILURE);
	}
}

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

/////////////////////////////////////////////////////////////////////// SHADERs

const GLchar* VertexShader =
{
	"#version 330 core\n"

	"in vec4 in_Position;\n"
	"in vec4 in_Color;\n"
	"out vec4 ex_Color;\n"

	"uniform mat4 Matrix;\n"

	"void main(void)\n"
	"{\n"
	"	gl_Position = Matrix * in_Position;\n"
	"	ex_Color = in_Color;\n"
	"}\n"
};

const GLchar* FragmentShader =
{
	"#version 330 core\n"

	"in vec4 ex_Color;\n"
	"out vec4 out_Color;\n"

	"void main(void)\n"
	"{\n"
	"	out_Color = ex_Color;\n"
	"}\n"
};

void createShaderProgram()
{
	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(VertexShaderId, 1, &VertexShader, 0);
	glCompileShader(VertexShaderId);

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
	glCompileShader(FragmentShaderId);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	glBindAttribLocation(ProgramId, VERTICES, "in_Position");
	glBindAttribLocation(ProgramId, COLORS, "in_Color");
	glLinkProgram(ProgramId);
	UniformId = glGetUniformLocation(ProgramId, "Matrix");

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	glUseProgram(0);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteProgram(ProgramId);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

typedef struct
{
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

const Vertex Vertices[] =
{
	{{ 0.25f, 0.25f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
	{{ 0.75f, 0.25f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
	{{ 0.50f, 0.75f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }}
};

const GLubyte Indices[] =
{
	0,1,2
};

void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(2, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(COLORS);
		glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertices[0].XYZW));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{

  glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(2, VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");

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

/////////////////////////////////////////////////////////////////////// SCENE

typedef GLfloat Matrix[16];

const Matrix I = {
	1.0f,  0.0f,  0.0f,  0.0f,
	0.0f,  1.0f,  0.0f,  0.0f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
}; // Row Major (GLSL is Column Major)

const Matrix M = {
	1.0f,  0.0f,  0.0f, -1.0f,
	0.0f,  1.0f,  0.0f, -1.0f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
}; // Row Major (GLSL is Column Major)


void draw()
{
  float ratio;
  int width, height;
  glfwGetFramebufferSize(window, &wWidth, &wHeight);
  ratio = wWidth / (float) wHeight;
  glViewport(0, 0, wWidth, wHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindVertexArray(VaoId);
	glUseProgram(ProgramId);

	glUniformMatrix4fv(UniformId, 1, GL_TRUE, I);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glUniformMatrix4fv(UniformId, 1, GL_TRUE, M);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
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

void setupGLEW()
{
	glewExperimental = GL_TRUE;
	GLenum result = glewInit() ;
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
}

int init()
{
  int result = glfwInit();
  glfwSetErrorCallback(error_callback);
  createWindow();
  setupGLEW();
  setupOpenGL();
  createShaderProgram();
	createBufferObjects();
  return result;
}

void shutdown(int code)
{
  logger->info("Shutting down");
  destroyShaderProgram();
	destroyBufferObjects();
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
