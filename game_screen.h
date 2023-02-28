// Copyright 2023 Alan Berndt

#pragma once

#include <random>
#include <string>
#include <utility>
#include <vector>

#include "./backdrop.h"
#include "./screen.h"
#include "./sprite.h"
#include "./text.h"

#include "./game_state.h"
#include "./garden.h"
#include "./moving_text.h"

class GameScreen : public Screen {
 public:
  explicit GameScreen(GameState state);

  bool update(const Input&, Audio&, unsigned int) override;
  void draw(Graphics& graphics) const override;

  std::string get_music_track() const override { return game_state_.music.track(); }
  Screen* next_screen() const override { return nullptr; }

 private:
  enum class State { Setup, Playing, Paused, Solved };

  static const std::array<std::string, 8> kNouns;
  static const std::array<std::string, 7> kVerbs;

  GameState game_state_;
  Backdrop backdrop_;
  Sprite dirt_;
  Text text_;

  std::mt19937 rng_;
  std::mt19937 music_rng_;
  State state_;
  Garden garden_;
  int timer_;

  std::vector<MovingText> messages_;

  void next_level();
  void move(Audio& audio, Garden::Direction dir);

  std::string track() const;
};
