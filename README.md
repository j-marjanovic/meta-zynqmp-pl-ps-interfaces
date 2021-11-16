# meta-zynqmp-pl-ps-interfaces

## Usage

### Preparation

```
mkdir zynqmp-pl-ps-interfaces
cd zynqmp-pl-ps-interfaces

repo init -u https://github.com/Xilinx/yocto-manifests.git -b rel-v2021.1
repo sync

cd sources
git clone -b 2021.1 https://github.com/Avnet/meta-avnet.git
git clone -b rel-v2021.1 https://github.com/j-marjanovic/meta-zynqmp-pl-ps-interfaces.git
```

### Compilation

```
docker run -it --rm -v /work:/work dr-yocto:18.04

source ./setupsdk
bitbake-layers add-layer ../sources/meta-avnet
bitbake-layers add-layer ../sources/meta-zynqmp-pl-ps-interfaces

echo 'MACHINE = "u96v2-sbc"' >> conf/local.conf
echo 'EXTRA_IMAGE_FEATURES += " fpga-manager"' >> conf/local.conf
echo 'MACHINE_FEATURES_append = " fpga-overlay"' >> conf/local.conf

# TODO: rename the image
bitbake avnet-image-minimal
bitbake xilinx-bootbin
```

### Deployement

`<DEPLOY_DIR>` is `<TOP_DIR>/build/tmp/deploy/images/u96v2-sbc/`

```
cp <DEPLOY_DIR>/Image <BOOT_PARTITION>
cp <DEPLOY_DIR>/boot.bin  <BOOT_PARTITION>
cp <DEPLOY_DIR>/avnet-boot/avnet_u96v2_dtb_boot.scr <BOOT_PARTITION>/boot.scr
cp <DEPLOY_DIR>/u96v2-sbc-system.dtb <BOOT_PARTITION>

sudo tar xzvf <DEPLOY_DIR>/avnet-image-minimal-u96v2-sbc.tar.gz -C <ROOTFS_PARTITION>
```
