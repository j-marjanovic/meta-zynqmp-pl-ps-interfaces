
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_remove_u96v2-sbc = " \
    file://avnet_jtag.txt \
"

SRC_URI_append_u96v2-sbc = " \
    file://avnet_u96v2_dtb_boot.txt \
"
