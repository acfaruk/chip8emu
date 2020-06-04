#pragma once

#include "config.h"
#include "input.h"

namespace c8emu {
class SDL_Input : public Input {
 public:
  SDL_Input(const InputConfig& config) : config(config){};
  // Inherited via Input
  virtual void SetInput(std::array<uint8_t, INPUT_COUNT>& input) override;

 private:
  InputConfig config;
};
}  // namespace c8emu