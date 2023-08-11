#include "YamlCacher.h"
#include <algorithm>
#include <memory>
#include <openssl/md5.h>
#include <shared_mutex>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
void get_md5_hash(std::string& a_filename, std::string& r_ret)
{
    unsigned char c[MD5_DIGEST_LENGTH];
    int i;
    FILE *inFile = fopen(a_filename.c_str(), "rb");
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];
    if (inFile == NULL) {
        return;
    }
    MD5_Init(&mdContext);
    while ((bytes = fread(data, 1, 1024, inFile)) != 0) {
        MD5_Update(&mdContext, data, bytes);
    }
    MD5_Final(c, &mdContext);
    fclose(inFile);
    char buf[33];
    for (i = 0; i < 16; i++) {
        sprintf(buf, "%02x", c[i]);
        r_ret += buf;
    }
}

enum class BooleanType
{
    Undefined,
    True,
    False
};

inline BooleanType getBooleanType(std::string& s) {
    if (s == "true" || s == "True") {
        return BooleanType::True;
    } else if (s == "false" || s == "False") {
        return BooleanType::False;
    } else {
        return BooleanType::Undefined;
    }
}

inline bool isNumber(std::string& s) { //
      if (s.empty()) return false;

      // Check for negative sign
      size_t pos = 0;
      if (s[0] == '-') {
          if (s.length() == 1) return false; // Only a negative sign is not a valid number
          pos = 1;
      }

      // Check for digits before the decimal point
      bool foundDigits = false;
      while (pos < s.length() && std::isdigit(s[pos])) {
          foundDigits = true;
          pos++;
      }

      // Check for decimal point and digits after it
      if (pos < s.length() && s[pos] == '.') {
          pos++;
          while (pos < s.length() && std::isdigit(s[pos])) {
              foundDigits = true;
              pos++;
          }
      }

      // If no digits found, it's not a valid number
      if (!foundDigits) return false;

      // Check for remaining characters
      while (pos < s.length() && std::isspace(s[pos])) {
          pos++;
      }

      // If we reached the end of the string, it's a valid number
      return pos == s.length();
}

const YamlCacher::YamlData YamlCacher::get_yaml_data(std::string a_absolute_path) {
    std::string current_file_md5;
    get_md5_hash(a_absolute_path, current_file_md5);
    { // read from yaml map and get the yaml if it exists
        std::shared_lock<std::shared_mutex> lock(this->_yaml_cache_map_lock); // accessing map
        auto it = this->_yaml_cache_map.find(a_absolute_path);
        if (it != this->_yaml_cache_map.end()) {
           YamlData data = it->second;
            // yaml is here, now check md5
            if (current_file_md5 ==  data.cached_file_md5) {
                return data; // return a copy of YamlData, so it's safe to be used outside of the lock
            } // fall through to re-insert
       }
    }
    // either the yaml doesn't exist in map, or the md5 is different, either way we re-insert the entry
    std::unique_lock<std::shared_mutex> lock(this->_yaml_cache_map_lock); // modifying map
    
    this->_yaml_cache_map.emplace(a_absolute_path, YamlData{current_file_md5, YAML::LoadFile(a_absolute_path)});

    return this->_yaml_cache_map[a_absolute_path];
}

YAML::Node YamlCacher::get_yaml_node(std::string a_absolute_path,
                                std::vector<std::string>& a_keys) {
  const YamlData yaml_data = get_yaml_data(a_absolute_path);
  YAML::Node node = Clone(yaml_data.yaml_node); // grab the og reference
  try {
    for (auto& key : a_keys) {
      node = node[key];
    }
  } catch (YAML::Exception& e) {
    throw std::runtime_error("YamlCacher::get_yaml_node: " + std::string(e.what()));
  } catch (...) {
    throw std::runtime_error("YamlCacher::get_yaml_node: Unknown error");
  }
  return node;
}

YamlCacher *YamlCacher::get_singleton() {
  static YamlCacher singleton;
  return &singleton;
}

PyObject *YamlCacher::get_py_yaml_object(std::string a_absolute_path,
                                         std::vector<std::string> &a_keys) {
  return yaml_node_to_py_object(get_yaml_node(a_absolute_path, a_keys));
}

PyObject *YamlCacher::yaml_node_to_py_object(
    YAML::Node a_node) // this is expensive; caller when calling GET_YAML_PY()
                       // should be as specific with keys as possible
{
  if (a_node.IsScalar()) {
    std::string value = a_node.as<std::string>();
    // check if it's boolean
    BooleanType boolean_type = getBooleanType(value);
    if (boolean_type == BooleanType::True) {
      Py_RETURN_TRUE;
    } else if (boolean_type == BooleanType::False) {
      Py_RETURN_FALSE;
    } else if (isNumber(value)) { // check if it's number
      return PyFloat_FromDouble(a_node.as<double>());
    } else { // or we simply return a string
      return PyUnicode_FromString(a_node.as<std::string>().c_str());
    }
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
