#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#define SQRT_PARTICLES (128)
#define PARTICLES (SQRT_PARTICLES * SQRT_PARTICLES)

#define GL_GLEXT_PROTOTYPES

#include <cmath>
#include <cstdlib>

#include <GL/gl.h>
#include <GL/glext.h>

#include "lwcppgl/display.hpp"

class System {
  public:
    static GLvoid initialize();
    static GLvoid update();
    static GLvoid render();

  private:
    static GLuint ibo;
    static GLuint vbo;
    static GLuint quadfbo;
    static GLuint quadibo;
    static GLuint quadvbo;
    static GLuint positionTexture[3];
    static GLuint positionProgram;
    static GLuint renderProgram;
    static GLushort swap;
};

#endif // SYSTEM_HPP
