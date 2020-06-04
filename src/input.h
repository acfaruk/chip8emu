#pragma once
#include <array>

namespace c8emu {
const int CHIP8_INPUT_COUNT = 16;

class Input {
 public:
  virtual ~Input() {}

  // This sets the input array correctly for chip8 to use
  virtual void SetInput(std::array<uint8_t, CHIP8_INPUT_COUNT>& input) = 0;
};
}  // namespace c8emu