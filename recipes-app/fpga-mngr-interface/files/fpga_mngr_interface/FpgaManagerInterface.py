#! /usr/bin/env python3

import argparse
import logging
import os
import subprocess
import sys


class FpgaManagerInterface:
    """An interface to the FPGA manager, similar to `fpgautil`"""

    CONFIGFS_PATH = "/configfs"
    FULL_OVERLAY_PATH = "/configfs/device-tree/overlays/full"
    FIRMWARE_PATH = "/lib/firmware"
    FPGA_MNGR_DEBUGFS_PATH = "/sys/kernel/debug/fpga/fpga0/image"

    def __init__(self):
        self.logger = logging.getLogger(__class__.__name__)

    def program_bitstream(self, bit_full_path, dtbo_full_path):
        """Program the bitstream"""

        self._check_files(bit_full_path, dtbo_full_path)

        self._ensure_cfs_mounted()
        self._create_dtbo_full()

        bit_base = os.path.basename(bit_full_path)
        dtbo_base = os.path.basename(dtbo_full_path)
        self._create_link(bit_full_path, bit_base)
        self._create_link(dtbo_full_path, dtbo_base)

        self._write_overlay(dtbo_base)

    def remove(self):
        """Remove the device tree overlay"""

        try:
            os.rmdir(self.FULL_OVERLAY_PATH)
        except FileNotFoundError:
            self.logger.info("[remove] overlay dir does not exists, nothing to do")

    def is_programmed(self):
        """Check if the FPGA is programmed"""

        overlay_exists = os.path.exists(self.FULL_OVERLAY_PATH)
        try:
            open(self.FPGA_MNGR_DEBUGFS_PATH, encoding="ascii").read(1024)
            debugfs_exists = True
        except PermissionError:
            debugfs_exists = False

        self.logger.debug(
            "[is prog] overlay exists: %s, debugfs exists: %s",
            overlay_exists,
            debugfs_exists,
        )
        return overlay_exists and debugfs_exists

    @staticmethod
    def _check_files(bit_full_path, dtbo_full_path):
        """Check if files even exists"""

        assert os.path.exists(bit_full_path), "Bitstream file does not exists"
        assert os.path.exists(dtbo_full_path), "dtbo file does not exists"

    def _ensure_cfs_mounted(self):
        """Check if configfs is mounted, and if that is not the case, mount it"""

        is_mount = os.path.ismount(self.CONFIGFS_PATH)

        if is_mount:
            self.logger.debug("[mount] configfs is already mounted, doing nothing")
            return

        self.logger.debug("[mount] Mouting configfs...")

        if not os.path.exists(self.CONFIGFS_PATH):
            self.logger.debug("[mount] Creating configfs directory ...")
            os.mkdir(self.CONFIGFS_PATH)

        subprocess.check_output(
            ["mount", "-t", "configfs", "configfs", self.CONFIGFS_PATH]
        )
        self.logger.debug("[mount] configfs mounted")

    def _create_dtbo_full(self):
        """Create .../full directory if it does not exits"""

        if os.path.exists(self.FULL_OVERLAY_PATH):
            self.logger.debug(
                "[dtbo_full] `full` directory already exists, doing nothing"
            )
            return

        self.logger.debug("[dtbo_full] mkdir %s", self.FULL_OVERLAY_PATH)
        os.mkdir(self.FULL_OVERLAY_PATH)

    def _create_link(self, dtbo_full_path: str, dtbo_base: str):
        """Create links in the firmware directory"""
        src = dtbo_full_path
        dst = os.path.join(self.FIRMWARE_PATH, dtbo_base)
        self.logger.debug("[link] %s --> %s", src, dst)
        try:
            os.remove(dst)
        except FileNotFoundError:
            pass
        os.symlink(src, dst)

    def _write_overlay(self, dtbo_filename: str):
        """Write .dtbo filename into the full overlay file"""

        self.logger.debug("[write] writing %s to `.../full/path`", dtbo_filename)
        full_path = os.path.join(self.FULL_OVERLAY_PATH, "path")
        open(full_path, "w", encoding="ascii").write(dtbo_filename)


def main():
    parser = argparse.ArgumentParser(
        description="A utility to program the FPGA (PL) from ARM (PS) in Zynq MPSoC"
    )

    parser.add_argument(
        "-d",
        "--debug",
        action="store_true",
        help="enable debug output",
    )
    parser.add_argument(
        "-b",
        "--bitstream",
        help="bitstream file",
    )
    parser.add_argument(
        "-o",
        "--overlay",
        help="DT overlay file",
    )
    parser.add_argument(
        "-R",
        "--remove",
        action="store_true",
        help="remove overlay",
    )
    parser.add_argument(
        "-?",
        "--check",
        action="store_true",
        help="check if the FPGA is programmed",
    )

    args = parser.parse_args()
    if args.debug:
        logging.basicConfig(level=logging.DEBUG)
    else:
        logging.basicConfig(level=logging.INFO)

    fmi = FpgaManagerInterface()

    if args.bitstream is not None and args.overlay is not None:
        fmi.program_bitstream(args.bitstream, args.overlay)
    elif args.bitstream is not None and args.overlay is None:
        print("Bitstream also requires DT overlay to be specified")
        sys.exit(os.EX_OSFILE)
    elif args.bitstream is None and args.overlay is not None:
        print("DT overlay also requires bitstream to be specified")
        sys.exit(os.EX_OSFILE)
    elif args.remove:
        fmi.remove()
    elif args.check:
        is_prog = fmi.is_programmed()
        if is_prog:
            print("PL is programmed")
        else:
            print("PL is NOT programmed")


if __name__ == "__main__":
    main()
