#include "game.hpp"

GLuint Game::ibo;
GLuint Game::vbo;
GLuint Game::program;
Ship *Game::ship = new Ship();



GLvoid Game::initialize() {
  // Initialize shaders.
  program = Display::shaders("render.vert", "render.frag");

  // Initialize ship class.
  Ship::initialize();
}



GLvoid Game::update() {
}



GLvoid Game::render() {
  const GLuint mvpUniform = glGetUniformLocation(program, "mvp");
  glm::mat4 projection = glm::ortho(0.f, 1280.f, 720.f, 0.f, -4.f, 4.f);
  glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(640.f, 360.f, 0.f));
  glm::mat4 model = glm::mat4(1.f);
  glm::mat4 mvp = projection * view * model;

  glClearColor(0.f, 0.f, 0.1f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glUseProgram(program);
  glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));

  ship->render();

  glUseProgram(0);
}
