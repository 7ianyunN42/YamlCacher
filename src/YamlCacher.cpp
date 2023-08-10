#include "YamlCacher.h"
YAML::Node YamlCacher::get_yaml(std::string a_absolute_path) {
    auto it = this->_yaml_cache_map.find(a_absolute_path);
    if (it != this->_yaml_cache_map.end()) {
        YamlData& data = it->second;
    }
  return YAML::LoadFile(a_absolute_path); // for now, just load the file
}

YAML::Node YamlCacher::get_yaml(std::string a_absolute_path,
                                std::vector<std::string>& a_keys) {
  YAML::Node yaml = get_yaml(a_absolute_path);
  for (auto key : a_keys) {
    yaml = yaml[key];
  }
  return yaml;
}


YamlCacher *YamlCacher::get_singleton() {
  static YamlCacher singleton;
  return &singleton;
}

PyObject *YamlCacher::get_py_yaml_object(std::string a_absolute_path,
                                         std::vector<std::string> &a_keys) {
  YAML::Node yaml = get_yaml(a_absolute_path, a_keys);
  return yaml_node_to_py_object(yaml);
}

PyObject *YamlCacher::yaml_node_to_py_object(
    YAML::Node a_node) // this is expensive; caller when calling GET_YAML_PY()
                       // should be as specific with keys as possible
{
  if (a_node.IsScalar()) {
    return PyUnicode_FromString(a_node.as<std::string>().c_str());
  } else if (a_node.IsSequence()) {
    PyObject *list = PyList_New(0);
    for (auto it = a_node.begin(); it != a_node.end(); ++it) {
      PyObject *py_value = yaml_node_to_py_object(*it);
      PyList_Append(list, py_value);
    }
    return list;
  } else if (a_node.IsMap()) {
    PyObject *dict = PyDict_New();
    for (auto it = a_node.begin(); it != a_node.end(); ++it) {
      std::string key = it->first.as<std::string>();
      YAML::Node value = it->second;
      PyObject *py_value = yaml_node_to_py_object(value);
      PyDict_SetItemString(dict, key.c_str(), py_value);
    }
    return dict;
  } else {
    Py_RETURN_NONE;
  }
}
