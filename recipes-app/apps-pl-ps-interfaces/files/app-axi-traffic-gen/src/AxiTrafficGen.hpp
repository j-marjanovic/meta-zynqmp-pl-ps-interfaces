
// Copyright (c) 2021 Jan Marjanovic

#pragma once

#include <iostream>

#include "UioInterface.hpp"

struct AxiTrafficGenStats {
  uint32_t wr_cyc;
  uint32_t rd_cyc;
  uint32_t rd_ok;
};

struct AxiTrafficConfig {
  uint32_t burst_len;
};

class AxiTrafficGen : public UioInterface {

#include "AxiTrafficGen_regs.hpp"
  volatile AxiTrafficGen_regs *regs() {
    return static_cast<volatile AxiTrafficGen_regs *>(_mem);
  }

public:
  using UioInterface::UioInterface;

  void print_info() {
    std::cout << "AxiTrafficGen info:\n";
    std::cout << "  id reg = 0x" << std::hex << regs()->ID_REG.ID << std::dec
              << "\n";
    std::cout << "  version = " << regs()->VERSION.MAJOR << "."
              << regs()->VERSION.MINOR << "." << regs()->VERSION.PATCH << "\n";
  }

  AxiTrafficConfig config_get(void) {
    return AxiTrafficConfig{regs()->CONFIG.BURST_LEN};
  }

  void config_axi(uint8_t cache, uint8_t prot, uint8_t user) {
    regs()->CONFIG_AXI.CACHE = cache;
    regs()->CONFIG_AXI.PROT = prot;
    regs()->CONFIG_AXI.USER = user;
  }

  void config_addr(uint64_t addr) {
    regs()->ADDR_LO = addr & 0xFFFFFFFFL;
    regs()->ADDR_HI = (addr >> 32) & 0xFFFFFFFFL;
  }

  void config_len(uint64_t len_bursts) { regs()->LENGTH = len_bursts; }

  void start_write(void) { regs()->CONTROL.START_WR = 1; }

  void start_read(void) { regs()->CONTROL.START_RD = 1; }

  void done_clear(void) { regs()->CONTROL.DONE_CLEAR = 1; }

  void wait_write_done(void) {
    const int TIMEOUT_VAL = 1000;
    for (int i = 0; i < TIMEOUT_VAL; i++) {
      if (regs()->STATUS.DONE_WR) {
        return;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    throw std::runtime_error("Timeout waiting for write to finish");
  }

  void wait_read_done(void) {
    const int TIMEOUT_VAL = 1000;
    for (int i = 0; i < TIMEOUT_VAL; i++) {
      if (regs()->STATUS.DONE_RD) {
        return;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    throw std::runtime_error("Timeout waiting for read to finish");
  }

  AxiTrafficGenStats get_stats(void) {
    return AxiTrafficGenStats{regs()->CNTR_WR_CYC, regs()->CNTR_RD_CYC,
                              regs()->CNTR_RD_OK};
  }
};
