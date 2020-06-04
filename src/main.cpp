
#include <SDL.h>

#include <iostream>
#include <string>

#include "config.h"
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

  auto test2 = config.Get();
  SDL_Display display = SDL_Display(config.Get().display_config);

  std::array<uint8_t, DISPLAY_SIZE> test;

  for (int i = 0; i < DISPLAY_SIZE; i++) {
    test[i] = 0;
  }

  SDL_Speaker speaker = SDL_Speaker(config.Get().speaker_config);

  SDL_Input input(test2.input_config);

  std::array<uint8_t, INPUT_COUNT> input_data;

  while (true) {
    // Get the next event
    SDL_Event event;
    // Handle events on queue
    while (SDL_PollEvent(&event) != 0) {
      // User requests quit
      if (event.type == SDL_QUIT) {
        return 0;
      }
    }

    input.SetInput(input_data);

    for (int i = 0; i < INPUT_COUNT; i++) {
      if (input_data[i]) {
        speaker.Beep();
        test[i] = 1;
      } else {
        test[i] = 0;
      }
    }
    display.SetScreen(test);
    SDL_Delay(50);
  }

  return 0;
}