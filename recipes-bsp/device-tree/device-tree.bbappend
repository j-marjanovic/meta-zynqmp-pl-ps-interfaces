FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append_u96v2-sbc = " \
  file://system-conf.dtsi \
"

do_configure_append () {

  # override .dts provided by Avnet - it contains some questionable config
  cp ${WORKDIR}/system-conf.dtsi ${DT_FILES_PATH}/system-bsp.dtsi

  echo "" > ${WORKDIR}/openamp.dtsi

}

