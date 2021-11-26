
// Copyright (c) 2021 Jan Marjanovic

#pragma once

#include <iostream>

#include "UioInterface.hpp"

class AxiProxy : public UioInterface {

#include "AxiProxy_regs.hpp"
  volatile AxiProxy_regs *regs() {
    return static_cast<volatile AxiProxy_regs *>(_mem);
  }

public:
  using UioInterface::UioInterface;

  void print_info() {
    std::cout << "AxiProxy info:\n";
    std::cout << "  id reg = 0x" << std::hex << regs()->ID_REG.ID << std::dec
              << "\n";
    std::cout << "  version = " << regs()->VERSION.MAJOR << "."
              << regs()->VERSION.MINOR << "." << regs()->VERSION.PATCH << "\n";
  }

  void config_axi(uint8_t cache, uint8_t prot, uint8_t user) {
    regs()->CONFIG_AXI.CACHE = cache;
    regs()->CONFIG_AXI.PROT = prot;
    regs()->CONFIG_AXI.USER = user;
  }

  uint32_t write(uint64_t addr, std::array<uint32_t, 4> &data) {
    assert(regs()->STATUS.READY_WR && regs()->STATUS.READY_RD);

    regs()->ADDR_LO = addr & 0xFFFFFFFFL;
    regs()->ADDR_HI = (addr >> 32) & 0xFFFFFFFFL;

    regs()->DATA_WR0.DATA = data[0];
    regs()->DATA_WR1.DATA = data[1];
    regs()->DATA_WR2.DATA = data[2];
    regs()->DATA_WR3.DATA = data[3];

    regs()->CONTROL.START_WR = 1;

    for (int i = 0; i < 1000; i++) {
      if (regs()->STATUS.DONE_WR) {
        break;
      }

      if (i == 1000 - 1) {
        throw std::runtime_error("timeout");
      }
    }

    regs()->CONTROL.DONE_CLEAR = 1;

    return regs()->STATS.CNTR_WR;
  }

  uint32_t read(uint64_t addr, std::array<uint32_t, 4> &data) {
    assert(regs()->STATUS.READY_WR && regs()->STATUS.READY_RD);

    regs()->ADDR_LO = addr & 0xFFFFFFFFL;
    regs()->ADDR_HI = (addr >> 32) & 0xFFFFFFFFL;

    regs()->CONTROL.START_RD = 1;

    for (int i = 0; i < 1000; i++) {
      if (regs()->STATUS.DONE_RD) {
        break;
      }

      if (i == 1000 - 1) {
        throw std::runtime_error("timeout");
      }
    }

    regs()->CONTROL.DONE_CLEAR = 1;

    data[0] = regs()->DATA_RD0.DATA;
    data[1] = regs()->DATA_RD1.DATA;
    data[2] = regs()->DATA_RD2.DATA;
    data[3] = regs()->DATA_RD3.DATA;

    return regs()->STATS.CNTR_WR;
  }
};
