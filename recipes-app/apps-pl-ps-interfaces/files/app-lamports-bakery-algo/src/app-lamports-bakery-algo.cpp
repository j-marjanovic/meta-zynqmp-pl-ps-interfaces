
// Copyright (c) 2021 Jan Marjanovic


#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <variant>
#include <thread>

#include "UioDevice.hpp"
#include "UioUtilities.hpp"
#include "Udmabuf.hpp"

#include "AxiLamportsBakeryAlgo.hpp"


int main() {

  Udmabuf udmabuf{"axi:udmabuf@0x0"};

  static std::vector<UioDevice> uios = UioUtilities::get_all_uios();
  std::sort(std::begin(uios),
            std::end(uios),
            [](const UioDevice &a, const UioDevice &b) -> bool { return a.addr < b.addr; });

  std::vector<AxiLamportsBakeryAlgo> axi_lamp_bas;

  for (auto &uio : uios) {
    std::cout << uio << "\n";
    if (uio.name == "LamportsBakeryAlgo") {
      axi_lamp_bas.emplace_back(uio);
    }
  }

  for (auto &axi_lamp_ba : axi_lamp_bas) {
    axi_lamp_ba.print_info();
  }

  uint32_t seed = 42;
  for (auto &axi_lamp_ba : axi_lamp_bas) {
    axi_lamp_ba.config_prbs(seed++);
  }

  uint32_t idx = 0;
  for (auto &axi_lamp_ba : axi_lamp_bas) {
    axi_lamp_ba.config_addr(udmabuf.get_phys_addr(), idx++, axi_lamp_bas.size());
    axi_lamp_ba.config_axi(0, 0, 0); // 0xF, 0x2, 0x1);
  }

  for (auto &axi_lamp_ba : axi_lamp_bas) {
    axi_lamp_ba.start();
  }

  const int TIMEOUT = 1000;
  for (int i = 0; i < TIMEOUT; i++) {
    bool all_done = true;
    for (auto &axi_lamp_ba : axi_lamp_bas) {
      all_done &= axi_lamp_ba.check_done();
    }
    if (all_done) {
      std::cout << "done\n";
      break;
    }
    std::cout << "." << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  for (auto &axi_lamp_ba : axi_lamp_bas) {
    axi_lamp_ba.clear_done();
  }

  for (auto &axi_lamp_ba : axi_lamp_bas) {
    std::cout << "Last counter = " << axi_lamp_ba.get_last_cntr() << "\n";
    std::cout << "Last data    = " << axi_lamp_ba.get_last_data() << "\n";
    std::cout << "\n";
  }

  void* buf = udmabuf.get_buf();
  uint32_t* buf_ptr = static_cast<uint32_t*>(buf);
  std::cout << "Data (from SW) = " << buf_ptr[0x100 / 4] << "\n";

}
