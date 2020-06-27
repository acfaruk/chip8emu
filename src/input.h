#pragma once
#include <array>
#include <string>

namespace c8emu {
const int CHIP8_INPUT_COUNT = 16;

struct InputConfig {
  std::array<std::string, CHIP8_INPUT_COUNT> input_map = {
      "Keypad 7", "Keypad 8", "Keypad 9", "d", "Keypad 4", "Keypad 5", "Keypad 6", "f",
      "Keypad 1", "Keypad 2", "Keypad 3", "g", "a",        "Keypad 0", "s",        "h"};
};

class Input {
 public:
  Input(InputConfig& config) : config(config){};
  // This sets the input array correctly for chip8 to use
  void SetInput(std::array<uint8_t, CHIP8_INPUT_COUNT>& input);

 private:
  InputConfig& config;
};
}  // namespace c8emu