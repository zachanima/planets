#ifndef BODY_HPP
#define BODY_HPP

#define GL_GLEXT_PROTOTYPES

#define SUBDIVISIONS (64)
#define VERTICES (SUBDIVISIONS + 2)
#define INDICES (VERTICES)

#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Body {
  public:
    static GLvoid initialize();
    GLvoid update(GLuint);
    GLvoid render(GLuint, glm::mat4 &);

  private:
    glm::vec2 position;
    GLfloat rotation;
    GLfloat angularVelocity;
    GLfloat radius;
    static GLuint ibo;
    static GLuint vbo;
};

#endif // BODY_HPP
