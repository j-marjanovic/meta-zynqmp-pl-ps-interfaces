
// Copyright (c) 2021 Jan Marjanovic

#pragma once

#include <cassert>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "UioDevice.hpp"

class UioInterface {
  int _fd;
  size_t _mem_length;

protected:
  volatile void *_mem;
  size_t _addr;
  int _number;

public:
  explicit UioInterface(const UioDevice &info) {
    std::string dev_filename{"/dev/uio"};
    dev_filename += std::to_string(info.number);
    _fd = open(dev_filename.c_str(), O_RDWR);

    _mem_length = info.size;
    _mem = mmap(NULL, _mem_length, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0);

    _addr = info.addr;
    _number = info.number;
  }

  UioInterface(const UioInterface &other) {
    std::string dev_filename{"/dev/uio"};
    dev_filename += std::to_string(other._number);
    _fd = open(dev_filename.c_str(), O_RDWR);

    _mem_length = other._mem_length;
    _mem = mmap(NULL, _mem_length, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0);

    _addr = other._addr;
    _number = other._number;
  }

  UioInterface(UioInterface &&other) noexcept {
    _fd = other._fd;
    _mem = other._mem;
    _mem_length = other._mem_length;
    _addr = other._addr;
    _number = other._number;

    other._fd = -1;
    other._mem = NULL;
  }

  ~UioInterface() {
    munmap(const_cast<void *>(_mem), _mem_length);
    close(_fd);
  }

  uint32_t _rd32(size_t offs) {
    assert((offs & 0x3) == 0);
    return *(static_cast<volatile uint32_t *>(_mem) + offs / 4);
  }

  void _wr32(size_t offs, uint32_t data) {
    assert((offs & 0x3) == 0);
    *(static_cast<volatile uint32_t *>(_mem) + offs / 4) = data;
  }

  uint64_t get_addr() { return _addr; }
};
