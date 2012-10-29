#ifndef BODY_HPP
#define BODY_HPP

#define GL_GLEXT_PROTOTYPES

#define SUBDIVISIONS (256)
#define VERTICES (SUBDIVISIONS / 2 + 2)
#define INDICES (VERTICES)

#include <cmath>
#include <GL/gl.h>
#include <GL/glext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/compatibility.hpp>

class Body {
  public:
    glm::vec2 position;
    Body(GLfloat, GLfloat, glm::vec3 &);
    Body(Body *, GLfloat, GLfloat, GLfloat, glm::vec3 &);
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
    glm::vec3 color;
    Body *host;
};

#endif // BODY_HPP
