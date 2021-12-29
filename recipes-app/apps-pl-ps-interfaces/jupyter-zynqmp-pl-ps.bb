DESCRIPTION = "Jupyter notebook for Zynq MP PL/PS interfaces"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=bd67c54ff052244b70a29a4f4591a6ea"

PV = "1.1"
PR = "r0"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI = "\
    file://notebooks/00-traffic-gen.ipynb \
    file://notebooks/01-axi-proxy.ipynb \
    file://notebooks/02-axi-proxy-on-repeat.ipynb \
    file://notebooks/vivado_axi_proxy.png \
    file://notebooks/vivado_axi_tg.png \
    file://notebooks/LICENSE \
"

B = "${WORKDIR}/notebooks"
S = "${WORKDIR}/notebooks"

do_install() {
    install -d ${D}/usr/share/notebooks
    install -d ${D}/usr/share/notebooks/zynqmp-pl-ps

    install -m 0644 ${S}/00-traffic-gen.ipynb ${D}/usr/share/notebooks/zynqmp-pl-ps
    install -m 0644 ${S}/01-axi-proxy.ipynb ${D}/usr/share/notebooks/zynqmp-pl-ps
    install -m 0644 ${S}/02-axi-proxy-on-repeat.ipynb ${D}/usr/share/notebooks/zynqmp-pl-ps
    install -m 0444 ${S}/vivado_axi_proxy.png ${D}/usr/share/notebooks/zynqmp-pl-ps
    install -m 0444 ${S}/vivado_axi_tg.png ${D}/usr/share/notebooks/zynqmp-pl-ps
}

FILES_${PN} = "/usr/share/notebooks/zynqmp-pl-ps"

RDEPENDS_${PN} = "python3-fpga-mngr-interface python3-zynqmp-pl-ps"

