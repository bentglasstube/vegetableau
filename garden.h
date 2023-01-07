#pragma once

#include <array>
#include <random>

#include "graphics.h"
#include "spritemap.h"

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

      private:
        Value value_;

    };

    Garden(unsigned int seed);

    Tile at(int x, int y) const;
    bool solved() const;
    void draw(Graphics& graphics, int x, int y) const;

    bool move(Direction dir);

  private:

    static constexpr int kSpriteSize = 16;

    SpriteMap sprites_;
    std::array<Tile, 64> tiles_;
    int width_, height_;
    std::mt19937 rng_;

    int size() const { return width_ * height_; }
    void shuffle(size_t count = 256);
    bool swap(int a, int b);
};
