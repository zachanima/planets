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
  const GLuint mvpUniform = glGetUniformLocation(program, "mvp");
  glm::mat4 model = glm::mat4(1.f);
  glm::mat4 view = glm::mat4(1.f);
  glm::mat4 projection = glm::ortho(0.f, 1280.f, 720.f, 0.f, -4.f, 4.f);
  glm::mat4 mvp = projection * view * model;

  glClearColor(0.f, 0.f, 0.1f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glUseProgram(program);
  glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));
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
