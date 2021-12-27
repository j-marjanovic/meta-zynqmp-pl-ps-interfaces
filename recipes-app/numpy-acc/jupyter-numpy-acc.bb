DESCRIPTION = "Jupyter notebook demonstrating Numpy acceleration"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=bd67c54ff052244b70a29a4f4591a6ea"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI = "\
    file://FpgaManagerInterface.py \
    file://axi_proxy_in_vivado.png \
    file://numpy_example_test.ipynb \
    file://LICENSE \
"

B = "${WORKDIR}"
S = "${WORKDIR}"

do_install() {
    install -d ${D}/usr/share/notebooks

    install -m 0644 ${S}/FpgaManagerInterface.py ${D}/usr/share/notebooks
    install -m 0644 ${S}/axi_proxy_in_vivado.png ${D}/usr/share/notebooks
    install -m 0644 ${S}/numpy_example_test.ipynb ${D}/usr/share/notebooks
}

FILES_${PN} = "/usr/share/notebooks"

