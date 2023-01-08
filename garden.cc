#include "garden.h"

#include <algorithm>
#include <deque>
#include <iostream>
#include <set>

Garden::Garden(unsigned int seed, int level) :
  sprites_("veggies.png", 4, kSpriteSize, kSpriteSize),
  rng_(seed)
{
  generate(level);
}

void Garden::generate(int level) {
  const int seed = rng_();
  rng_.seed(seed);

  tiles_.fill(0);

  if (level < 1) level = 1;
  if (level > kMaxLevel) level = kMaxLevel;

  std::cerr << "Generating level " << level << " seed " << seed << std::endl;

  const auto spec = kLevelSpecs[level - 1];

  level_ = level;
  width_ = spec.width;
  height_ = spec.height;

  std::vector<int> fill(size());
  for (int i = 0; i < size(); ++i) {
    fill[i] = i;
  }
  std::shuffle(fill.begin(), fill.end(), rng_);

  tiles_[fill.back()] = Tile::Empty;
  fill.pop_back();

  std::vector<Tile> veggies(spec.veggies);
  for (int i = 0; i < spec.veggies; ++i) {
    veggies[i] = i + 1;
  }

  for (int i = 0; i < spec.stones; ++i) {
    tiles_[fill.back()] = Tile::Stone;
    fill.pop_back();
  }

  while (!veggies.empty()) {
    tiles_[fill.back()] = veggies.back();
    fill.pop_back();
    veggies.pop_back();
  }

  int misses = 0;

  while (!fill.empty()) {
    const int i = fill.back();
    const int ix = i % width_, iy = i / width_;
    fill.pop_back();

    std::vector<Tile> neighbors;
    if (at(ix - 1, iy).veggie()) neighbors.push_back(at(ix - 1, iy));
    if (at(ix + 1, iy).veggie()) neighbors.push_back(at(ix + 1, iy));
    if (at(ix, iy - 1).veggie()) neighbors.push_back(at(ix, iy - 1));
    if (at(ix, iy + 1).veggie()) neighbors.push_back(at(ix, iy + 1));

    if (neighbors.empty()) {
      fill.insert(fill.begin(), i);
      ++misses;

      if (misses > 1024) {
        generate(level);
        return;
      }
    } else {
      std::shuffle(neighbors.begin(), neighbors.end(), rng_);
      tiles_[i] = neighbors.front();
    }
  }

  while (solved()) {
    shuffle(4096);
  }

  std::uniform_real_distribution<float> ra(0, 2 * M_PI);
  for (int i = 0; i < size(); ++i) {
    const float a = ra(rng_);
    Point p = { static_cast<int>(200 * std::cos(a)), static_cast<int>(200 * std::sin(a)) };
    sliders_.emplace_back(p, i, tiles_[i], 750);
  }
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

    if (t.veggie()) {
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
  }

  // Don't consider solved until done animating.
  return !animating();
}

void Garden::update(unsigned int elapsed) {
  for (auto it = sliders_.begin(); it != sliders_.end(); ) {
    it->timer += elapsed;
    if (it->timer >= it->duration) {
      it = sliders_.erase(it);
    } else {
      ++it;
    }
  }
}

void Garden::draw(Graphics& graphics, int x, int y) const {
  std::set<int> skip;

  for (auto const& s : sliders_) {
    const float a = s.timer / (float)s.duration;
    const int sx = s.start.x + a * (pos(s.end).x - s.start.x);
    const int sy = s.start.y + a * (pos(s.end).y - s.start.y);

    sprites_.draw(graphics, s.veg - 1, x + sx, y + sy);
    skip.insert(s.end);
  }

  for (int iy = 0; iy < height_; ++iy) {
    for (int ix = 0; ix < width_; ++ix) {
      if (skip.find(iy * width_ + ix) == skip.end()) {
        const auto tile = at(ix, iy);
        const int tx = x + kSpriteSize * ix;
        const int ty = y + kSpriteSize * iy;
        if (tile != Tile::Empty) {
          sprites_.draw(graphics, tile - 1, tx, ty);
        }
      }
    }
  }


}

bool Garden::move(Direction dir, bool animate) {
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
      return swap(empty, empty + 1, animate);

    case Direction::Right:
      if (empty % width_ == 0) return false;
      return swap(empty, empty - 1, animate);

    case Direction::Up:
      if (empty >= size() - width_) return false;
      return swap(empty, empty + width_, animate);

    case Direction::Down:
      if (empty < width_) return false;
      return swap(empty, empty - width_, animate);
  }

  return false;
}

void Garden::shuffle(size_t count) {
  std::uniform_int_distribution<int> dist(0, 3);
  while (count > 0) {
    if (move(static_cast<Direction>(dist(rng_)), false)) --count;
  }
}

bool Garden::swap(int a, int b, bool animate) {
  if (tiles_[a].empty() && tiles_[b].moveable()) {
    if (animate) {
      std::cerr << "Making slider from " << b << " to " << a << std::endl;
      sliders_.emplace_back(pos(b), a, tiles_[b]);
    }
    std::swap(tiles_[a], tiles_[b]);
    return true;
  } else if (tiles_[a].moveable() && tiles_[b].empty()) {
    if (animate) {
      std::cerr << "Making slider from " << a << " to " << b << std::endl;
      sliders_.emplace_back(pos(a), b, tiles_[a]);
    }
    std::swap(tiles_[a], tiles_[b]);
    return true;
  } else {
    return false;
  }
}

const std::array<Garden::LevelSpec, Garden::kMaxLevel> Garden::kLevelSpecs = {{
  { 3, 3, 3, 0 },
  { 4, 4, 3, 0 },
  { 4, 4, 3, 1 },
  { 4, 4, 3, 2 },
  { 5, 5, 4, 2 },
  { 6, 5, 3, 4 },
  { 6, 6, 4, 2 },
  { 6, 6, 5, 2 },
  { 6, 6, 5, 3 },
  { 6, 6, 6, 3 },
  { 7, 5, 6, 4 },
  { 7, 5, 2, 1 },
  { 8, 4, 4, 2 },
  { 7, 7, 7, 5 },
  { 8, 8, 8, 8 },
}};
