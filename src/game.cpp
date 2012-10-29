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
  glm::vec2 position(0.f, -208.f);
  ship = new Ship(position);

  // Instantiate bodies.
  glm::vec3 color(1.f, 1.f, 0.f);
  bodies[0] = new Body(100.f, 1.f, color);
  color = glm::vec3(1.f, 0.5f, 0.f);
  bodies[1] = new Body(bodies[0], 20.f, .1f, 500.f, color);
  color = glm::vec3(0.8f, 0.6f, 0.f);
  bodies[2] = new Body(bodies[0], 40.f, .4f, 800.f, color);
  color = glm::vec3(0.1f, 0.2f, 1.f);
  bodies[3] = new Body(bodies[0], 50.f, .5f, 1200.f, color);
  color = glm::vec3(0.8f, 0.8f, 0.8f);
  bodies[4] = new Body(bodies[3], 10.f, .025f, 200.f, color);

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
  if (Keyboard::isKeyDown(KEY_W)) { ship->act(THRUST, delta); }
  if (Keyboard::isKeyDown(KEY_A)) { ship->act(CCW, delta); }
  if (Keyboard::isKeyDown(KEY_D)) { ship->act(CW, delta); }

  // Update ship.
  ship->update(delta);
}



GLvoid Game::render() {
  const GLuint mvpUniform = glGetUniformLocation(program, "mvp");
  GLuint ibo;
  GLuint vbo;
  glm::mat4 projection = glm::ortho(0.f, 1280.f, 720.f, 0.f, -4.f, 4.f);
  glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(640.f, 360.f, 0.f));
  view = glm::translate(view, glm::vec3(-ship->position, 0.f));
  glm::mat4 model = glm::mat4(1.f);
  glm::mat4 vp = projection * view;
  glm::mat4 mvp = vp * model;

  glClearColor(0.f, 0.f, 0.1f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glUseProgram(program);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Render ship.
  ship->render(program, vp);

  // Render bodies and lines.
  for (size_t i = 0; i < BODIES; i++) {
    const GLuint is[] = { 0, 1 };
    const glm::vec3 vs[] = { glm::vec3(ship->position, 0.f), glm::vec3(ship->position, 0.f) + glm::normalize(glm::vec3(bodies[i]->position - ship->position, 0.f)) * 16.f };
    bodies[i]->render(program, vp);
    glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(GLuint), is, GL_STATIC_DRAW);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec3), vs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid *)0);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (GLvoid *)0);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &vbo);
  }
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glUseProgram(0);
}
