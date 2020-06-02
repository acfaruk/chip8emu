#include <SDL.h>

#include <iostream>
#include <string>

#include "config.h"

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


  return 0;
}