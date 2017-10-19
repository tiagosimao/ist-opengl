#include "spdlog/spdlog.h"
#include "GL/glew.h"
#include "draw.hpp"
#include "cgj-math/mat.hpp"
#include <math.h>

auto drawLogger = spdlog::stdout_color_mt("draw");

#define VERTICES 0
#define COLORS 1
GLuint VaoId, VboId[2];
GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;

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
	{{ -0.5f, -0.25f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
	{{ 0.5f, -0.25f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
	{{ 0.0f, 0.25f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }}
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

typedef GLfloat Matrix[16];

Matrix I = {
	1.0f,  0.0f,  0.0f,  -1.0f,
	0.0f,  1.0f,  0.0f,  0.0f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
}; // Row Major (GLSL is Column Major)

float M[16] = {
	1.0f,  0.0f,  0.0f, 0.0f,
	0.0f,  1.0f,  0.0f, 1.0f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
}; // Row Major (GLSL is Column Major)

Matrix N = {
	0.5f,  0.0f,  0.0f, 0.0f,
	0.0f,  0.5f,  0.0f, -0.5f,
	0.0f,  0.0f,  0.5f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
}; // Row Major (GLSL is Column Major)

Matrix O = {
	0.5f,  0.0f,  0.0f, 1.0f,
	0.0f,  0.5f,  0.0f, 1.0f,
	0.0f,  0.0f,  0.5f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
}; // Row Major (GLSL is Column Major)



void draw::draw()
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindVertexArray(VaoId);
	glUseProgram(ProgramId);

  Mat t1;
  t1=t1.scale(1.0f);
  t1=t1.rotZ(-M_PI/2);
  t1=t1.move(-0.25f,0.0f,0.0f);
	glUniformMatrix4fv(UniformId, 1, GL_TRUE, t1.toGl());
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  Mat t2;
  t2=t2.scale(1.0f);
  t2=t2.rotZ(M_PI);
  t2=t2.move(0.0f,0.25f,0.0f);
  glUniformMatrix4fv(UniformId, 1, GL_TRUE, t2.toGl());
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  Mat t3;
  t3=t3.rotZ(M_PI/2);
  t3=t3.scale(0.5f);
  t3=t3.move(0.75f,0.5f,0.0f);
  glUniformMatrix4fv(UniformId, 1, GL_TRUE, t3.toGl());
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  Mat t4;
  t4=t4.move(0.375f,-0.375f,0.0f);
  t4=t4.scale(0.7f);
  t4=t4.rotZ(-3.0f*M_PI/4.0f);

  glUniformMatrix4fv(UniformId, 1, GL_TRUE, t4.toGl());
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);


  Mat t5;
  //t5=t5.rotZ(-3.0f*M_PI/4.0f);
  t5=t5.scale(0.5f);
  t5=t5.move(0.0f,-0.25f,0.0f);
  glUniformMatrix4fv(UniformId, 1, GL_TRUE, t5.toGl());
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);


/*
	glUniformMatrix4fv(UniformId, 1, GL_TRUE, M);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  glUniformMatrix4fv(UniformId, 1, GL_TRUE, N);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

  glUniformMatrix4fv(UniformId, 1, GL_TRUE, O);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);
*/
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
