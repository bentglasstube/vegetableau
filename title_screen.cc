#include "title_screen.h"

#include "game_screen.h"

TitleScreen::TitleScreen() :
  backdrop_("title.png"),
  text_("text.png"),
  pointer_("pointer.png", 0, 0, 16, 16),
  cursor_(0), music_(0) {}

bool TitleScreen::update(const Input& input, Audio& audio, unsigned int) {
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
    if (cursor_ == 0) return false;
    if (cursor_ == 1) audio.play_sample("bump.wav");
  }

  return true;
}

void TitleScreen::draw(Graphics& graphics) const {
  backdrop_.draw(graphics);

  const int y = graphics.height() / 2;

  text_.draw(graphics, "Play", graphics.width() / 2, y, Text::Alignment::Center);
  text_.draw(graphics, "Tutorial", graphics.width() / 2, y + 16, Text::Alignment::Center);
  text_.draw(graphics, "Music: ", graphics.width() / 2, y + 32, Text::Alignment::Center);
  text_.draw(graphics,  music_name(), graphics.width() / 2, y + 48, Text::Alignment::Center);

  pointer_.draw(graphics, 48, y + 16 * cursor_);
}

Screen* TitleScreen::next_screen() const {
  return new GameScreen(music_);
}
