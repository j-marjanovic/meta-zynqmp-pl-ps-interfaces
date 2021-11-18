
// Copyright (c) 2021 Jan Marjanovic


#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <variant>

#include "UioDevice.hpp"
#include "UioUtilities.hpp"


int main() {
  static std::vector<UioDevice> uios = UioUtilities::get_all_uios();

  std::sort(std::begin(uios),
            std::end(uios),
            [](const UioDevice &a, const UioDevice &b) -> bool { return a.addr < b.addr; });

  std::cout << "uios length = " << uios.size() << "\n";

  for (auto &uio : uios) {
    std::cout << uio << "\n";
  }
}
