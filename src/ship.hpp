#ifndef SHIP_HPP
#define SHIP_HPP

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "body.hpp"
#include "orbit.hpp"

enum {
  THRUST,
  CW,
  CCW,
  ORBIT
};

class Ship {
  public:
    glm::vec2 position;
    Ship(glm::vec2 &);
    static GLvoid initialize();
    GLvoid act(GLuint, GLuint);
    GLvoid update(GLuint);
    GLvoid render(GLuint, glm::mat4 &);
    GLvoid gravitate(Body *body, GLuint);

  private:
    Body *nearest;
    Orbit *orbit;
    glm::vec2 velocity;
    GLfloat rotation;
    GLfloat angularVelocity;
    static GLuint ibo;
    static GLuint vbo;
};

#endif // SHIP_HPP
