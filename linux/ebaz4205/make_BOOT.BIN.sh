#!/bin/bash -eu

petalinux-package \
    --boot \
    --force \
    --fsbl ./images/linux/zynq_fsbl.elf \
    --fpga ./project-spec/hw-description/ebaz4205_wrapper.bit \
    --u-boot

