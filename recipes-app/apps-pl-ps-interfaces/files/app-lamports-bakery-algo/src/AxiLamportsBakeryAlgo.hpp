
// Copyright (c) 2021 Jan Marjanovic

#pragma once

#include <iostream>

#include "UioInterface.hpp"

class AxiLamportsBakeryAlgo : public UioInterface {

  struct __attribute__((__packed__)) _regs {
    uint32_t id_reg;
    struct __attribute__((__packed__)) {
      uint32_t patch : 8;
      uint32_t minor : 8;
      uint32_t major : 8;
      uint32_t fill : 8;
    } version;
    uint32_t rsvd0x8;
    uint32_t scratch;
    struct __attribute__((__packed__)) {
      uint32_t done : 1;
      uint32_t rsvd : 31;
    } status;
    struct __attribute__((__packed__)) {
      uint32_t start : 1;
      uint32_t clear : 1;
      uint32_t rsvd : 30;
    } control;
    uint32_t rsvd0x18;
    uint32_t rsvd0x1c;
    uint64_t addr_counter;
    uint64_t addr_choosing;
    uint64_t addr_number;
    uint32_t rsvd0x38;
    uint32_t rsvd0x3c;
    uint32_t idx_inst;
    uint32_t idx_last;
    uint32_t rsvd0x48;
    uint32_t rsvd0x4c;
    uint32_t config_nr_cycles;
    uint32_t config_dly_prbs_init;
    uint32_t config_axi;
    uint32_t rsvd0x5c;
    uint32_t diag_last_cntr;
    uint32_t diag_last_data;
  };

  static_assert(sizeof(_regs) == 0x68, "sizeof _regs");

  volatile _regs *regs() { return static_cast<volatile _regs *>(_mem); }

public:
  using UioInterface::UioInterface;

  void print_info() {
    std::cout << "AxiLamportsBakeryAlgo info:\n";
    std::cout << "  id reg = 0x" << std::hex << regs()->id_reg << std::dec
              << "\n";
    std::cout << "  version = " << regs()->version.major << "."
              << regs()->version.minor << "." << regs()->version.patch << "\n";
  }

  void config_prbs(uint32_t seed) { regs()->config_dly_prbs_init = seed; }

  void config_addr(uint64_t base, uint32_t idx_inst, uint32_t nr_insts) {
    regs()->addr_counter = base | 0x100;
    regs()->addr_choosing = base | 0x1000;
    regs()->addr_number = base | 0x2000;

    regs()->idx_inst = idx_inst;
    regs()->idx_last = nr_insts - 1;

    regs()->config_nr_cycles = 100000 - 1;
  }

  void config_axi(uint8_t cache, uint8_t prot, uint8_t user) {
    regs()->config_axi = cache | (prot << 8) | (user << 16);
  }

  void start() { regs()->control.start = 1; }

  bool check_done() {
    uint32_t done = regs()->status.done;
    return done;
  }

  void clear_done() { regs()->control.clear = 1; }

  uint32_t get_last_cntr() { return regs()->diag_last_cntr; }

  uint32_t get_last_data() { return regs()->diag_last_data; }
};
