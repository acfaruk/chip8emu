#include "config.h"

#include <fstream>
namespace c8emu {

Config::Config() {
  std::ifstream config_file(default_config_file);
  if (config_file.good()) {
    root = YAML::LoadFile(default_config_file);
  } else {
    CreateDefaultConfig();
    YAML::Emitter emitter;
    emitter << root;
    std::ofstream config_file(default_config_file);
    config_file << emitter.c_str();
  }
}

Config::Config(const std::string& file_name) { root = YAML::LoadFile(file_name); }

const Chip8EmuConfig Config::Get() { return root.as<Chip8EmuConfig>(); }

void Config::CreateDefaultConfig() {
  Chip8EmuConfig config;

  DisplayConfig displayConfig;
  displayConfig.pixel_multiplier = 10;
  displayConfig.background_color = DisplayColor(0, 0, 0, 255);
  displayConfig.foreground_color = DisplayColor(255, 255, 255, 255);
  config.display_config = displayConfig;

  root = config;
}

}  // namespace c8emu