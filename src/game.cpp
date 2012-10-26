#include "game.hpp"

GLuint Game::ibo;
GLuint Game::vbo;
GLuint Game::program;



GLvoid Game::initialize() {
  const GLushort is[] = { 0, 1, 2, 3 };
  const GLfloat vs[] = {
    -1.f,  1.f, 0.f, 0.f, 1.f,
    -1.f, -1.f, 0.f, 0.f, 0.f,
     1.f,  1.f, 0.f, 1.f, 1.f,
     1.f, -1.f, 0.f, 1.f, 0.f
  };

  // Initialize index buffer object.
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLushort), is, GL_STATIC_DRAW);

  // Initialize vertex buffer object.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(GLfloat), vs, GL_STATIC_DRAW);

  // Initialize shaders.
  program = Display::shaders("render.vert", "render.frag");
}



GLvoid Game::update() {
}



GLvoid Game::render() {
  glClearColor(0.f, 0.f, 0.1f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glUseProgram(program);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);

  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (GLvoid *)0);

  glDisableVertexAttribArray(1);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
}
