#include "ship.hpp"

GLuint Ship::ibo;
GLuint Ship::vbo;



GLvoid Ship::initialize() {
  const GLushort is[] = { 0, 1, 2 };
  const GLfloat vs[] = {
    -8.f, -4.f, 0.f,
    -8.f,  4.f, 0.f,
     8.f,  0.f, 0.f,
  };

  // Initialize index buffer object.
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLushort), is, GL_STATIC_DRAW);

  // Initialize vertex buffer object.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), vs, GL_STATIC_DRAW);

  // Initialize rotation.
}



GLvoid Ship::act(GLuint action) {
  switch (action) {
    case THRUST:
      velocity += glm::vec2(cosf(rotation), -sinf(rotation)) * .001f;
      break;
    case CW:
      rotation -= 0.005f;
      break;
    case CCW:
      rotation += 0.005f;
      break;
  }
}



GLvoid Ship::update() {
  position += velocity;
}



GLvoid Ship::render(GLuint program, glm::mat4 &vp) {
  const GLuint mvpUniform = glGetUniformLocation(program, "mvp");
  glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(position.x, position.y, 0.f));
  model = glm::rotate(model, glm::degrees(-rotation), glm::vec3(0.f, 0.f, 1.f));
  glm::mat4 mvp = vp * model;

  glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

  glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_SHORT, (GLvoid *)0);

  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
