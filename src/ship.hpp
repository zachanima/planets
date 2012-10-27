#ifndef SHIP_HPP
#define SHIP_HPP

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum {
  THRUST,
  CW,
  CCW
};

class Ship {
  public:
    static GLvoid initialize();
    GLvoid act(GLuint);
    GLvoid update(GLuint);
    GLvoid render(GLuint, glm::mat4 &);

  private:
    glm::vec2 position;
    glm::vec2 velocity;
    GLfloat rotation;
    GLfloat angularVelocity;
    static GLuint ibo;
    static GLuint vbo;
};

#endif // SHIP_HPP
