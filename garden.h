// Copyright 2023 Alan Berndt

#pragma once

#include <array>
#include <random>
#include <utility>
#include <vector>

#include "./graphics.h"
#include "./spritemap.h"

class Garden {
 public:
  enum class Direction { Left, Right, Up, Down };

  class Tile {
   public:
    enum Value {
      Empty,
      Carrot, Tomato, Eggplant, Potato, Brocolli, Onion, Pepper, Beet,
      Stone, Water
    };

    constexpr Tile() : value_(Empty) {}
    constexpr Tile(Value v) : value_(v) {}
    constexpr Tile(int n) : value_(static_cast<Value>(n)) {}
    operator Value() const { return value_; }
    explicit operator bool() = delete;

    bool empty() const { return value_ == Empty; }
    bool moveable() const {
      switch (value_) {
        case Stone:
        case Water:
          return false;
        default:
          return true;
      }
    }

    bool veggie() const {
      switch (value_) {
        case Carrot:
        case Tomato:
        case Eggplant:
        case Potato:
        case Brocolli:
        case Onion:
        case Pepper:
        case Beet:
          return true;
        default:
          return false;
      }
    }

   private:
    Value value_;
  };

  explicit Garden(unsigned int seed, int level = 1);
  void generate(unsigned int seed, int level);

  Tile at(int x, int y) const;
  bool solved() const;
  bool animating() const { return !sliders_.empty(); }
  void update(unsigned int elapsed);
  void draw(Graphics& graphics, int x, int y) const;
  void clear_animations() { sliders_.clear(); }

  int level() const { return level_; }
  int width() const { return width_; }
  int height() const { return height_; }

  static int max_level() { return kMaxLevel; }

  bool move(Direction dir, bool animate = true);

 private:
  struct LevelSpec {
    int width, height;
    int veggies;
    int stones;
  };

  struct Point {
    Point(int x, int y) : x(x), y(y) {}
    int x, y;
  };

  struct SlidingVeg {
    SlidingVeg(Point start, int end, Tile veg) : start(start), end(end), duration(kSlideTime), timer(0), veg(veg) {}
    SlidingVeg(Point start, int end, Tile veg, int duration) : start(start), end(end), duration(duration), timer(0), veg(veg) {}
    Point start;
    int end;
    int duration;
    int timer;
    Tile veg;
  };

  static constexpr int kSpriteSize = 16;
  static constexpr int kMaxLevel = 15;
  static constexpr int kSlideTime = 100;
  static const std::array<LevelSpec, kMaxLevel> kLevelSpecs;

  SpriteMap sprites_;
  std::mt19937 rng_;
  std::array<Tile, 64> tiles_;
  int width_, height_;
  int level_;

  std::vector<SlidingVeg> sliders_;

  int size() const { return width_ * height_; }
  void shuffle(size_t count = 256);
  bool swap(int a, int b, bool animate);
  Point pos(int i) const { return { (i % width_) * kSpriteSize, (i / width_) * kSpriteSize }; }
};
