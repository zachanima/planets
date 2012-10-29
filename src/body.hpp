#ifndef BODY_HPP
#define BODY_HPP

#define GL_GLEXT_PROTOTYPES

#define SUBDIVISIONS (256)
#define VERTICES (SUBDIVISIONS + 2)
#define INDICES (VERTICES)

#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Body {
  public:
    glm::vec2 position;
    Body(GLfloat, GLfloat);
    Body(Body *, GLfloat, GLfloat, GLfloat);
    GLfloat mass;
    GLfloat radius;
    static GLvoid initialize();
    GLvoid update(GLuint);
    GLvoid render(GLuint, glm::mat4 &);

  private:
    static GLuint ibo;
    static GLuint vbo;
    GLfloat orbitalDistance;
    GLfloat orbitalPeriod;
    GLfloat orbitalAngle;
    Body *host;
};

#endif // BODY_HPP
