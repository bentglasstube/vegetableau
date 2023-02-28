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

#include "./garden.h"
#include "./moving_text.h"

class GameScreen : public Screen {
 public:
  explicit GameScreen(int music_ = 0);

  bool update(const Input&, Audio&, unsigned int) override;
  void draw(Graphics& graphics) const override;

  std::string get_music_track() const override { return track(music_); }
  Screen* next_screen() const override { return nullptr; }

 private:
  enum class State { Setup, Playing, Paused, Solved };

  static const std::array<std::string, 8> kNouns;
  static const std::array<std::string, 7> kVerbs;

  Backdrop backdrop_;
  Sprite dirt_;
  Text text_;

  std::mt19937 rng_;
  State state_;
  Garden garden_;
  int timer_;
  int music_;

  std::vector<MovingText> messages_;

  void next_level();
  void move(Audio& audio, Garden::Direction dir);

  std::string track(int i) const;
};
