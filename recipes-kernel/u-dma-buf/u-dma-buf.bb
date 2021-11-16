DESCRIPTION = "u-dma-buf(User space mappable DMA Buffer)"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=bebf0492502927bef0741aa04d1f35f5"

inherit module

SRCREV = "40f5dfceaf55d5f8de071a302e5c932ec3f56b0f"
PV = "3.2.4+git${SRCPV}"
PR = "r0"

SRC_URI = " \
    git://github.com/ikwzm/udmabuf.git;protocol=https \
"

S = "${WORKDIR}/git"

