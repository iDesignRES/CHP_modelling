#include "utils.h"
#include <iostream>

std::string get_database_path(std::string sub_path) {
  std::string current_file_path = __FILE__;
  std::string db_file_path;
  std::size_t pos = current_file_path.find_last_of("/\\");
  if (pos != std::string::npos) {
    db_file_path =
        current_file_path.substr(0, pos + 1) + "Database/" + sub_path;
    return db_file_path;
  } else
    throw std::runtime_error("Failed to determine current file path.");
}

toml::table get_toml_table(std::string file) {
  try {
    return toml::parse_file(file);
  } catch (const toml::parse_error& err) {
    throw std::runtime_error("Parsing failed when opening: " + file + "\n" +
                             err.what());
  }
}