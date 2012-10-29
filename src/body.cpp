#include "body.hpp"

GLuint Body::ibo;
GLuint Body::vbo;



Body::Body(GLfloat radius, GLfloat mass, glm::vec3 &color) {
  this->host = NULL;
  this->radius = radius;
  this->mass = mass;
  this->color = color;
}



Body::Body(Body *host, GLfloat radius, GLfloat mass, GLfloat orbitalDistance, glm::vec3 &color) {
  const GLfloat TAU = 6.28318531f;
  const GLfloat G = 1.f;
  const GLfloat M = mass + host->mass;
  GLfloat a;
  GLfloat mu;

  this->host = host;
  this->radius = radius;
  this->mass = mass;
  this->orbitalDistance = orbitalDistance;
  this->color = color;

  a = glm::distance(glm::vec2(orbitalDistance, 0.f), host->position);
  mu = G * M;
  this->orbitalPeriod = TAU * glm::sqrt(glm::pow(a, 3.f) / mu) * 0.1f;
}



GLvoid Body::initialize() {
  const GLfloat TAU = 6.28318531f;
  const GLfloat ANGLE = TAU / (GLfloat)SUBDIVISIONS;
  GLuint is[INDICES];
  glm::vec2 vs[VERTICES];

  // Initialize indices.
  for (size_t i = 0; i < VERTICES; i++) {
    is[i] = i;
  }

  // Initialize vertices.
  vs[0] = glm::vec2(0.f, 0.f);
  for (size_t i = 1; i < VERTICES; i++) {
    vs[i].x = cos((GLfloat)(i-1) * ANGLE);
    vs[i].y = -sin((GLfloat)(i-1) * ANGLE);
  }

  // Initialize index buffer object.
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES * sizeof(GLuint), is, GL_STATIC_DRAW);

  // Initialize vertex buffer object.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, VERTICES * sizeof(glm::vec2), vs, GL_STATIC_DRAW);
}



GLvoid Body::update(GLuint delta) {
  const GLfloat TAU = 6.28318531f;

  // Orbit.
  if (host != NULL) {
    position = host->position + glm::vec2(cos(orbitalAngle), sin(orbitalAngle)) * orbitalDistance;
    orbitalAngle += TAU / orbitalPeriod;
  }
}



GLvoid Body::render(GLuint program, glm::mat4 &vp) {
  const GLuint mvpUniform = glGetUniformLocation(program, "mvp");
  const GLuint inColorUniform = glGetUniformLocation(program, "inColor");
  glm::mat4 mvp;
  glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(position.x, position.y, 0.f));
  model = glm::scale(model, glm::vec3(radius, radius, 1.f));
  model = glm::rotate(model, glm::degrees(glm::atan2(position.y, position.x)) + 270.f, glm::vec3(0.f, 0.f, 1.f));
  mvp = vp * model;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid *)0);

  // Draw light side.
  glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));
  glUniform3fv(inColorUniform, 1, glm::value_ptr(color));
  glDrawElements(GL_TRIANGLE_FAN, INDICES, GL_UNSIGNED_INT, (GLvoid *)0);

  // Draw dark side.
  model = glm::rotate(model, 180.f, glm::vec3(0.f, 0.f, 1.f));
  mvp = vp * model;
  glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));
  glUniform3fv(inColorUniform, 1, glm::value_ptr(color * 0.25f));
  glDrawElements(GL_TRIANGLE_FAN, INDICES, GL_UNSIGNED_INT, (GLvoid *)0);

  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
