#pragma once
#include <SDL.h>

#include "display.h"

namespace c8emu {
class SDLDisplay : public Display {
 public:
  SDLDisplay(const DisplayConfig& config);
  // Inherited via Display
  virtual void InitWindow(const DisplayConfig& config) override;
  virtual void ClearScreen() override;
  virtual void SetScreen(const std::array<uint8_t, display_size>& pixel_data) override;

 private:
  DisplayConfig config;

  // SDL
  SDL_Window* window;
  SDL_Renderer* renderer;

  // All the rectangles that could be drawn, usually only a subset of them will be drawn
  std::array<SDL_Rect, display_size> rects;
};
}  // namespace c8emu