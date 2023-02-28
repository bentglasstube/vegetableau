// Copyright 2023 Alan Berndt

#pragma once

#include <string>
#include <vector>

#include "./backdrop.h"
#include "./screen.h"
#include "./text.h"

#include "./game_state.h"
#include "./garden.h"
#include "./title_screen.h"

class TutorialScreen : public Screen {
 public:
  explicit TutorialScreen(GameState state);
  bool update(const Input&, Audio&, unsigned int) override;
  void draw(Graphics&) const override;
  Screen* next_screen() const override { return new TitleScreen(state_); }
  std::string get_music_track() const override { return "greenthumb.ogg"; }

 private:
  static constexpr int kMoveTime = 1000;

  GameState state_;
  Backdrop backdrop_;
  Text text_;
  Garden garden_;

  int timer_, index_;
  std::vector<Garden::Direction> moves_;
};
