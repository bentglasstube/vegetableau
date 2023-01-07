#pragma once

#include "screen.h"

#include "garden.h"

class GameScreen : public Screen {
  public:

    GameScreen();

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics& graphics) const override;

    std::string get_music_track() const override { return ""; }
    Screen* next_screen() const override { return nullptr; }

  private:

    Garden garden_;
    bool solved_;
};
