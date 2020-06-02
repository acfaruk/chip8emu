#pragma once

#include <yaml-cpp/yaml.h>

#include <string>

namespace c8emu {

static const std::string default_config_file = "config.yml";

class Config {
 public:
  Config();
  Config(const std::string& file_path);

  YAML::Node Get();

 private:
  YAML::Node root;

  void CreateDefaultConfig();
};
}  // namespace c8emu