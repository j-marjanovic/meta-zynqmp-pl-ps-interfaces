
// Copyright (c) 2021 Jan Marjanovic

#pragma once

#include <cstdint>
#include <cstring>

#include "AxiTrafficGen.hpp"
#include "Udmabuf.hpp"

typedef unsigned __int128 uint128_t;

std::ostream &operator<<(std::ostream &os, uint128_t data) {
  os << std::hex << "0x";

  for (int i = sizeof(uint128_t) / sizeof(uint32_t) - 1; i >= 0; i--) {
    uint32_t tmp = (data >> (i * sizeof(uint32_t) * 8)) &
                   ((1ULL << (sizeof(uint32_t) * 8)) - 1);
    os << tmp;
    if (i != 0) {
      os << "_";
    }
  }

  os << std::dec;
  return os;
}

struct TrafficGenTestConfig {
  uint32_t udmabuf_flags;
  uint32_t axi_cache;
  uint32_t axi_prot;
  uint32_t axi_user;
};

class TrafficGenTest {
  AxiTrafficGen &_atg;
  Udmabuf _udmabuf;
  TrafficGenTestConfig &_config;

  static constexpr int BURST_LEN_BEATS = 4; // each burst is 4 beats long
  static constexpr int BEAT_LEN_BYTES = 16; // each beat is 16 bytes
  static constexpr int BURST_LEN_BYTES = BURST_LEN_BEATS * BEAT_LEN_BYTES;

  static constexpr uint128_t ALL_ONES = -1;

public:
  explicit TrafficGenTest(AxiTrafficGen &atg, TrafficGenTestConfig &config)
      : _atg{atg}, _udmabuf{"axi:udmabuf@0x0", config.udmabuf_flags},
        _config{config} {

    if (_atg.config_get().burst_len != BURST_LEN_BEATS) {
      throw std::runtime_error(
          "Burst len config does not match between SW and HW");
    }
  }

  void run(uint32_t size_burst) {
    _clean_mem(size_burst);
    _check_mem(size_burst, ALL_ONES, [](uint128_t x) -> uint128_t {
      (void)x;
      return ALL_ONES;
    });

    _atg.config_axi(_config.axi_cache, _config.axi_prot, _config.axi_user);
    _atg.config_len(size_burst);
    _atg.config_addr(_udmabuf.get_phys_addr());
    _atg.start_write();
    _atg.wait_write_done();
    _atg.done_clear();

    _check_mem(size_burst, 0, [](uint128_t x) -> uint128_t { return x + 1; });

    _atg.start_read();
    _atg.wait_read_done();
    _atg.done_clear();

    AxiTrafficGenStats stats = _atg.get_stats();
    std::cout << "stats: rd cyc = " << stats.rd_cyc
              << ", wr cyc = " << stats.wr_cyc << ", rd_ok = " << stats.rd_ok
              << "\n";

    if (size_burst * BURST_LEN_BEATS != stats.rd_ok) {
      throw std::runtime_error("AXI TG readback: number of OK words mismatch");
    }
  }

  void _clean_mem(uint32_t size_burst) {
    memset(_udmabuf.get_buf(), 0xFF, size_burst * BURST_LEN_BYTES);
  }

  void _check_mem(uint32_t size_burst, uint128_t init,
                  std::function<uint128_t(uint128_t)> next) {
    uint128_t exp_val = init;
    uint128_t *mem_ptr = static_cast<uint128_t *>(_udmabuf.get_buf());

    for (uint32_t burst_addr = 0; burst_addr < size_burst; burst_addr++) {
      for (uint32_t beat_offs = 0; beat_offs < BURST_LEN_BEATS; beat_offs++) {
        uint32_t word_offs = burst_addr * BURST_LEN_BEATS + beat_offs;
        uint128_t val = mem_ptr[word_offs];
        if (exp_val != val) {
          std::stringstream msg;
          msg << "Memory value mismatch at offset " << word_offs
              << ": expected " << exp_val << ", received " << val;
          throw std::runtime_error(msg.str());
        }

        exp_val = next(exp_val);
      }
    }

    std::cout << "Memory check (size = " << size_burst
              << " bursts) successfully completed\n";
  }
};
