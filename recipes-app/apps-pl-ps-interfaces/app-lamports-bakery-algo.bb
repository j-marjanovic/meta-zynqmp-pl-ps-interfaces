# Copyright (C) 2021 Jan Marjanovic <jan.marjanovic@outlook.com>
# Released under the MIT license (see COPYING.MIT for the terms)

DESCRIPTION = "Lamport's Bakery Algo application"
SECTION = "examples"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${WORKDIR}/app-lamports-bakery-algo/src/app-lamports-bakery-algo.cpp;\
md5=a1b476fadc2adc2c4ecebfb041623f35;beginline=1;endline=3"

PV = "0.1"
PR = "r0" 

inherit pkgconfig cmake

SRC_URI = " \
    file://common/UioDevice.hpp \
    file://common/UioInterface.hpp \
    file://common/Udmabuf.hpp \
    file://common/UioUtilities.hpp \
    file://app-lamports-bakery-algo/src/app-lamports-bakery-algo.cpp \
    file://app-lamports-bakery-algo/src/AxiLamportsBakeryAlgo.hpp \
    file://app-lamports-bakery-algo/CMakeLists.txt \
"

S = "${WORKDIR}/app-lamports-bakery-algo"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 app-lamports-bakery-algo ${D}${bindir}
}

