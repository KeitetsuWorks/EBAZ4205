FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://disable-cpu-power-management.cfg \
            file://disable-unused-drivers.cfg \
            "

