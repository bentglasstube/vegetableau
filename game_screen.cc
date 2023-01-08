#include "game_screen.h"

#ifndef NDEBUG
#define SEED 8675309
#else
#include "util.h"
#define SEED Util::random_seed()
#endif

GameScreen::GameScreen() :
  backdrop_("title.png"),
  dirt_("dirt.png", 0, 0, 135, 135),
  text_("text.png"),
  rng_(SEED),
  state_(State::Setup),
  garden_(rng_(), 1),
  timer_(0)
{}

bool GameScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  if (state_ == State::Setup) {
    garden_.update(elapsed);
    if (!garden_.animating()) state_ = State::Playing;
  } else if (state_ == State::Playing) {

    if (input.key_pressed(Input::Button::Left))  garden_.move(Garden::Direction::Left);
    if (input.key_pressed(Input::Button::Right)) garden_.move(Garden::Direction::Right);
    if (input.key_pressed(Input::Button::Up))    garden_.move(Garden::Direction::Up);
    if (input.key_pressed(Input::Button::Down))  garden_.move(Garden::Direction::Down);

    if (input.key_pressed(Input::Button::Start)) state_ = State::Paused;

    timer_ += elapsed;
    garden_.update(elapsed);

    if (garden_.solved()) {
      message_ = rng_();
      state_ = State::Solved;
    }

  } else if (state_ == State::Paused) {
    if (input.key_pressed(Input::Button::Start)) state_ = State::Playing;
  } else if (state_ == State::Solved) {
    if (input.key_pressed(Input::Button::Start)) next_level();
    if (input.key_pressed(Input::Button::A)) next_level();
    if (input.key_pressed(Input::Button::B)) next_level();
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

  if (state_ == State::Solved) {
    const std::string noun = kNouns[message_ % kNouns.size()];
    const std::string verb = kVerbs[message_ % kVerbs.size()];

    text_.draw(graphics, noun, 152, 128, Text::Alignment::Center);
    text_.draw(graphics, verb + "!", 152, 144, Text::Alignment::Center);
  }

  const int s = (timer_ / 1000) % 60;
  const int m = timer_ / 60000;
  const std::string time = std::to_string(m) + ":" + (s < 10 ? "0" : "") + std::to_string(s);

  text_.draw(graphics, "Level " + std::to_string(garden_.level()), 152, 60, Text::Alignment::Center);
  text_.draw(graphics, time, graphics.width() - 4, 210, Text::Alignment::Right);
}

void GameScreen::next_level() {
  garden_.generate(rng_(), garden_.level() + 1);
  state_ = State::Setup;
}

const std::array<std::string, 8> GameScreen::kNouns = {"Produce", "Crops", "Vegetables", "Plants", "Harvest", "Goods", "Plants", "Flora"};
const std::array<std::string, 7> GameScreen::kVerbs = {"Organized", "Sorted", "Arranged", "Ordered", "Grouped", "Marshalled", "Placed"};
