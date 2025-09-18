#include "utils.h"

std::vector<std::string> divide_string(std::string str, char c) {
  // std::cout << "divide " << str << std::endl;
  std::vector<std::string> list;
  std::vector<char> cstr(str.begin(), str.end());
  std::string element = "";
  int l = 0;
  while (l < str.length()) {
    if (cstr[l] != c) {
      element = element + cstr[l];
    }
    if (cstr[l] == c) {
      list.push_back(element);
      element = "";
    }
    if (l == str.length() - 1) {
      list.push_back(element);
      break;
    }
    // std::cout << l << " " << cstr[l] << " " << element << std::endl;
    l = l + 1;
  }
  cstr.clear();
  return list;
}

std::string get_database_path(std::string sub_path) {
  std::string current_file_path = __FILE__;
  std::string db_file_path;
  std::size_t pos = current_file_path.find_last_of("/\\");
  if (pos != std::string::npos) {
    db_file_path = current_file_path.substr(0, pos + 1) + "Database/" + sub_path;
  } else {
    db_file_path = "";
  }
  return db_file_path;
}