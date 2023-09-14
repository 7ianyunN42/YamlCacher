#pragma once
#include "Python.h"
#include "yaml-cpp/yaml.h"
#include <memory>
#include <mutex>
#include <object.h>
#include <shared_mutex>
#include <unordered_map>
#include <yaml-cpp/node/node.h>

class YamlCacher {
  struct YamlData;

public:
  static YamlCacher *get_singleton();

  PyObject *get_py_yaml_object(std::string a_absolute_path,
                               std::vector<std::string> &a_keys);

  const YamlData get_yaml_data(std::string a_absolute_path);

  YAML::Node get_yaml_node(std::string a_absolute_path,
                           std::vector<std::string> &a_keys);

private:
  struct YamlData {
    std::string cached_file_md5;
    YAML::Node yaml_node;
  };

  std::unordered_map<std::string, YamlData> _yaml_cache_map;
  std::shared_mutex _yaml_cache_map_lock;

  PyObject *yaml_scalar_node_to_py_object(YAML::Node a_node);

    PyObject *yaml_node_to_py_object(
        const YAML::Node
            a_node) // this is expensive; caller when calling GET_YAML_PY()
                    // should be as specific with keys as possible
        ;
};