#include "sdl_display.h"

namespace c8emu {
SDL_Display::SDL_Display(const DisplayConfig& config) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error("SDL initialization failed: " + std::string(SDL_GetError()));
  }
  InitWindow(config);
}
void SDL_Display::InitWindow(const DisplayConfig& config) {
  this->config = config;
  window = SDL_CreateWindow("chip8emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            DISPLAY_WIDTH * config.pixel_multiplier,
                            DISPLAY_HEIGHT * config.pixel_multiplier, SDL_WINDOW_SHOWN);

  if (window == nullptr) {
    std::string error = SDL_GetError();
    SDL_Quit();
    throw std::runtime_error("SDL window creation failed: " + error);
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr) {
    std::string error = SDL_GetError();
    SDL_DestroyWindow(window);
    SDL_Quit();
    throw std::runtime_error("SDL renderer creation failed: " + error);
  }

  ClearScreen();
}

void SDL_Display::ClearScreen() {
  SDL_SetRenderDrawColor(renderer, config.background_color.r, config.background_color.g,
                         config.background_color.b, config.background_color.a);

  SDL_RenderClear(renderer);
}

void SDL_Display::SetScreen(const std::array<uint8_t, CHIP8_DISPLAY_SIZE>& pixel_data) {
  ClearScreen();

  int rect_counter = 0;
  for (int i = 0; i < pixel_data.size(); i++) {
    uint8_t pixel = pixel_data[i];

    if (!pixel) {
      continue;
    }

    SDL_Rect& rect = rects[rect_counter];

    rect.x = (i % 64) * config.pixel_multiplier;
    rect.y = (i / 64) * config.pixel_multiplier;
    rect.w = config.pixel_multiplier;
    rect.h = config.pixel_multiplier;

    rect_counter++;
  }

  SDL_SetRenderDrawColor(renderer, config.foreground_color.r, config.foreground_color.g,
                         config.foreground_color.b, config.foreground_color.a);
  SDL_RenderFillRects(renderer, rects.data(), rect_counter);
  SDL_RenderPresent(renderer);
}
}  // namespace c8emu