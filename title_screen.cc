#include "title_screen.h"

#include "game_screen.h"

TitleScreen::TitleScreen() : backdrop_("title.png"), text_("text.png") {}

bool TitleScreen::update(const Input& input, Audio&, unsigned int) {
  return !input.key_pressed(Input::Button::Start);
}

void TitleScreen::draw(Graphics& graphics) const {
  backdrop_.draw(graphics);
  text_.draw(graphics, "Press Start", graphics.width() / 2, graphics.height() - 32, Text::Alignment::Center);
}

Screen* TitleScreen::next_screen() const {
  return new GameScreen();
}
