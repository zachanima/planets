#include "main.hpp"

int main(int argc, char **argv) {
  Display::create(1280, 720);
  Game::initialize();

  while (!Keyboard::isKeyDown(KEY_ESCAPE)) {
    Game::update();
    Game::render();
    Display::update();
  }

  return 0;
}
