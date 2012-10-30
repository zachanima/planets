#ifndef ORBIT_HPP
#define ORBIT_HPP

#include <GL/gl.h>
#include <glm/glm.hpp>

class Body;

class Orbit {
  public:
    Orbit(Body *, GLfloat);
    GLvoid update(GLuint);
    glm::vec2 position();

  private:
    Body *host;
    GLfloat distance;
    GLfloat period;
    GLfloat angle;
};

#endif // ORBIT_HPP
