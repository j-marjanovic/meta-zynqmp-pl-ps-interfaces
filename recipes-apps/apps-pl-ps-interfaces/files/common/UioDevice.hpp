
// Copyright (c) 2021 Jan Marjanovic


#pragma once

#include <iomanip>
#include <string>

struct UioDevice {
  int number;
  std::string name;
  size_t addr;
  size_t size;
};

std::ostream &operator<<(std::ostream &os, const UioDevice &dev) {
  os << "UioDevice{number = " << std::setw(2) << dev.number
     << ", name = " << std::setw(20) << dev.name << ", addr = 0x" << std::hex
     << dev.addr << std::dec << ", size = " << dev.size << "}";
  return os;
}
