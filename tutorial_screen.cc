// Copyright 2023 Alan Berndt

#include "./tutorial_screen.h"

TutorialScreen::TutorialScreen(GameState state) :
  state_(state),
  backdrop_("houndstooth.png"),
  text_("text.png"),
  garden_(42, 1),
  timer_(0), index_(0),
  moves_({
      Garden::Direction::Right,
      Garden::Direction::Right,
      Garden::Direction::Up,
      Garden::Direction::Left,
      Garden::Direction::Left,
      Garden::Direction::Up,
  })
{
  garden_.clear_animations();
}

bool TutorialScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  timer_ += elapsed;

  if (timer_ > kMoveTime) {
    timer_ -= kMoveTime;
    garden_.move(moves_[index_]);
    ++index_;

    if (garden_.solved()) {
      garden_.generate(42, 1);
      garden_.clear_animations();
      index_ = 0;
    }
  }

  garden_.update(elapsed);

  if (input.key_pressed(Input::Button::Start)) return false;
  if (input.key_pressed(Input::Button::A)) return false;
  if (input.key_pressed(Input::Button::B)) return false;

  return true;
}

void TutorialScreen::draw(Graphics& graphics) const {
  backdrop_.draw(graphics);
  garden_.draw(graphics, 104, 56);

  text_.draw(graphics, "You cannot stand the chaos!", 128, 16, Text::Alignment::Center);

  text_.draw(graphics, "Slide your crops around to",   16, 128);
  text_.draw(graphics, "arrange them so that all the", 16, 144);
  text_.draw(graphics, "matching ones are in one",     16, 160);
  text_.draw(graphics, "contiguous block.",            16, 176);

  if (garden_.solved()) text_.draw(graphics, "Nice", 128, 70, Text::Alignment::Center);
}
