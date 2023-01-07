#include "game_screen.h"

#ifndef NDEBUG
#define SEED 8675309
#else
#include "util.h"
#define SEED Util::random_seed()
#endif

GameScreen::GameScreen() : garden_(SEED), solved_(false) {}

bool GameScreen::update(const Input& input, Audio&, unsigned int) {
  if (input.key_pressed(Input::Button::Left))  garden_.move(Garden::Direction::Left);
  if (input.key_pressed(Input::Button::Right)) garden_.move(Garden::Direction::Right);
  if (input.key_pressed(Input::Button::Up))    garden_.move(Garden::Direction::Up);
  if (input.key_pressed(Input::Button::Down))  garden_.move(Garden::Direction::Down);

  solved_ = garden_.solved();

  if (solved_ && input.key_pressed(Input::Button::Start)) {
    garden_.generate(garden_.level() + 1);
  }

  return true;
}

void GameScreen::draw(Graphics& graphics) const {
  if (solved_) graphics.draw_rect({8, 8}, { 72, 72 }, 0xd8ff00ff, true);
  garden_.draw(graphics, 16, 16);
}


