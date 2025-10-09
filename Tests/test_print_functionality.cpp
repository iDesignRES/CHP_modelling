#include <iostream>

#include "../src/bioCHP.h"

void print_parameter(parameter &p) {
  std::cout << p.data_id << " " << p.data_type;

  if (p.data_type == "str") {
    for (std::size_t ns = 0; ns < p.str.size(); ns++) {
      std::cout << " " << p.str[ns];
    }
    std::cout << std::endl;
  }

  if (p.data_type == "num") {
    for (std::size_t ns = 0; ns < p.vct.size(); ns++) {
      std::cout << " " << p.vct[ns];
    }
    std::cout << std::endl;
  }
}

void print_parameters(object &obj) {
  std::cout << " -------------------------------------- " << std::endl;
  std::cout << obj.sys_type << " " << obj.sys_def
            << " parameters: " << std::endl;
  std::cout << " -------------------------------------- " << std::endl;

  for (std::size_t np = 0; np < obj.p.size(); np++) {
    print_parameter(obj.p[np]);
  }
  std::cout << " -------------------------------------- " << std::endl;

  if (obj.c.size() > 0) {
    for (std::size_t nc = 0; nc < obj.c.size(); nc++) {
      print_parameters(obj.c[nc]);
    }
  }
}

int main() {
  object bioCHP = bioCHP_plant("../Tests/input_files/bioCHP_test_2.toml");
  print_parameter(bioCHP.p[1]);
  print_parameters(bioCHP);

  return 0;
}
