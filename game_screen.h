#pragma once

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

    Backdrop backdrop_;
    Garden garden_;
    Sprite dirt_;
    Text text_;
    bool solved_;
    int timer_;
};
