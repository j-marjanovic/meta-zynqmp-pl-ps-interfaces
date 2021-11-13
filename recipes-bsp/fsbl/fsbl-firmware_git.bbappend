
YAML_COMPILER_FLAGS_append = " -DFSBL_DEBUG"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
SRC_URI += " \
	file://0001-Enable-CCI-debug-NIDEN-and-SPINDEN-on-CCI-400.patch \
	file://0002-Enable-CCI-snoop-on-APU-port-port-S3-on-CCI400.patch \
	file://0003-Enable-sharing-on-the-HPC-port-port-S0-on-CCI400.patch \
"

