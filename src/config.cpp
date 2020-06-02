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

YAML::Node Config::Get() { return root; }

void Config::CreateDefaultConfig() {
  root["rom"] = "roms/default.rom";
  root["graphics"]["pixel_multiplier"] = 10;
}

}  // namespace c8emu