#include "spdlog/spdlog.h"
#include "draw.hpp"
#include "shader.hpp"
#include "cgj-math/mat.hpp"
#include <math.h>

auto drawLogger = spdlog::stdout_color_mt("draw");

GLuint VaoId, squareId;

/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		drawLogger->error("OpenGL ERROR [{}].", errString);
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if(isOpenGLError()) {
		drawLogger->error("{}",error);
		exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////////////////////////////////////// SHADERs

Shader shaderProgram("shaders/simpleVertex.shader", "shaders/simpleFragment.shader");

void createShaderProgram()
{
	shaderProgram.init();
	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	shaderProgram.shutdown();
	checkOpenGLError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

typedef struct
{
	GLfloat XYZW[4];
} Vertex;

const Vertex vertices[] =
{
	{{ -0.5f, -0.25f, 0.0f, 1.0f }},
	{{ 0.5f, -0.25f, 0.0f, 1.0f }},
	{{ 0.0f, 0.25f, 0.0f, 1.0f }},
	{{ 1.0f, 0.25f, 0.0f, 1.0f }}
};

const Vertex squareVertices[] =
{
	{{ -0.5f, -0.5f, 0.0f, 1.0f }},
	{{ 0.5f, -0.5f, 0.0f, 1.0f }},
	{{ -0.5f, 0.5f, 0.0f, 1.0f }},
	{{ 0.5f, 0.5f, 0.0f, 1.0f }}
};

const GLubyte Indices[] =
{
	0,1,2,3
};

void createBufferObjects()
{
	GLuint VboId[2];

	// a triangle strip
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(2, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
	}

	// a square
	glGenVertexArrays(1, &squareId);
	glBindVertexArray(squareId);
	{
		glGenBuffers(2, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

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
	//glDisableVertexAttribArray(VERTICES);
	//glDeleteBuffers(2, VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");

}

void checkOpenGLInfo()
{
  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *vendor = glGetString(GL_VENDOR);
  const GLubyte *version = glGetString(GL_VERSION);
  const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
  drawLogger->info("OpenGL Renderer:  ({})", renderer, vendor);
  drawLogger->info("OpenGL version: {}", version);
  drawLogger->info("GLSL version: {}", glslVersion);
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
		drawLogger->error("ERROR glewInit: {}", glewGetString(result));
		exit(EXIT_FAILURE);
	}
}


/////////////////////////////////////////////////////////////////////// SCENE

GLfloat red[4]={1.0f, 0.15f, 0.04f, 1.0f};
GLfloat orange[4]={1.0f, 0.4f, 0.09f, 1.0f};
GLfloat green[4]={0.35f, 0.5f, 0.15f, 1.0f};
GLfloat yellow[4]={1.0f, 0.9f, 0.0f, 1.0f};
GLfloat bordo[4]={0.6f, 0.08f, 0.0f, 1.0f};
GLfloat darkblue[4]={0.15f, 0.045f, 0.40f, 1.0f};
GLfloat blue[4]={0.175f, 0.26f, 0.625f, 1.0f};


int t = 0;
void draw::draw()
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindVertexArray(VaoId);
	glUseProgram(shaderProgram.ProgramId);

  Mat t1;
  t1=t1.scale(1.0f);
  t1=t1.rotZ(-M_PI/2);
  t1=t1.move(-0.25f,0.0f,0.0f);
	glUniformMatrix4fv(shaderProgram.uTransformMatrixId, 1, GL_TRUE, t1.toGl());
	glUniform4fv(shaderProgram.uColor, 1, orange);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  Mat t2;
  t2=t2.scale(1.0f);
  t2=t2.rotZ(M_PI);
  t2=t2.move(0.0f,0.25f,0.0f);
  glUniformMatrix4fv(shaderProgram.uTransformMatrixId, 1, GL_TRUE, t2.toGl());
	glUniform4fv(shaderProgram.uColor, 1, green);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  Mat t3;
  t3=t3.rotZ(M_PI/2);
  t3=t3.scale(0.5f);
  t3=t3.move(0.75f,0.5f,0.0f);
  glUniformMatrix4fv(shaderProgram.uTransformMatrixId, 1, GL_TRUE, t3.toGl());
	glUniform4fv(shaderProgram.uColor, 1, bordo);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  Mat t4;
  t4=t4.move(0.375f,-0.375f,0.0f);
  t4=t4.scale(0.71f);
  t4=t4.rotZ(-3.0f*M_PI/4.0f);
  glUniformMatrix4fv(shaderProgram.uTransformMatrixId, 1, GL_TRUE, t4.toGl());
	glUniform4fv(shaderProgram.uColor, 1, red);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);


  Mat t5;
  t5=t5.scale(0.5f);
  t5=t5.move(0.0f,-0.25f,0.0f);
  glUniformMatrix4fv(shaderProgram.uTransformMatrixId, 1, GL_TRUE, t5.toGl());
	glUniform4fv(shaderProgram.uColor, 1, darkblue);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

	Mat t6;
  t6=t6.scale(0.5f);
  t6=t6.move(-0.5f,-0.75f,0.0f);
  glUniformMatrix4fv(shaderProgram.uTransformMatrixId, 1, GL_TRUE, t6.toGl());
	glUniform4fv(shaderProgram.uColor, 1, blue);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glBindVertexArray(squareId);
	Mat t7;
	t7=t7.rotZ(M_PI/4.0f);
	t7=t7.move(0.25f,0.0f,0.0f);
  t7=t7.scale(0.354f);
  glUniformMatrix4fv(shaderProgram.uTransformMatrixId, 1, GL_TRUE, t7.toGl());
	glUniform4fv(shaderProgram.uColor, 1, yellow);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

bool draw::init()
{
  setupGLEW();
  setupOpenGL();
  createShaderProgram();
	createBufferObjects();
  return true;
}

bool draw::shutdown()
{
  destroyShaderProgram();
	destroyBufferObjects();
  return true;
}
