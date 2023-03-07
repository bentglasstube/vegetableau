// Copyright 2023 Alan Berndt

#include "./title_screen.h"

#include "./game_screen.h"
#include "./game_state.h"
#include "./options_screen.h"
#include "./tutorial_screen.h"

TitleScreen::TitleScreen(GameState state) :
  state_(state),
  backdrop_("hills.png"),
  text_("text.png"),
  pointer_("pointer.png", 0, 0, 16, 16),
  title_("title.png", 0, 0, 224, 48),
  cursor_(0), timer_(0) {}

bool TitleScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (input.key_pressed(Input::Button::Up)) {
    if (cursor_ > 0) {
      --cursor_;
    } else {
      audio.play_sample("bump.wav");
    }
  }
  if (input.key_pressed(Input::Button::Down)) {
    if (cursor_ < 3) {
      ++cursor_;
    } else {
      audio.play_sample("bump.wav");
    }
  }

  if (input.key_pressed(Input::Button::Start)) return false;

  timer_ += elapsed;

  return true;
}

void TitleScreen::draw(Graphics& graphics) const {
  backdrop_.draw(graphics);
  title_.draw(graphics, 16, 8 + static_cast<int>(8 * std::sin(timer_ / 500.f)));

  const int y = graphics.height() / 2;

  text_.draw(graphics, "Play", 96, y);
  text_.draw(graphics, "Tutorial", 96, y + 16);
  text_.draw(graphics, "Options", 96, y + 32);
  text_.draw(graphics, "Quit", 96, y + 48);

  pointer_.draw(graphics, 72, y + 16 * cursor_);
}

Screen* TitleScreen::next_screen() const {
  switch (cursor_) {
    case 0: return new GameScreen(state_);
    case 1: return new TutorialScreen(state_);
    case 2: return new OptionsScreen(state_);
    default: return nullptr;
  }
}
