
// Copyright (c) 2021 Jan Marjanovic
// Released under the MIT license (see COPYING.MIT for the terms)

#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <thread>
#include <unordered_map>
#include <variant>

#include <boost/program_options.hpp>

#include "AxiTrafficGen.hpp"
#include "TrafficGenTest.hpp"
#include "Udmabuf.hpp"
#include "UioDevice.hpp"
#include "UioUtilities.hpp"

namespace po = boost::program_options;

int main(int argc, char *argv[]) {

  int count;
  std::string zynq_if;
  uint32_t axi_prot, axi_cache, axi_user;
  bool use_osync;

  // clang-format off
  po::options_description desc("Control AXI Traffic Generator\n\nArguments");
  desc.add_options()
    ("help",
        "display this help text and exit")
    ("interface",
        po::value<std::string>(&zynq_if)->default_value("hp"),
        "Zynq interface (\"hp\", \"hpc\", \"acp\")")
    ("count",
        po::value<int>(&count)->default_value(32),
        "number of bursts to transfer")
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
        "AXI AxUSER signal value");
  // clang-format on

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 0;
  }

  uint32_t udmabuf_flags = 0;
  if (use_osync) {
    udmabuf_flags |= O_SYNC;
  }

  static std::vector<UioDevice> uios = UioUtilities::get_all_uios();
  std::sort(std::begin(uios), std::end(uios),
            [](const UioDevice &a, const UioDevice &b) -> bool {
              return a.addr < b.addr;
            });

  std::unordered_map<std::string, AxiTrafficGen> axi_traffic_gens;

  for (auto &uio : uios) {
    std::cout << uio << "\n";
    if (uio.name == "AxiTrafficGen") {
      axi_traffic_gens.emplace(uio.note, uio);
    }
  }

  AxiTrafficGen &atg = axi_traffic_gens.at(zynq_if);
  atg.print_info();

  TrafficGenTestConfig config{.udmabuf_flags = udmabuf_flags,
                              .axi_cache = axi_cache,
                              .axi_prot = axi_prot,
                              .axi_user = axi_user};

  TrafficGenTest test{atg, config};

  std::cout << "Transfering " << count << " bursts\n";
  test.run(count);
}
