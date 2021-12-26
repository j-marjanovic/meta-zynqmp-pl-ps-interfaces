
IMAGE_INSTALL_remove_u96v2-sbc = "\
    openamp-fw-echo-testd \
    openamp-fw-mat-muld \
    openamp-fw-rpc-demo \
    packagegroup-petalinux-openamp \
"

IMAGE_FSTYPES_remove = "jffs2 cpio cpio.gz"

EXTRA_IMAGE_FEATURES = "debug-tweaks tools-profile"
TOOLCHAIN_TARGET_TASK_append = " kernel-devsrc"

IMAGE_INSTALL_append = "\
    fpga-manager-util \
    xilinx-xvc-driver \
    xilinx-xvc-server \
    u-dma-buf \
    bash \
    app-lamports-bakery-algo \
    app-axi-traffic-gen \
"

