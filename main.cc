// Copyright 2023 Alan Berndt

#include "./game.h"

#include "./title_screen.h"

#ifdef __EMSCRIPTEN__
#include "./emscripten.h"

void step(void* game) {
  static_cast<Game*>(game)->step();
}
#endif

int main(int, char**) {
  Game::Config config;

  config.graphics.title = "Vegetableau";
  config.graphics.width = 256;
  config.graphics.height = 224;
  config.graphics.scale = 2;
  config.graphics.fullscreen = false;

  Game game(config);
  Screen *start = new TitleScreen();

#ifdef __EMSCRIPTEN__
  game.start(start);
  emscripten_set_main_loop_arg(step, &game, 0, true);
#else
  game.loop(start);
#endif

  return 0;
}
