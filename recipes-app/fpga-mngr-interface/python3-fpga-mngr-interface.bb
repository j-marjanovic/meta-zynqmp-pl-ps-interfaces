SUMMARY = "FPGA Manager interface in Python3"
LICENSE = "MIT"
SECTION = "devel/python"
LIC_FILES_CHKSUM = "file://LICENSE;md5=bd67c54ff052244b70a29a4f4591a6ea"

PV = "1.0"
PR = "r2"

inherit setuptools3

SRC_URI = " \
    file://LICENSE \
    file://setup.py \
    file://fpga_mngr_interface/__init__.py \
    file://fpga_mngr_interface/FpgaManagerInterface.py \
"

S = "${WORKDIR}"
B = "${WORKDIR}"

