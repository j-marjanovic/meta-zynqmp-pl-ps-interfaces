DESCRIPTION = "Jupyter notebook demonstrating Numpy acceleration"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=bd67c54ff052244b70a29a4f4591a6ea"

PV = "1.0"
PR = "r0"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI = "\
    file://axi_proxy_in_vivado.png \
    file://numpy_example.ipynb \
    file://LICENSE \
"

B = "${WORKDIR}"
S = "${WORKDIR}"

do_install() {
    install -d ${D}/usr/share/notebooks
    install -d ${D}/usr/share/notebooks/numpy-acc

    install -m 0644 ${S}/axi_proxy_in_vivado.png ${D}/usr/share/notebooks/numpy-acc
    install -m 0644 ${S}/numpy_example.ipynb ${D}/usr/share/notebooks/numpy-acc
}

FILES_${PN} = "/usr/share/notebooks/numpy-acc"

RDEPENDS_${PN} = "python3-fpga-mngr-interface"

