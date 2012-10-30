#include "orbit.hpp"
#include "body.hpp"

Orbit::Orbit(Body *host, GLfloat distance, GLfloat angle) {
  const GLfloat TAU = 6.2831853071795864f;
  const GLfloat G = .1f;
  const GLfloat M = host->mass; // TODO: Add smaller mass.
  GLfloat a = distance;
  GLfloat mu = G * M;

  this->host = host;
  this->distance = distance;
  this->angle = angle;
  period = TAU * glm::sqrt(glm::pow(a, 3.f) / mu);
}



GLvoid Orbit::update(GLuint delta) {
  const GLfloat TAU = 6.2831853071795864f;

  angle += (TAU / period) * (GLfloat)delta;
}



glm::vec2 Orbit::position() {
  return host->position + glm::vec2(cos(angle), sin(angle)) * distance;
}



glm::vec2 Orbit::velocity() {
  const GLfloat TAU = 6.2831853071795864f;
  glm::vec2 previous = host->position + glm::vec2(cos(angle), sin(angle)) * distance;
  glm::vec2 current = host->position + glm::vec2(cos(angle + TAU / period), sin(angle + TAU / period)) * distance;
  return current - previous;
}
