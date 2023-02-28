// Copyright 2023 Alan Berndt

#pragma once

#include <string>
#include <utility>

#include "./text.h"

class MovingText {
 public:
  struct Point { int x, y; };

  MovingText(const Text& text, const std::string& message);
  MovingText(const Text& text, const std::string& message, Point from, Point to, int duration);

  void update(unsigned int elapsed);
  void draw(Graphics& graphics) const;
  void move(Point from, Point to, int duration);
  bool moving() const { return timer_ < duration_; }

 private:
  const Text& text_;
  const std::string message_;
  int timer_, duration_;
  Point from_, to_;

  Point interpolate() const;
};
