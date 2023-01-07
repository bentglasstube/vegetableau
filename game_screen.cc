#include "game_screen.h"

#ifndef NDEBUG
#define SEED 8675309
#else
#include "util.h"
#define SEED Util::random_seed()
#endif

GameScreen::GameScreen() :
  backdrop_("title.png"),
  garden_(SEED, 1),
  dirt_("dirt.png", 0, 0, 135, 135),
  text_("text.png"),
  solved_(false)
{}

bool GameScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  if (input.key_pressed(Input::Button::Left))  garden_.move(Garden::Direction::Left);
  if (input.key_pressed(Input::Button::Right)) garden_.move(Garden::Direction::Right);
  if (input.key_pressed(Input::Button::Up))    garden_.move(Garden::Direction::Up);
  if (input.key_pressed(Input::Button::Down))  garden_.move(Garden::Direction::Down);

  solved_ = garden_.solved();

  if (solved_ && input.key_pressed(Input::Button::Start)) {
    garden_.generate(garden_.level() + 1);
  }

  timer_ += elapsed;

  return true;
}

void GameScreen::draw(Graphics& graphics) const {
  const int dx = 152 - 8 * garden_.width();
  const int dy = 144 - 8 * garden_.height();

  backdrop_.draw(graphics);
  dirt_.draw(graphics, 84, 76);
  if (solved_) graphics.draw_rect({87, 79}, {216, 208}, 0xd8ff00ff, true);
  garden_.draw(graphics, dx, dy);

  const int s = (timer_ / 1000) % 60;
  const int m = timer_ / 60000;
  const std::string time = std::to_string(m) + ":" + (s < 10 ? "0" : "") + std::to_string(s);

  text_.draw(graphics, "Level " + std::to_string(garden_.level()), 152, 60, Text::Alignment::Center);
  text_.draw(graphics, time, graphics.width() - 4, 210, Text::Alignment::Right);
}
