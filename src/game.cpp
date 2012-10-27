#include "game.hpp"

GLuint Game::ibo;
GLuint Game::vbo;
GLuint Game::program;
GLuint Game::ticks;
Ship *Game::ship = new Ship();



GLvoid Game::initialize() {
  // Initialize shaders.
  program = Display::shaders("render.vert", "render.frag");

  // Initialize ship class.
  Ship::initialize();

  // Initialize tick counter.
  ticks = SDL_GetTicks();
}



GLvoid Game::update() {
  const GLuint delta = SDL_GetTicks() - ticks;
  ticks = SDL_GetTicks();

  // Apply actions.
  if (Keyboard::isKeyDown(KEY_W)) { ship->act(THRUST); }
  if (Keyboard::isKeyDown(KEY_A)) { ship->act(CCW); }
  if (Keyboard::isKeyDown(KEY_D)) { ship->act(CW); }

  // Update ship.
  ship->update(delta);
}



GLvoid Game::render() {
  glm::mat4 projection = glm::ortho(0.f, 1280.f, 720.f, 0.f, -4.f, 4.f);
  glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(640.f, 360.f, 0.f));
  glm::mat4 vp = projection * view;

  glClearColor(0.f, 0.f, 0.1f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glUseProgram(program);

  ship->render(program, vp);

  glUseProgram(0);
}
