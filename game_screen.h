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

    static const std::array<std::string, 8> kNouns;
    static const std::array<std::string, 7> kVerbs;

    Backdrop backdrop_;
    Sprite dirt_;
    Text text_;

    std::mt19937 rng_;
    State state_;
    Garden garden_;
    int timer_;
    int message_;

    void next_level();
    void move(Audio& audio, Garden::Direction dir);
};
