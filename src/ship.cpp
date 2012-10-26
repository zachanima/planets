#include "ship.hpp"

GLuint Ship::ibo;
GLuint Ship::vbo;



GLvoid Ship::initialize() {
  const GLushort is[] = { 0, 1, 2 };
  const GLfloat vs[] = {
    -4.f,  8.f, 0.f,
     4.f,  8.f, 0.f,
     0.f, -8.f, 0.f,
  };

  // Initialize index buffer object.
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLushort), is, GL_STATIC_DRAW);

  // Initialize vertex buffer object.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), vs, GL_STATIC_DRAW);
}



GLvoid Ship::render() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

  glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_SHORT, (GLvoid *)0);

  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
