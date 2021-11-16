DESCRIPTION = "Linux kernel driver for Xilinx Virtual Cable"

inherit module

SRCREV="067232f9fb78972b76cd3c70c7a4c7c46b841221"
PV = "0.1.0+git${SRCPV}"
PR = "r0"

LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263"

SRC_URI = "\
  git://github.com/Xilinx/XilinxVirtualCable.git \
  file://0001-Add-a-missing-include-for-iounmap.patch \
"

S = "${WORKDIR}/git/zynqMP/src/driver"
