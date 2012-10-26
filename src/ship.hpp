#ifndef SHIP_HPP
#define SHIP_HPP

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp>

class Ship {
  public:
    static GLvoid initialize();
    GLvoid update();
    GLvoid render();

  private:
    glm::vec2 position;
    glm::vec2 velocity;
    static GLuint ibo;
    static GLuint vbo;
};

#endif // SHIP_HPP
