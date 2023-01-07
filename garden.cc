#include "garden.h"

#include <algorithm>
#include <deque>
#include <set>

Garden::Garden() :
  sprites_("veggies.png", 4, kSpriteSize, kSpriteSize),
  width_(3), height_(3)
{
  tiles_ = { Tile::Empty, Tile::Carrot, Tile::Tomato,
             Tile::Tomato, Tile::Carrot, Tile::Potato,
             Tile::Potato, Tile::Tomato, Tile::Potato };
}

Garden::Tile Garden::at(int x, int y) const {
  if (x < 0 || x >= width_) return Tile::Empty;
  if (y < 0 || y >= height_) return Tile::Empty;
  return tiles_[y * width_ + x];
}

bool Garden::solved() const {
  std::set<int> check;
  std::set<Tile> seen;

  for (int i = 0; i < size(); ++i) {
    check.insert(check.end(), i);
  }

  while (!check.empty()) {
    const auto i = *check.begin();
    const auto t = tiles_[i];

    if (seen.find(t) != seen.end()) return false;

    std::deque<int> queue = {i};
    check.erase(i);

    auto flood = [this, t, &queue, &check](int i) {
      if (check.find(i) != check.end() && tiles_[i] == t) {
        queue.push_back(i);
        check.erase(i);
      }
    };

    while (!queue.empty()) {
      const int q = queue.back();
      queue.pop_back();

      const int qx = q % width_;
      const int qy = q / width_;

      if (qx > 0)           flood(q - 1);
      if (qx < width_ - 1)  flood(q + 1);
      if (qy > 0)           flood(q - width_);
      if (qy < height_ - 1) flood(q + width_);
    }

    seen.insert(t);
  }

  return true;
}

void Garden::draw(Graphics& graphics, int x, int y) const {
  for (int iy = 0; iy < height_; ++iy) {
    for (int ix = 0; ix < width_; ++ix) {
      const auto tile = at(ix, iy);
      const int tx = x + kSpriteSize * ix;
      const int ty = y + kSpriteSize * iy;
      if (tile != Tile::Empty) {
        sprites_.draw(graphics, static_cast<int>(tile) - 1, tx, ty);
      }
    }
  }
}

bool Garden::move(Direction dir) {
  int empty = 0;
  for (int i = 0; i < width_ * height_; ++i) {
    if (tiles_[i] == Tile::Empty) {
      empty = i;
      break;
    }
  }

  switch (dir) {
    case Direction::Left:
      if (empty % width_ == width_ - 1) return false;
      std::swap(tiles_[empty + 1], tiles_[empty]);
      break;

    case Direction::Right:
      if (empty % width_ == 0) return false;
      std::swap(tiles_[empty - 1], tiles_[empty]);
      break;

    case Direction::Up:
      if (empty > size() - width_) return false;
      std::swap(tiles_[empty + width_], tiles_[empty]);
      break;

    case Direction::Down:
      if (empty < width_) return false;
      std::swap(tiles_[empty - width_], tiles_[empty]);
      break;
  }

  return true;
}
