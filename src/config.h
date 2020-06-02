#pragma once

#include <yaml-cpp/yaml.h>

#include <string>

namespace c8emu {

static const std::string default_config_file = "config.yml";

// Configuration Structs
struct DisplayColor {
  DisplayColor(){};
  DisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
  uint8_t r = 0, g = 0, b = 0, a = 0;
};

struct DisplayConfig {
  uint8_t pixel_multiplier;
  DisplayColor background_color;
  DisplayColor foreground_color;
};

struct Chip8EmuConfig {
  DisplayConfig display_config;
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
struct convert<c8emu::Chip8EmuConfig> {
  static Node encode(const c8emu::Chip8EmuConfig& rhs) {
    Node node;
    node["display"] = rhs.display_config;
    return node;
  }

  static bool decode(const Node& node, c8emu::Chip8EmuConfig& rhs) {
    if (!node.IsMap() || node.size() != 1) {
      return false;
    }
    rhs.display_config = node["display"].as<c8emu::DisplayConfig>();

    return true;
  }
};

}  // namespace YAML