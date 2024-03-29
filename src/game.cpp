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
  glm::vec3 mars(   1.f,  .4f,  .4f);
  glm::vec3 jupiter(1.f,  .8f,  .5f);
  glm::vec3 saturn( 1.f,  .9f,  .8f);
  glm::vec3 moon(    .5f, .5f,  .5f);
  glm::vec3 io(      .8f, .8f,  .3f);
  glm::vec3 europa(  .9f, .7f,  .7f);
  // planets                 r (earth)  m (earth)                       distance (10,000m)
  bodies[SUN] =     new Body(1.09e2f,   3.33e5f,   sun);
  bodies[MERCURY] = new Body(3.829e-1f, 5.5e-2f,   mercury, bodies[SUN], 5.79091e6f);
  bodies[VENUS] =   new Body(9.499e-1f, 8.15e-1f,  venus,   bodies[SUN], 1.08208e7f);
  bodies[EARTH] =   new Body(1.f,       1.f,       earth,   bodies[SUN], 1.49598261e7f);
  bodies[MARS] =    new Body(5.33e-1f,  1.07e-1f,  mars,    bodies[SUN], 2.279391e7f);
  bodies[JUPITER] = new Body(1.1209e1f, 3.178e2f,  jupiter, bodies[SUN], 7.785472e7f);
  bodies[SATURN] =  new Body(9.4492f,   9.5152e1f, saturn,  bodies[SUN], 1.43344937e8f);
  // moons                  r (earth) m (earth)                          distance (10,000m)
  bodies[MOON] =   new Body(2.73e-1f, 1.23e-2f, moon,   bodies[EARTH],   3.84399e4f);
  bodies[IO] =     new Body(2.86e-1f, 1.5e-2f,  io,     bodies[JUPITER], 4.217e4f);
  bodies[EUROPA] = new Body(2.45e-1f, 8.e-3f,   europa, bodies[JUPITER], 6.709e4f);

  // Instantiate ship.
  glm::vec2 position(bodies[EARTH]->position.x, -258.f);
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
  if (Keyboard::isKeyDown(KEY_E)) { ship->act(ORBIT, delta); }

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
