// Copyright 2023 Alan Berndt

#pragma once

#include <string>

class Music {
 public:
  enum Value {
    Random, GardenGaiden, FruitsOfLabor, LegendOfCarrot, GreenThumb, Silence
  };

  constexpr Music() : value_(Random) {}
  constexpr Music(Value v) : value_(v) {}
  constexpr Music(int n) : value_(static_cast<Value>(n)) {}
  operator Value() const { return value_; }
  explicit operator bool() = delete;

  std::string name() const { return value_name(value_); }
  std::string track() const { return value_track(value_); }

  static std::string value_name(Value v) {
    switch (v) {
      case Random:         return "Random";
      case GardenGaiden:   return "Garden Gaiden";
      case FruitsOfLabor:  return "Fruits of Labor";
      case LegendOfCarrot: return "Legend of Carrot";
      case GreenThumb:     return "Green Thumb";
      case Silence:        return "Silence";
      default:             return "Unknown";
    }
  }

  static std::string value_track(Value v) {
    switch (v) {
      case Random:         return "";
      case GardenGaiden:   return "gardengaiden.ogg";
      case FruitsOfLabor:  return "fruitsoflabor.ogg";
      case LegendOfCarrot: return "legendofcarrot.ogg";
      case GreenThumb:     return "greenthumb.ogg";
      case Silence:        return "";
      default:             return "";
    }
  }


 private:
  Value value_;
};

class Controls {
 public:
  enum Value { Normal, Reversed };

  constexpr Controls() : value_(Normal) {}
  constexpr Controls(Value v) : value_(v) {}
  constexpr Controls(int n) : value_(static_cast<Value>(n)) {}
  operator Value() const { return value_; }
  explicit operator bool() = delete;

  std::string name() const { return value_name(value_); }

  static std::string value_name(Value v) {
    switch (v) {
      case Normal:   return "Normal";
      case Reversed: return "Reversed";
      default:       return "Unknown";
    }
  }

 private:
  Value value_;
};

struct GameState {
  Music music;
  Controls controls;
};
