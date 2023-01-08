#pragma once

#include <random>

#include "backdrop.h"
#include "screen.h"
#include "sprite.h"
#include "text.h"

#include "garden.h"

class GameScreen : public Screen {
  public:

    GameScreen();

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics& graphics) const override;

    std::string get_music_track() const override { return ""; }
    Screen* next_screen() const override { return nullptr; }

  private:

    enum class State { Setup, Playing, Paused, Solved };

    Backdrop backdrop_;
    Sprite dirt_;
    Text text_;

    std::mt19937 rng_;
    State state_;
    Garden garden_;
    int timer_;

    void next_level();
};
