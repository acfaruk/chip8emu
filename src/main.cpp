
#include <SDL.h>

#include <fstream>
#include <iostream>
#include <string>

#include "config.h"
#include "sdl_chip8_emulator.h"
#include "sdl_display.h"
#include "sdl_input.h"
#include "sdl_speaker.h"

using namespace c8emu;

uint32_t EmulateCycle(uint32_t interval, void* emulator) {
  ((Chip8Emulator*)emulator)->EmulateCycle();
  return interval;
}

int main(int argc, char* argv[]) {
  Config config;
  if (argc == 1) {
    config = Config();
  } else if (argc == 2) {
    try {
      config = Config(argv[1]);
    } catch (std::exception) {
      std::cout << "The specified configuration file could not be loaded.";
      return 1;
    }
  } else {
    std::cout << "Only zero or one argument is allowed. Start without arguments to read/create the "
                 "default config or provide the path to your own config file."
              << std::endl;
    return 2;
  }

  Chip8EmuConfig configuration = config.Get();

  SDL_Display display(configuration.display_config);
  SDL_Speaker speaker(configuration.speaker_config);
  SDL_Input input(configuration.input_config);

  SDL_Chip8Emulator emulator(input, display, speaker, configuration.emulation_config);

  if (SDL_Init(SDL_INIT_TIMER) != 0) {
    throw std::runtime_error("SDL initialization failed: " + std::string(SDL_GetError()));
  }

  SDL_AddTimer((1 / 500.0) * 1000, EmulateCycle, &emulator);

  while (true) {
    // Get the next event
    SDL_Event event;
    // Handle events on queue
    while (SDL_PollEvent(&event) != 0) {
      // User requests quit
      if (event.type == SDL_QUIT) {
        SDL_Quit();
        return 0;
      }
    }

    SDL_Delay(50);
  }

  return 0;
}