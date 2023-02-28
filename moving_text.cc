// Copyright 2023 Alan Berndt

#include "./moving_text.h"

MovingText::MovingText(const Text& text, const std::string& message) :
  text_(text),
  message_(message),
  timer_(0), duration_(0)
{}

MovingText::MovingText(const Text& text, const std::string& message, Point from, Point to, int duration) :
  text_(text),
  message_(message),
  timer_(0), duration_(duration),
  from_(from), to_(to)
{}

void MovingText::update(unsigned int elapsed) {
  timer_ += elapsed;
  if (timer_ > duration_) timer_ = duration_;
}

void MovingText::draw(Graphics& graphics) const {
  const auto p = interpolate();
  text_.draw(graphics, message_, p.x, p.y, Text::Alignment::Center);
}

void MovingText::move(Point from, Point to, int duration) {
  from_ = from;
  to_ = to;
  duration_ = duration;
  timer_ = 0;
}

MovingText::Point MovingText::interpolate() const {
  float a = timer_ / static_cast<float>(duration_);
  const int x = from_.x + a * (to_.x - from_.x);
  const int y = from_.y + a * (to_.y - from_.y);
  return {x, y};
}
