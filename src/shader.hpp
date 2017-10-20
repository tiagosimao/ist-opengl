#define VERTICES 0
#define COLORS 1

#include "GL/glew.h"
class Shader {
  public:
    GLuint VertexShaderId, FragmentShaderId, ProgramId;
    GLint uTransformMatrixId, uColor;
    Shader(char const * vertexShaderFile, char const * fragmentShaderFile);
    bool init();
    bool shutdown();
  private:
    char const * vertexShaderCode;
    char const * fragmentShaderCode;
};
