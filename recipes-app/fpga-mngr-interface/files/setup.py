#!/usr/bin/env python3

from distutils.core import setup

setup(
    name="fpga-mngr-interface",
    version="0.1",
    description="FPGA Manager interface (program PL from PS)",
    author="Jan Marjanovic",
    author_email="jan.marjanovic@outlook.com",
    packages=["fpga_mngr_interface"],
    entry_points={
        "console_scripts": ["fmi = fpga_mngr_interface.FpgaManagerInterface.main"]
    },
)
