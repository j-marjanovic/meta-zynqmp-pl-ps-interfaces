
// Copyright (c) 2021 Jan Marjanovic


#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

class Udmabuf {

  void* _buf;
  size_t _size;
  size_t _phys_addr;

public:
  explicit Udmabuf(const std::string &udmabuf_name) {
    constexpr int flags = O_RDWR | O_SYNC;
    std::string udmabuf_filename{"/dev/" + udmabuf_name};
    int fd = open(udmabuf_filename.c_str(), flags);
    if (fd < 0) {
      throw std::runtime_error("could not open " + udmabuf_filename);
    }

    _size = _get_size(udmabuf_name);
    _phys_addr = _get_phys_addr(udmabuf_name);

    _buf = mmap(NULL, _size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (_buf == MAP_FAILED) {
      throw std::runtime_error("mmap failed");
    }

    std::cout << "Udmabuf\n";
    std::cout << "  name = " << udmabuf_name << "\n";
    std::cout << "  virt addr = " << _buf << "\n";
    std::cout << "  phys addr = 0x" << std::hex << _phys_addr << std::dec << "\n";
    std::cout << "  size = " << _size << "\n";
  }

  size_t get_phys_addr() {
    return _phys_addr;
  }

  uint32_t rd32(size_t offs) {
    return *static_cast<uint32_t*>(static_cast<uint32_t*>(_buf) + offs/4);
  }

  void wr32(size_t offs, uint32_t data) {
    *static_cast<uint32_t*>(static_cast<uint32_t*>(_buf) + offs/4) = data;
  }

  void* get_buf() {
    return _buf;
  }

private:
  size_t _get_size(const std::string &udmabuf_name) {
    return _get_sysfs_attr(udmabuf_name, "size");
  }

  size_t _get_phys_addr(const std::string &udmabuf_name) {
    return _get_sysfs_attr(udmabuf_name, "phys_addr");
  }

  size_t _get_sysfs_attr(const std::string &udmabuf_name, const std::string &attr_name) {
    std::string filename{"/sys/class/u-dma-buf/" + udmabuf_name + "/" + attr_name};
    std::ifstream infile{filename};
    if (!infile.good()) {
      throw std::runtime_error("could not open " + filename);
    }
    std::string s;
    infile >> s;
    return std::stoll(s, nullptr, 0);
  }
};
