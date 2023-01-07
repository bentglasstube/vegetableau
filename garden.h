#pragma once

#include <array>

#include "graphics.h"
#include "spritemap.h"

class Garden {
  public:

    enum class Direction { Left, Right, Up, Down };
    enum class Tile { Empty, Carrot, Tomato, Eggplant, Potato, Brocolli, Onion, Pepper, Beet };

    Garden();

    Tile at(int x, int y) const;
    bool solved() const;
    void draw(Graphics& graphics, int x, int y) const;

    bool move(Direction dir);

  private:

    static constexpr int kSpriteSize = 16;

    SpriteMap sprites_;
    std::array<Tile, 64> tiles_;
    int width_, height_;

    void shuffle();
    int size() const { return width_ * height_; }

};
