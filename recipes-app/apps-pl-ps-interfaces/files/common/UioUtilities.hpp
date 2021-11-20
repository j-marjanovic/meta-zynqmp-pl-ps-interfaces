
// Copyright (c) 2021 Jan Marjanovic


#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <fstream>

#include "UioDevice.hpp"

namespace fs = std::filesystem;

struct UioUtilities {

  static constexpr std::string_view UIO_SYSFS_DIR{"/sys/class/uio/"};

  static std::vector<UioDevice> get_all_uios() {
    std::vector<UioDevice> tmp;
    for (const fs::directory_entry &p : fs::directory_iterator(UIO_SYSFS_DIR)) {
      tmp.push_back(UioDevice{get_uio_number(p.path()), get_uio_name(p.path()),
                              get_uio_address(p.path()), get_uio_size(p.path())});
    }

    return tmp;
  }

  static int get_uio_number(const fs::path &path) {
    // we just skip "uio"
    return std::stoi(std::string{path.filename().c_str()[3]});
  }

  static std::string get_uio_name(const fs::path &path) {
    fs::path path_name = path / "name";
    std::ifstream ifs{path_name};
    if (!ifs.good()) {
      throw std::runtime_error(std::string{"Could not open "} +
                               path_name.c_str());
    }
    std::string name;
    std::getline(ifs, name);

    return name;
  }

  static size_t get_uio_size(const fs::path &path) {
    fs::path path_size = path / "maps/map0/size";
    std::ifstream ifs{path_size};
    if (!ifs.good()) {
      throw std::runtime_error(std::string{"Could not open "} +
                               path_size.c_str());
    }

    std::string size_str;
    std::getline(ifs, size_str);

    return std::stoll(size_str, 0, 0);
  }

  static size_t get_uio_address(const fs::path &path) {
    fs::path path_addr = path / "maps/map0/addr";
    std::ifstream ifs{path_addr};
    if (!ifs.good()) {
      throw std::runtime_error(std::string{"Could not open "} +
                               path_addr.c_str());
    }

    std::string addr_str;
    std::getline(ifs, addr_str);

    return std::stoll(addr_str, 0, 0);
  }
};
