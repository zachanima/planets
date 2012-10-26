#ifndef GAME_HPP
#define GAME_HPP

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#include "lwcppgl/display.hpp"

class Game {
  public:
    static GLvoid initialize();
    static GLvoid update();
    static GLvoid render();

  private:
    static GLuint ibo;
    static GLuint vbo;
    static GLuint program;
};

#endif // GAME_HPP
