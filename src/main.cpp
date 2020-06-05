
#include <SDL.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include "config.h"
#include "sdl_chip8_emulator.h"
#include "sdl_display.h"
#include "sdl_input.h"
#include "sdl_speaker.h"

using namespace c8emu;

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

  SDL_Chip8Emulator emulator(configuration.emulation_config, input, display, speaker);

  std::chrono::high_resolution_clock clock;

  while (true) {
    auto time = clock.now();

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        SDL_Quit();
        return 0;
      }
    }

    emulator.EmulateMillisecond();

    while (true) {
      auto new_time = clock.now();
      long nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(new_time - time).count();
      if (nanos >= 1000000) break;  // break out off loop after a millisecond
    }
  }

  return 0;
}