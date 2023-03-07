// Copyright 2023 Alan Berndt

#include "./options_screen.h"

OptionsScreen::OptionsScreen(GameState state) :
  state_(state),
  backdrop_("houndstooth.png"),
  text_("text.png"),
  cursor_(0) {}

bool OptionsScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (input.key_pressed(Input::Button::Start)) return false;

  if (input.key_pressed(Input::Button::Up)) {
    if (cursor_ > 0) {
      --cursor_;
    } else {
      audio.play_sample("bump.wav");
    }
  }

  if (input.key_pressed(Input::Button::Down)) {
    if (cursor_ < 0) {
      ++cursor_;
    } else {
      audio.play_sample("bump.wav");
    }
  }

  return true;
}

void OptionsScreen::draw(Graphics& graphics) const {
  backdrop_.draw(graphics);
}
