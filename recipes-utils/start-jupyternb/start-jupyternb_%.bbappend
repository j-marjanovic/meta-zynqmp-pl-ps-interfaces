
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append = " \
    file://jupyter_notebook_zynqmp_pl_ps.py \
    file://start-jupyter_zynqmp_pl_ps.sh \
"

do_install_append() {
    install -m 0755 ${WORKDIR}/start-jupyter_zynqmp_pl_ps.sh ${D}${base_sbindir}/start-jupyter.sh
    install -m 0644 ${WORKDIR}/jupyter_notebook_zynqmp_pl_ps.py ${D}${sysconfdir}/jupyter/jupyter_notebook_config.py
}

