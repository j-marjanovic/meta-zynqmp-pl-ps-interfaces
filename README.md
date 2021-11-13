# meta-zynqmp-pl-ps-interfaces

## Usage

```
mkdir zynqmp-pl-ps-interfaces
cd zynqmp-pl-ps-interfaces

repo init -u https://github.com/Xilinx/yocto-manifests.git -b rel-v2021.1
repo sync

cd sources
git clone -b 2021.1 https://github.com/Avnet/meta-avnet.git
TODO: clone my layer

docker run -it --rm -v /work:/work dr-yocto:18.04

source ./setupsdk
bitbake-layers add-layer ../sources/meta-avnet
bitbake-layers add-layer ../sources/meta-zynqmp-pl-ps-interfaces

echo 'MACHINE = "u96v2-sbc"' >> conf/local.conf
echo 'EXTRA_IMAGE_FEATURES += " fpga-manager"' >> conf/local.conf

bitbake avnet-image-minimal
bitbake xilinx-bootbin
```

