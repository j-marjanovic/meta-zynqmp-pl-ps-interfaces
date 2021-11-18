DESCRIPTION = "Server for Xilinx Virtual Cable"

SRCREV="067232f9fb78972b76cd3c70c7a4c7c46b841221"
PV = "0.1.0+git${SRCPV}"
PR = "r0"

LICENSE = "CC0-1.0"
LIC_FILES_CHKSUM = "file://README.md;beginline=1;endline=5;md5=a8037d24edd5b716c5c6f9256e732f6b"

SRC_URI = "\
    git://github.com/Xilinx/XilinxVirtualCable.git \
"

S = "${WORKDIR}/git/zynqMP/src/user"

EXTRA_OEMAKE_append = " MYCC='${CC} ${TARGET_LDFLAGS}' "

do_install () {
    install -d ${D}${bindir}
    install -m 0775 ${S}/xvcServer_ioctl ${D}${bindir}
}

