#include "input.h"

#include <SDL.h>

#include <iostream>

namespace c8emu {
void Input::SetInput(std::array<uint8_t, CHIP8_INPUT_COUNT>& input) {
  // SDL_PumpEvents();
  const uint8_t* state = SDL_GetKeyboardState(nullptr);

  for (int i = 0; i < CHIP8_INPUT_COUNT; i++) {
    SDL_Scancode scan_code = SDL_GetScancodeFromName(config.input_map[i].c_str());

    input[i] = state[scan_code];
  }
}
}  // namespace c8emu