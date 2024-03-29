#include "ship.hpp"

GLuint Ship::ibo;
GLuint Ship::vbo;



Ship::Ship(glm::vec2 &position) {
  const GLfloat TAU = 6.2831853071795864f;

  this->position = position;
  rotation = TAU / 4.f;
  angularVelocity = 0.f;
  this->nearest = NULL;
}



GLvoid Ship::initialize() {
  const GLushort is[] = { 0, 1, 2 };
  const GLfloat vs[] = {
    -8.f, -4.f,
    -8.f,  4.f,
     8.f,  0.f
  };

  // Initialize index buffer object.
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLushort), is, GL_STATIC_DRAW);

  // Initialize vertex buffer object.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(GLfloat), vs, GL_STATIC_DRAW);
}



GLvoid Ship::act(GLuint action, GLuint delta) {
  switch (action) {
    case THRUST:
      velocity += glm::vec2(cosf(rotation), -sinf(rotation)) * .0004f * (GLfloat)delta;
      // TODO: delete orbit;
      orbit = NULL;
      break;
    case CW:
      angularVelocity -= 0.000005f * (GLfloat)delta;
      break;
    case CCW:
      angularVelocity += 0.000005f * (GLfloat)delta;
      break;
    case ORBIT:
      orbit = new Orbit(nearest, glm::distance(position, nearest->position), glm::atan2(position.y - nearest->position.y, position.x - nearest->position.x));
      break;
  }
}



GLvoid Ship::update(GLuint delta) {
  if (orbit == NULL) {
    position += velocity * (GLfloat)delta;
  } else {
    orbit->update(delta);
    position = orbit->position();
    velocity = orbit->velocity();
  }
  rotation += angularVelocity * (GLfloat)delta;
}



GLvoid Ship::render(GLuint program, glm::mat4 &vp) {
  const GLuint mvpUniform = glGetUniformLocation(program, "mvp");
  const GLuint inColorUniform = glGetUniformLocation(program, "inColor");
  glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(position.x, position.y, 0.f));
  model = glm::rotate(model, glm::degrees(-rotation), glm::vec3(0.f, 0.f, 1.f));
  glm::mat4 mvp = vp * model;

  glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));
  glUniform3fv(inColorUniform, 1, glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)0);

  glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_SHORT, (GLvoid *)0);

  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}



GLvoid Ship::gravitate(Body *body, GLuint delta) {
  if (orbit == NULL) {
    const GLfloat G = .1f;
    const GLfloat SOFT = 1024.f / body->radius;
    const GLfloat DISTANCE = glm::max(SOFT, glm::distance(position, body->position + body->radius));
    glm::vec2 unit = glm::normalize(body->position - position);
    glm::vec2 acceleration = G * body->mass / glm::pow(DISTANCE, 2.f) * unit;
    velocity += acceleration * (GLfloat)delta;

    if (glm::distance(position + velocity, body->position) < body->radius) {
      velocity = body->velocity;
    }

    if (nearest == NULL || glm::distance(position, body->position) < glm::distance(position, nearest->position)) {
      nearest = body;
    }
  } else {
  }
}
