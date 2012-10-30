#include "orbit.hpp"
#include "body.hpp"

Orbit::Orbit(Body *host, GLfloat distance) {
  const GLfloat TAU = 6.2831853071795864f;
  const GLfloat G = .125f;
  const GLfloat M = host->mass; // TODO: Add smaller mass.
  GLfloat a = distance;
  GLfloat mu = G * M;

  this->host = host;
  this->distance = distance;
  period = TAU * glm::sqrt(glm::pow(a, 3.f) / mu);
  angle = 0.f;
}



GLvoid Orbit::update(GLuint delta) {
  const GLfloat TAU = 6.2831853071795864f;

  angle += (TAU / period) * (GLfloat)delta;
}



glm::vec2 Orbit::position() {
  return host->position + glm::vec2(cos(angle), sin(angle)) * distance;
}
