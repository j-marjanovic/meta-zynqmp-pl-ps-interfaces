SUMMARY = "Python Library to interface with Zynq devices"
LICENSE = "MIT"
SECTION = "devel/python"
LIC_FILES_CHKSUM = "file://LICENSE;md5=bd67c54ff052244b70a29a4f4591a6ea"

inherit setuptools3

SRC_URI = " \
    file://LICENSE \
    file://zynqmp_pl_ps/UioDev.py \
    file://zynqmp_pl_ps/_AxiTrafficGenMap.py \
    file://zynqmp_pl_ps/AxiTrafficGen.py \
    file://zynqmp_pl_ps/_AxiProxyMap.py \
    file://zynqmp_pl_ps/AxiProxy.py \
    file://zynqmp_pl_ps/__init__.py \
    file://zynqmp_pl_ps/Udmabuf.py \
    file://setup.py \
"

S = "${WORKDIR}"
B = "${WORKDIR}"

