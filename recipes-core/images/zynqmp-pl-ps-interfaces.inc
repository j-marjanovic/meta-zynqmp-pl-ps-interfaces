
IMAGE_INSTALL_remove_u96v2-sbc = "\
    openamp-fw-echo-testd \
    openamp-fw-mat-muld \
    openamp-fw-rpc-demo \
    packagegroup-petalinux-openamp \
"

EXTRA_IMAGE_FEATURES = "debug-tweaks tools-profile"
TOOLCHAIN_TARGET_TASK_append = " kernel-devsrc"

IMAGE_INSTALL_append = "\
    fpga-manager-util \
    xilinx-xvc-driver \
    xilinx-xvc-server \
    bash \
"
