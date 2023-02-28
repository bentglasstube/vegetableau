// Copyright 2023 Alan Berndt

#include "./title_screen.h"

#include "./game_screen.h"
#include "./tutorial_screen.h"

TitleScreen::TitleScreen() :
  backdrop_("title.png"),
  text_("text.png"),
  pointer_("pointer.png", 0, 0, 16, 16),
  title_("name.png", 0, 0, 224, 48),
  cursor_(0), music_(0), timer_(0) {}

bool TitleScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (input.key_pressed(Input::Button::Up)) {
    if (cursor_ > 0) --cursor_;
  }
  if (input.key_pressed(Input::Button::Down)) {
    if (cursor_ < 2) ++cursor_;
  }

  if (cursor_ == 2) {
    if (input.key_pressed(Input::Button::Left)) {
      if (--music_ < 0) music_ = 5;
    }
    if (input.key_pressed(Input::Button::Right)) {
      if (++music_ > 5) music_ = 0;
    }
  } else if (input.key_pressed(Input::Button::Start)) {
    if (cursor_ == 2) {
      audio.play_sample("bump.wav");
    } else {
      return false;
    }
  }

  timer_ += elapsed;

  return true;
}

void TitleScreen::draw(Graphics& graphics) const {
  backdrop_.draw(graphics);
  title_.draw(graphics, 16, 8 + static_cast<int>(8 * std::sin(timer_ / 500.f)));

  const int y = graphics.height() / 2;

  text_.draw(graphics, "Play", 96, y);
  text_.draw(graphics, "Tutorial", 96, y + 16);
  text_.draw(graphics, "$ " + music_name(), 96, y + 32);

  pointer_.draw(graphics, 64, y + 16 * cursor_);
}

Screen* TitleScreen::next_screen() const {
  if (cursor_ == 0) {
    return new GameScreen(music_);
  } else {
    return new TutorialScreen();
  }
}
