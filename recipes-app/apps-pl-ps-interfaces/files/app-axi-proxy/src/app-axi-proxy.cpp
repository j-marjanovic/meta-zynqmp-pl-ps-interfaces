
// Copyright (c) 2021 Jan Marjanovic
// Released under the MIT license (see COPYING.MIT for the terms)

#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <thread>
#include <variant>

#include <boost/program_options.hpp>

#include "AxiProxy.hpp"
#include "Udmabuf.hpp"
#include "UioDevice.hpp"
#include "UioUtilities.hpp"

namespace po = boost::program_options;

namespace std {
std::ostream &operator<<(std::ostream &os, const std::vector<uint32_t> xs) {
  bool first = true;
  os << "std::vector<uint32_t>{";
  for (auto x : xs) {
    if (first) {
      first = false;
    } else {
      os << ", ";
    }
    os << x;
  }
  os << "}";
  return os;
}
} // namespace std

int main(int argc, char *argv[]) {

  std::string zynq_if;
  uint32_t axi_prot, axi_cache, axi_user;
  bool use_osync;
  std::vector<uint32_t> data;
  std::vector<uint32_t> DATA_DEFAULT_VAL{10, 20, 30, 40};
  constexpr int DATA_LEN = 4;

  po::options_description desc("Control AXI Proxy\n\nArguments");

  // clang-format off
  desc.add_options()
    ("help", "display this help text and exit")
    ("interface",
        po::value<std::string>(&zynq_if)->default_value("hp"),
        "Zynq interface (\"hp\", \"hpc\", \"acp\")")
    ("data",
        po::value<std::vector<uint32_t>>(&data)
	  ->multitoken()
	  ->default_value(DATA_DEFAULT_VAL),
        "data to write (and read) over the proxy")
    ("use-osync",
        po::bool_switch(&use_osync),
        "Use O_SYNC when opening the u-dma-buf buffer")
    ("axi-prot",
        po::value<uint32_t>(&axi_prot)->default_value(0),
        "AXI AxPROT signal value")
    ("axi-cache",
        po::value<uint32_t>(&axi_cache)->default_value(0),
        "AXI AxCACHE signal value")
    ("axi-user",
        po::value<uint32_t>(&axi_user)->default_value(0),
        "AXI AxUSER signal value")
    ;
  // clang-format on

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 0;
  }

  if (data.size() != DATA_LEN) {
    std::cout << "Data should be 4 but it is " << data.size()
              << " elements long\n";
    return EXIT_FAILURE;
  }

  uint32_t udmabuf_flags = 0;

  if (use_osync) {
    udmabuf_flags |= O_SYNC;
  }

  Udmabuf udmabuf{"axi:udmabuf@0x0", udmabuf_flags};

  static std::vector<UioDevice> uios = UioUtilities::get_all_uios();
  std::sort(std::begin(uios), std::end(uios),
            [](const UioDevice &a, const UioDevice &b) -> bool {
              return a.addr < b.addr;
            });

  std::unordered_map<std::string, AxiProxy> axi_proxies;

  for (auto &uio : uios) {
    std::cout << uio << "\n";
    if (uio.name == "AxiProxy") {
      axi_proxies.emplace(uio.note, uio);
    }
  }

  AxiProxy &axi_proxy = axi_proxies.at(zynq_if);
  axi_proxy.print_info();
  axi_proxy.config_axi(axi_cache, axi_prot, axi_user);

  uint64_t phys_addr = udmabuf.get_phys_addr();
  std::array<uint32_t, DATA_LEN> arr;
  std::copy_n(data.begin(), DATA_LEN, arr.begin());
  axi_proxy.write(phys_addr, arr);

  std::cout << "SW read, HW written:\n";
  for (int i = 0; i < DATA_LEN; i++) {
    uint32_t sw_read = udmabuf.rd32(i * 4);
    std::cout << "  " << sw_read << ", " << data[i] << "\n";
    if (sw_read != data[i]) {
      throw std::runtime_error("Readback value does not match");
    }
  }

  std::array<uint32_t, DATA_LEN> readback;
  axi_proxy.read(phys_addr, readback);

  std::cout << "readback, expected:\n";
  for (int i = 0; i < DATA_LEN; i++) {
    std::cout << "  " << readback[i] << ", " << data[i] << "\n";
    if (readback[i] != data[i]) {
      throw std::runtime_error("Readback value does not match");
    }
  }
}
