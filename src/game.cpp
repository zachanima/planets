#include "game.hpp"

GLuint Game::ibo;
GLuint Game::vbo;
GLuint Game::program;
GLuint Game::ticks;
Ship *Game::ship;
Body *Game::bodies[BODIES];



GLvoid Game::initialize() {
  // Initialize shaders.
  program = Display::shaders("render.vert", "render.frag");

  // Initialize classes.
  Body::initialize();
  Ship::initialize();
  
  // Instantiate ship.
  glm::vec2 position(0.f, -108.f);
  ship = new Ship(position);

  // Instantiate bodies.
  bodies[0] = new Body(100.f, 1.f);
  bodies[1] = new Body(bodies[0], 20.f, .1f, 500.f);
  bodies[2] = new Body(bodies[0], 40.f, .4f, 800.f);
  bodies[3] = new Body(bodies[0], 50.f, .5f, 1200.f);
  bodies[4] = new Body(bodies[3], 10.f, .025f, 200.f);

  // Initialize tick counter.
  ticks = SDL_GetTicks();

  // Enable blending.
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Enable line smoothing.
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}



GLvoid Game::update() {
  const GLuint delta = SDL_GetTicks() - ticks;
  ticks = SDL_GetTicks();

  // Update bodies.
  for (size_t i = 0; i < BODIES; i++) {
    bodies[i]->update(delta);
  }

  // Apply gravity to ship.
  for (size_t i = 0; i < BODIES; i++) {
    ship->gravitate(bodies[i], delta);
  }

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
  view = glm::translate(view, glm::vec3(-ship->position, 0.f));
  glm::mat4 vp = projection * view;

  glClearColor(0.f, 0.f, 0.1f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glUseProgram(program);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  for (size_t i = 0; i < BODIES; i++) {
    bodies[i]->render(program, vp);
  }
  ship->render(program, vp);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glUseProgram(0);
}
