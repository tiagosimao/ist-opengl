#include "spdlog/spdlog.h"
#include "draw.hpp"
#include "shader.hpp"
#include "thing.hpp"
#include "cgj-math/mat.hpp"
#include "cgj-math/vec.hpp"
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

/////////////////////////////////////////////////////////////////////// View Matrices



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

Thing square("objects/square.json");

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
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glDisable(GL_CULL_FACE);
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


void drawThing(GLuint vaoId, GLenum mode, GLsizei count, GLfloat color[4], Mat modelMat, Mat viewMat, Mat projectionMat)
{
	glBindVertexArray(vaoId);

	glUniformMatrix4fv(shaderProgram.uModelMat, 1, GL_TRUE, modelMat.toGl());
	glUniformMatrix4fv(shaderProgram.uViewMat, 1, GL_TRUE, viewMat.toGl());
	glUniformMatrix4fv(shaderProgram.uProjectionMat, 1, GL_TRUE, projectionMat.toGl());
	glUniform4fv(shaderProgram.uColor, 1, color);
  glDrawElements(mode, count, GL_UNSIGNED_BYTE, (GLvoid*)0);
}

Vec eye(0,0,10);
Vec target(0,0,0);
Vec up(0,1,0);

void look(Mat &view, Vec eyeAt, Vec targetAt, Vec upAt)
{
	Vec viewZ(eye);
	viewZ=viewZ.sum(targetAt).normalise();
	Vec viewX(upAt);
	viewX=viewX.cross(viewZ).normalise();
	Vec viewY(viewZ);
	viewY=viewY.cross(viewX);

	float orientationArr[16]={
		viewX.a, viewY.a, viewZ.a, 0,
		viewX.b, viewY.b, viewZ.b, 0,
		viewX.c, viewY.c, viewZ.c, 0,
		0, 0, 0, 1
	};
	Mat orientation;
	orientation.set(orientationArr);

	float translationArr[16]={
		viewX.a, viewY.a, viewZ.a, 0,
		viewX.b, viewY.b, viewZ.b, 0,
		viewX.c, viewY.c, viewZ.c, 0,
		0, 0, 0, 1
	};
	Mat translation;
	translation.set(translationArr);

	view.set(orientation.mul(translation).m);

}

void project( Mat& m, float fov, float aspect, float near, float far)
{
	float D2R = M_PI / 180.0;
	float yScale = 1.0 / tan(D2R * fov / 2);
	float xScale = yScale / aspect;
	float nearmfar = near - far;
	float arr[] = {
			xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, (far + near) / nearmfar, -1.0f,
			0.0f, 0.0f, 2.0f*far*near / nearmfar, 0.0f 
	};    
	m.set(arr);
}

void projectOrtho(Mat & m, float l, float r, float t, float b, float n, float f)
{
	float arr[16] = {
		2/(r-l),  0.00f,  0.00f,  -(r+l)/(r-l),
		0.00f,  2/(t-b),  0.00f,  -(t+b)/(t-b),
		0.00f,  0.00f, -2/(f-n),  -(f+n)/(f-n),
		0.00f,  0.00f, 0.0f,  1.00f
	};
	m.set(arr);
}

int t = 0;
bool ortho = false;
void draw::draw(double width, double height, double mouseX, double mouseY)
{
	float ratio = width/height;

	t++;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	eye.a=10*(mouseX-width/2)/width;
	eye.b=10*(mouseY-height/2)/height;

	Mat viewMat;
  look(viewMat,eye,target,up);

	Mat projectionMat;

	if(ortho)
	{
		projectOrtho(projectionMat,-2,2,2,-2,0,-10);
	} else 
	{
		project(projectionMat, 10.0f, ratio, -2.0f, 100.0f);
	}
	
	float wobble = 0.00f;

	glUseProgram(shaderProgram.ProgramId);

	Mat t1;
  t1=t1.rotZ(-M_PI/2);
	t1=t1.move(-0.25f,0.0f,0.0f);
	t1=t1.move(0,0,1);
	t1=t1.move(0,sin(M_PI*(t/100.0f)) * wobble,0);
	drawThing(VaoId, GL_TRIANGLES, 3, orange, t1, viewMat, projectionMat);

	
  Mat t2;
  t2=t2.scale(1.0f);
  t2=t2.rotZ(M_PI);
	t2=t2.move(0.0f,0.25f,0.0f);
	t2=t2.move(0,0,2);
	t2=t2.move(0,sin(M_PI*(t/60.0f)) * wobble,0);
	drawThing(VaoId, GL_TRIANGLES, 3, green, t2, viewMat, projectionMat);

  Mat t3;
  t3=t3.rotZ(M_PI/2);
  t3=t3.scale(0.5f);
	t3=t3.move(0.75f,0.5f,0.0f);
	t3=t3.move(0,0,3);
	t3=t3.move(0,sin(M_PI*(t/30.0f)) * wobble,0);
	drawThing(VaoId, GL_TRIANGLES, 3, bordo, t3, viewMat, projectionMat);

  Mat t4;
  t4=t4.move(0.375f,-0.375f,0.0f);
  t4=t4.scale(0.71f);
	t4=t4.rotZ(-3.0f*M_PI/4.0f);
	t4=t4.move(0,0,4);
	t4=t4.move(0,sin(M_PI*(t/150.0f)) * wobble,0);
	drawThing(VaoId, GL_TRIANGLES, 3, red, t4, viewMat, projectionMat);

  Mat t5;
  t5=t5.scale(0.5f);
	t5=t5.move(0.0f,-0.25f,0.0f);
	t5=t5.move(0,0,5);
	t5=t5.move(0,sin(M_PI*(t/80.0f)) * wobble,0);
	drawThing(VaoId, GL_TRIANGLES, 3, darkblue, t5, viewMat, projectionMat);

	Mat t6;
  t6=t6.scale(0.5f);
	t6=t6.move(-0.5f,-0.75f,0.0f);
	t6=t6.move(0,0,6);
	t6=t6.move(0,sin(M_PI*(t/70.0f)) * wobble,0);
	drawThing(VaoId, GL_TRIANGLE_STRIP, 4, blue, t6, viewMat, projectionMat);
	
	Mat t7;
	t7=t7.rotZ(M_PI/4.0f);
	t7=t7.move(0.25f,0.0f,0.0f);
	t7=t7.scale(0.354f);
	t7=t7.move(0,0,7);
	t7=t7.move(0,sin(M_PI*(t/40.0f)) * wobble,0);
	drawThing(squareId, GL_TRIANGLE_STRIP, 4, yellow, t7, viewMat, projectionMat);

	glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

void draw::keyPressed(int key,int action)
{
	if(key==80&&action==1)
	{
		ortho=!ortho;
	}
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
