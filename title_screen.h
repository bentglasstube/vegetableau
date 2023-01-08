#pragma once

#include "backdrop.h"
#include "screen.h"
#include "sprite.h"
#include "text.h"

class TitleScreen : public Screen {
  public:

    TitleScreen();

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics&) const override;

    Screen* next_screen() const override;

    std::string get_music_track() const override { return "greenthumb.ogg"; }

  private:

    Backdrop backdrop_;
    Text text_;
    Sprite pointer_, title_;
    int cursor_, music_, timer_;

    std::string music_name() const {
      switch (music_) {
        case 0: return "Random";
        case 1: return "Garden Gaiden";
        case 2: return "Fruits of Labor";
        case 3: return "Legend of Carrot";
        case 4: return "Green Thumb";
        case 5: return "Silence";
        default: return "Unknown";
      }
    }
};
