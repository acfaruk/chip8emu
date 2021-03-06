#pragma once

#include <yaml-cpp/yaml.h>

#include <array>
#include <string>

#include "display.h"
#include "emulator.h"
#include "input.h"
#include "speaker.h"

namespace c8emu {

static const std::string default_config_file = "config.yml";

struct Chip8EmuConfig {
  SpeakerConfig speaker_config;
  DisplayConfig display_config;
  InputConfig input_config;
  EmulationConfig emulation_config;
};

class Config {
 public:
  Config();
  Config(const std::string& file_path);

  const Chip8EmuConfig Get();

 private:
  YAML::Node root;

  void CreateDefaultConfig();
};

}  // namespace c8emu

namespace YAML {
// Conversions between internal structs and YAML
template <>
struct convert<c8emu::DisplayColor> {
  static Node encode(const c8emu::DisplayColor& rhs) {
    Node node;
    node.push_back((int)rhs.r);
    node.push_back((int)rhs.g);
    node.push_back((int)rhs.b);
    node.push_back((int)rhs.a);
    return node;
  }

  static bool decode(const Node& node, c8emu::DisplayColor& rhs) {
    if (!node.IsSequence() || node.size() != 4) {
      return false;
    }

    rhs.r = node[0].as<int>();
    rhs.g = node[1].as<int>();
    rhs.b = node[2].as<int>();
    rhs.a = node[3].as<int>();
    return true;
  }
};

template <>
struct convert<c8emu::DisplayConfig> {
  static Node encode(const c8emu::DisplayConfig& rhs) {
    Node node;
    node["pixel_multiplier"] = (int)rhs.pixel_multiplier;
    node["background_color"] = rhs.background_color;
    node["foreground_color"] = rhs.foreground_color;
    return node;
  }

  static bool decode(const Node& node, c8emu::DisplayConfig& rhs) {
    if (!node.IsMap() || node.size() != 3) {
      return false;
    }
    rhs.pixel_multiplier = node["pixel_multiplier"].as<int>();
    rhs.background_color = node["background_color"].as<c8emu::DisplayColor>();
    rhs.foreground_color = node["foreground_color"].as<c8emu::DisplayColor>();

    return true;
  }
};

template <>
struct convert<c8emu::SpeakerConfig> {
  static Node encode(const c8emu::SpeakerConfig& rhs) {
    Node node;
    node["beep_frequency"] = (int)rhs.beep_frequency;
    node["beep_duration"] = (int)rhs.beep_duration;
    return node;
  }

  static bool decode(const Node& node, c8emu::SpeakerConfig& rhs) {
    if (!node.IsMap() || node.size() != 2) {
      return false;
    }
    rhs.beep_frequency = node["beep_frequency"].as<int>();
    rhs.beep_duration = node["beep_duration"].as<int>();
    return true;
  }
};

template <>
struct convert<c8emu::InputConfig> {
  static Node encode(const c8emu::InputConfig& rhs) {
    Node node;
    node = rhs.input_map;
    return node;
  }

  static bool decode(const Node& node, c8emu::InputConfig& rhs) {
    if (!node.IsSequence() || node.size() != c8emu::CHIP8_INPUT_COUNT) {
      return false;
    }

    rhs.input_map = node.as<std::array<std::string, c8emu::CHIP8_INPUT_COUNT>>();
    return true;
  }
};

template <>
struct convert<c8emu::EmulationConfig> {
  static Node encode(const c8emu::EmulationConfig& rhs) {
    Node node;
    node["path_to_rom"] = rhs.path_to_rom;
    node["clock_ms"] = rhs.clock_ms;
    return node;
  }

  static bool decode(const Node& node, c8emu::EmulationConfig& rhs) {
    if (!node.IsMap()) {
      return false;
    }
    rhs.path_to_rom = node["path_to_rom"].as<std::string>();
    rhs.clock_ms = node["clock_ms"].as<int>();
    return true;
  }
};

template <>
struct convert<c8emu::Chip8EmuConfig> {
  static Node encode(const c8emu::Chip8EmuConfig& rhs) {
    Node node;
    node["speaker"] = rhs.speaker_config;
    node["display"] = rhs.display_config;
    node["input"] = rhs.input_config;
    node["emulation"] = rhs.emulation_config;
    return node;
  }

  static bool decode(const Node& node, c8emu::Chip8EmuConfig& rhs) {
    if (!node.IsMap()) {
      return false;
    }
    rhs.display_config = node["display"].as<c8emu::DisplayConfig>();
    rhs.speaker_config = node["speaker"].as<c8emu::SpeakerConfig>();
    rhs.input_config = node["input"].as<c8emu::InputConfig>();
    rhs.emulation_config = node["emulation"].as<c8emu::EmulationConfig>();
    return true;
  }
};
}  // namespace YAML