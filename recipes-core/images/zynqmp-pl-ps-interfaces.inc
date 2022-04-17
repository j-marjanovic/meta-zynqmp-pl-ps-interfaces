
IMAGE_INSTALL_remove_u96v2-sbc = "\
    openamp-fw-echo-testd \
    openamp-fw-mat-muld \
    openamp-fw-rpc-demo \
    packagegroup-petalinux-openamp \
    packagegroup-petalinux-ultra96-webapp \
    packagegroup-petalinux-96boards-sensors \
    ultra96-radio-leds \
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
    app-axi-proxy \
    python3-jupyter \
    start-jupyternb \
    python3-fpga-mngr-interface \
    python3-numpy \
    python3-matplotlib \
    python3-pillow \
    jupyter-numpy-acc \
    jupyter-zynqmp-pl-ps \
"

