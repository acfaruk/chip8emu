#pragma once
#include <array>
#include <cstdint>

#include "config.h"

namespace c8emu {

const int DISPLAY_WIDTH = 64;
const int DISPLAY_HEIGHT = 32;
const int DISPLAY_SIZE = DISPLAY_WIDTH * DISPLAY_HEIGHT;

// Interface for a chip8 display
class Display {
 public:
  virtual ~Display() {}

  // Initialize window with default 64 x 32 size and the given configuration
  virtual void InitWindow(const DisplayConfig& config) = 0;

  // Clears the screen with the background color
  virtual void ClearScreen() = 0;

  // Set the display data, the given array is from left to right and from top to bottom, the values
  // are considered boolean, meaning 0 is white and everything else is black
  virtual void SetScreen(const std::array<uint8_t, DISPLAY_SIZE>& pixel_data) = 0;
};
}  // namespace c8emu
