

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI = " \
    file://regs.init \
"

BIF_PARTITION_ATTR_prepend := "init "

BIF_PARTITION_IMAGE[init] = "${WORKDIR}/regs.init"
BIF_PARTITION_ATTR[init] = "init"

