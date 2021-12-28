# Copyright (C) 2021 Jan Marjanovic <jan.marjanovic@outlook.com>
# Released under the MIT license (see COPYING.MIT for the terms)

DESCRIPTION = "AXI Proxy application"
SECTION = "examples"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${WORKDIR}/app-axi-proxy/src/app-axi-proxy.cpp;\
md5=a1b476fadc2adc2c4ecebfb041623f35;beginline=1;endline=3"

PV = "0.2"
PR = "r0"

DEPENDS = "boost libconfig"
RDEPENDS_${PN} = "boost-program-options"

inherit pkgconfig cmake

SRC_URI = " \
    file://common/UioDevice.hpp \
    file://common/UioInterface.hpp \
    file://common/Udmabuf.hpp \
    file://common/UioUtilities.hpp \
    file://app-axi-proxy/src/AxiProxy_regs.hpp \
    file://app-axi-proxy/src/app-axi-proxy.cpp \
    file://app-axi-proxy/src/AxiProxy.hpp \
    file://app-axi-proxy/CMakeLists.txt \
"

S = "${WORKDIR}/app-axi-proxy"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 app-axi-proxy ${D}${bindir}
}

