// Copyright 2023 Alan Berndt

#include "./game_screen.h"

#ifndef NDEBUG
#define SEED 8675309
#else
#include "./util.h"
#define SEED Util::random_seed()
#endif

GameScreen::GameScreen(GameState state) :
  game_state_(state),
  backdrop_("title.png"),
  dirt_("dirt.png", 0, 0, 135, 135),
  text_("text.png"),
  rng_(SEED),
  music_rng_(rng_()),
  state_(State::Setup),
  garden_(rng_(), 1),
  timer_(0)
{}

bool GameScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  for (auto& m : messages_) {
    m.update(elapsed);
  }

  if (state_ == State::Setup) {
    garden_.update(elapsed);
    if (!garden_.animating()) state_ = State::Playing;
  } else if (state_ == State::Playing) {

    if (input.key_pressed(Input::Button::Left))  move(audio, Garden::Direction::Left);
    if (input.key_pressed(Input::Button::Right)) move(audio, Garden::Direction::Right);
    if (input.key_pressed(Input::Button::Up))    move(audio, Garden::Direction::Up);
    if (input.key_pressed(Input::Button::Down))  move(audio, Garden::Direction::Down);

    if (input.key_pressed(Input::Button::Start)) {
      audio.play_sample("pause.wav");
      state_ = State::Paused;
    }

    timer_ += elapsed;
    garden_.update(elapsed);

    if (garden_.solved()) {
      audio.play_sample("victory.wav");

      auto r = rng_();

      const std::string noun = kNouns[r % kNouns.size()];
      const std::string verb = kVerbs[r % kVerbs.size()];

      messages_.emplace_back(text_, noun, MovingText::Point{ -104, 128 }, MovingText::Point{ 152, 128 }, 250 );
      messages_.emplace_back(text_, verb + "!", MovingText::Point{ 408, 144 }, MovingText::Point{ 152, 144 }, 250 );

      state_ = State::Solved;
    }

  } else if (state_ == State::Paused) {
    if (input.key_pressed(Input::Button::Start)) {
      audio.play_sample("resume.wav");
      state_ = State::Playing;
    }
  } else if (state_ == State::Solved) {
    if (input.key_pressed(Input::Button::Start)) next_level();
    if (input.key_pressed(Input::Button::A)) next_level();
    if (input.key_pressed(Input::Button::B)) next_level();
  }

  if (game_state_.music == Music::Random && !audio.music_playing()) {
    std::uniform_int_distribution<int> r(1, 3);
    Music song = r(music_rng_);
    audio.play_music(song.track(), false);
  }

  return true;
}

void GameScreen::draw(Graphics& graphics) const {
  const int dx = 152 - 8 * garden_.width();
  const int dy = 144 - 8 * garden_.height();

  backdrop_.draw(graphics);
  dirt_.draw(graphics, 84, 76);

  if (state_ == State::Paused) {
    text_.draw(graphics, "Paused", 152, 136, Text::Alignment::Center);
  } else {
    garden_.draw(graphics, dx, dy);
  }

  const int s = (timer_ / 1000) % 60;
  const int m = timer_ / 60000;
  const std::string time = std::to_string(m) + ":" + (s < 10 ? "0" : "") + std::to_string(s);

  text_.draw(graphics, "Level " + std::to_string(garden_.level()), 152, 60, Text::Alignment::Center);
  text_.draw(graphics, time, graphics.width() - 4, 210, Text::Alignment::Right);

  for (const auto& m : messages_) {
    m.draw(graphics);
  }
}

void GameScreen::next_level() {
  garden_.generate(rng_(), garden_.level() + 1);
  state_ = State::Setup;
  messages_.clear();
}

void GameScreen::move(Audio& audio, Garden::Direction dir) {
  if (!garden_.move(dir)) audio.play_sample("bump.wav");
}

const std::array<std::string, 8> GameScreen::kNouns = {"Produce", "Crops", "Vegetables", "Plants", "Harvest", "Goods", "Plants", "Flora"};
const std::array<std::string, 7> GameScreen::kVerbs = {"Organized", "Sorted", "Arranged", "Ordered", "Grouped", "Marshalled", "Placed"};
