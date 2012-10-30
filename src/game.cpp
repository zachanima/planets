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
  
  // Instantiate bodies.
  glm::vec3 sun(    4.f, 4.f,  0.f);
  glm::vec3 mercury(1.f,  .5f, 0.f);
  glm::vec3 venus(   .8f, .6f, 0.f);
  glm::vec3 earth(   .1f, .2f, 1.f);
  glm::vec3 moon(    .8f, .8f,  .8f);
  glm::vec3 mars(   1.f,  .4f,  .4f);
  glm::vec3 phobos(  .8f, .8f,  .8f);
  glm::vec3 deimos(  .8f, .8f,  .8f);
  glm::vec3 jupiter(1.f,  .8f,  .5f);
  glm::vec3 saturn( 1.f,  .9f,  .8f);
  bodies[SUN] =     new Body(109.f,  3330.f, sun);
  bodies[MERCURY] = new Body( .3829f,  .055f,  mercury, bodies[SUN],   8000.f);
  bodies[VENUS] =   new Body( 1.f,    1.f,     venus,   bodies[SUN],    800.f);
  bodies[EARTH] =   new Body( 1.f,    1.f,     earth,   bodies[SUN],  12000.f);
  bodies[MOON] =    new Body(  .273f,  .0123f, moon,    bodies[EARTH],  2.f * 384.399f);
  bodies[MARS] =    new Body( 1.f,     .8f,    mars,    bodies[SUN],   1800.f);
  bodies[PHOBOS] =  new Body( 1.f,     .004f,  phobos,  bodies[MARS],   100.f);
  bodies[DEIMOS] =  new Body( 1.f,     .004f,  deimos,  bodies[MARS],    80.f);
  bodies[JUPITER] = new Body( 1.f,    2.f ,    jupiter, bodies[SUN],   2500.f);
  bodies[SATURN] =  new Body( 1.f,    1.8f,    saturn,  bodies[SUN],   3200.f);
  bodies[URANUS] =  new Body( 1.f,    1.2f,    earth,   bodies[SUN],   3200.f);

  // Instantiate ship.
  glm::vec2 position(0.f, -bodies[SUN]->radius - 258.f);
  ship = new Ship(position);

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

  // Update bodies, apply gravity to ship.
  for (size_t i = 0; i < BODIES; i++) {
    if (bodies[i] != NULL) {
      bodies[i]->update(delta);
      ship->gravitate(bodies[i], delta);
    }
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

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glUseProgram(program);

  // Render ship.
  ship->render(program, vp);

  // Render bodies and lines.
  for (size_t i = 0; i < BODIES; i++) {
    if (bodies[i] != NULL) {
      const GLuint is[] = { 0, 1 };
      const glm::vec2 vs[] = {
        glm::vec2(ship->position),
        glm::vec2(ship->position) +
          glm::normalize(glm::vec2(bodies[i]->position - ship->position)) *
          glm::sqrt(glm::distance(ship->position, bodies[i]->position) - bodies[i]->radius)
      };
      bodies[i]->render(program, vp);
      glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));
      glGenBuffers(1, &ibo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(GLuint), is, GL_STATIC_DRAW);
      glGenBuffers(1, &vbo);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec2), vs, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid *)0);
      glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (GLvoid *)0);
      glDisableVertexAttribArray(0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glDeleteBuffers(1, &ibo);
      glDeleteBuffers(1, &vbo);
    }
  }

  glUseProgram(0);
}
