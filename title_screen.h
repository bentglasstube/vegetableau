// Copyright 2023 Alan Berndt

#pragma once

#include <string>

#include "./backdrop.h"
#include "./screen.h"
#include "./sprite.h"
#include "./text.h"

#include "./game_state.h"

class TitleScreen : public Screen {
 public:
  explicit TitleScreen(GameState state);

  bool update(const Input&, Audio&, unsigned int) override;
  void draw(Graphics&) const override;
  Screen* next_screen() const override;
  std::string get_music_track() const override { return "greenthumb.ogg"; }

 private:
  GameState state_;
  Backdrop backdrop_;
  Text text_;
  Sprite pointer_, title_;
  int cursor_, timer_;
};
