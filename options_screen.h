// Copyright 2023 Alan Berndt

#pragma once

#include <string>
#include <vector>

#include "./backdrop.h"
#include "./screen.h"
#include "./text.h"

#include "./game_state.h"
#include "./title_screen.h"

class OptionsScreen : public Screen {
 public:
  explicit OptionsScreen(GameState state);

  bool update(const Input&, Audio&, unsigned int) override;
  void draw(Graphics&) const override;
  Screen* next_screen() const override { return new TitleScreen(state_); }
  std::string get_music_track() const override { return "greenthumb.ogg"; }

 private:
  GameState state_;
  Backdrop backdrop_;
  Text text_;
  int cursor_;
};
