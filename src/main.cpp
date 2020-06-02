#include <SDL.h>

#include <iostream>
#include <string>

#include "config.h"
#include "display.h"
#include "sdl_display.h"

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

  SDLDisplay display = SDLDisplay(config.Get().display_config);

  std::array<uint8_t, display_size> test;

  for (int i = 0; i < display_size; i++) {
    test[i] = 0;
  }

  test[30] = 1;
  test[20 * 64 + 63] = 1;

  display.SetScreen(test);

  SDL_Delay(10000);

  return 0;
}