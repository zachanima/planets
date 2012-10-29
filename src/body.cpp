#include "body.hpp"

GLuint Body::ibo;
GLuint Body::vbo;



GLvoid Body::initialize() {
  const GLfloat TAU = 6.28318531f;
  const GLfloat ANGLE = TAU / (GLfloat)SUBDIVISIONS;
  GLuint is[INDICES];
  glm::vec3 vs[VERTICES];

  // Initialize indices.
  for (size_t i = 0; i < VERTICES; i++) {
    is[i] = i;
  }

  // Initialize vertices.
  vs[0] = glm::vec3(0.f, 0.f, 0.f);
  for (size_t i = 1; i < VERTICES; i++) {
    vs[i].x = cos((GLfloat)(i-1) * ANGLE);
    vs[i].y = -sin((GLfloat)(i-1) * ANGLE);
  }

  // Initialize index buffer object.
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES * sizeof(GLuint), is, GL_STATIC_DRAW);

  // Initialize vertex buffer object.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, VERTICES * sizeof(glm::vec3), vs, GL_STATIC_DRAW);
}



GLvoid Body::update(GLuint delta) {
  radius = 200.f;
  mass = 1.f;
}



GLvoid Body::render(GLuint program, glm::mat4 &vp) {
  const GLuint mvpUniform = glGetUniformLocation(program, "mvp");
  glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(position.x, position.y, 0.f));
  model = glm::scale(model, glm::vec3(radius, radius, 1.f));
  glm::mat4 mvp = vp * model;

  glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid *)0);

  glDrawElements(GL_TRIANGLE_FAN, INDICES, GL_UNSIGNED_INT, (GLvoid *)0);

  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
