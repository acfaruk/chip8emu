#pragma once
#include <SDL.h>

#include <array>
#include <cstdint>

namespace c8emu {

const int CHIP8_DISPLAY_WIDTH = 64;
const int CHIP8_DISPLAY_HEIGHT = 32;
const int CHIP8_DISPLAY_SIZE = CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_HEIGHT;

struct DisplayColor {
  DisplayColor(){};
  DisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
  uint8_t r = 0, g = 0, b = 0, a = 0;
};

struct DisplayConfig {
  uint8_t pixel_multiplier = 10;
  DisplayColor background_color = DisplayColor(0, 0, 0, 255);
  DisplayColor foreground_color = DisplayColor(255, 255, 255, 255);
};

class Display {
 public:
  Display(DisplayConfig& config);
  ~Display() {}

  // Initialize window with default 64 x 32 size and the given configuration
  void InitWindow();

  // Clears the screen with the background color
  void ClearScreen(bool force_redraw = true);

  // Set the display data, the given array is from left to right and from top to bottom, the values
  // are considered boolean, meaning 0 is white and everything else is black
  void SetScreen(const std::array<uint8_t, CHIP8_DISPLAY_SIZE>& pixel_data);

 private:
  DisplayConfig& config;

  // SDL
  SDL_Window* window;
  SDL_Renderer* renderer;

  // All the rectangles that could be drawn, usually only a subset of them will be drawn
  std::array<SDL_Rect, CHIP8_DISPLAY_SIZE> rects;
};
}  // namespace c8emu
