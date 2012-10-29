#ifndef GAME_HPP
#define GAME_HPP

#define GL_GLEXT_PROTOTYPES

#define BODIES (8)

#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "lwcppgl/display.hpp"
#include "lwcppgl/keyboard.hpp"

#include "body.hpp"
#include "ship.hpp"

class Game {
  public:
    static GLvoid initialize();
    static GLvoid update();
    static GLvoid render();

  private:
    static GLuint ibo;
    static GLuint vbo;
    static GLuint program;
    static GLuint ticks;
    static Ship *ship;
    static Body *bodies[BODIES];
};

#endif // GAME_HPP
