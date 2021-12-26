# Copyright (C) 2021 Jan Marjanovic <jan.marjanovic@outlook.com>
# Released under the MIT license (see COPYING.MIT for the terms)

DESCRIPTION = "AXI Traffic Gen application"
SECTION = "examples"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${WORKDIR}/app-axi-traffic-gen/src/app-axi-traffic-gen.cpp;\
md5=a1b476fadc2adc2c4ecebfb041623f35;beginline=1;endline=3"

PV = "0.1"
PR = "r0" 

DEPENDS = "boost libconfig"
RDEPENDS_${PN} = "boost-program-options"

inherit pkgconfig cmake

SRC_URI = " \
    file://common/UioDevice.hpp \
    file://common/UioInterface.hpp \
    file://common/Udmabuf.hpp \
    file://common/UioUtilities.hpp \
    file://app-axi-traffic-gen/src/app-axi-traffic-gen.cpp \
    file://app-axi-traffic-gen/src/TrafficGenTest.hpp \
    file://app-axi-traffic-gen/src/AxiTrafficGen.hpp \
    file://app-axi-traffic-gen/src/AxiTrafficGen_regs.hpp \
    file://app-axi-traffic-gen/CMakeLists.txt \
"

S = "${WORKDIR}/app-axi-traffic-gen"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 app-axi-traffic-gen ${D}${bindir}
}

