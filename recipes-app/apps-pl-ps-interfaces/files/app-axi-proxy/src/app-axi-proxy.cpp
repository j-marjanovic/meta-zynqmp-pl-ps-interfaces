
// Copyright (c) 2021 Jan Marjanovic
// Released under the MIT license (see COPYING.MIT for the terms)

#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <thread>
#include <variant>

#include "Udmabuf.hpp"
#include "UioDevice.hpp"
#include "UioUtilities.hpp"

#include "AxiProxy.hpp"

int main() {

  Udmabuf udmabuf{"axi:udmabuf@0x0"};

  static std::vector<UioDevice> uios = UioUtilities::get_all_uios();
  std::sort(std::begin(uios), std::end(uios),
            [](const UioDevice &a, const UioDevice &b) -> bool {
              return a.addr < b.addr;
            });

  std::vector<AxiProxy> axi_proxies;

  for (auto &uio : uios) {
    std::cout << uio << "\n";
    if (uio.name == "AxiProxy") {
      axi_proxies.emplace_back(uio);
    }
  }

  for (auto &axi_proxy : axi_proxies) {
    axi_proxy.print_info();
  }

  // TODO: configure AXI

  uint64_t phys_addr = udmabuf.get_phys_addr();
  std::array<uint32_t, 4> arr{0xa, 0xb, 0xc, 0xd};

  axi_proxies[1].write(phys_addr, arr);

  std::array<uint32_t, 4> readback;
  axi_proxies[1].read(phys_addr, readback);

  for (auto val : readback) {
    std::cout << val << "\n";
  }
}
