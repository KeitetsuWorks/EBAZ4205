# How to Build 

## Vivado Project (Optional)

1. Change directory and launch Vivado GUI

    ```console
    $ cd ./vivado
    $ vivado
    ```

1. Run the following command in the Tcl console to create a Vivado project and generate a block design

    ```
    source ./ebaz4205.tcl
    ```

1. Run implementation from Flow Navigator

1. In Vivado, select File -> Export -> Export Hardware

1. In the Export Hardware Platform dialog box, select the Include bitstream check box and export XSA file


## PetaLinux Project

1. Update Hardware Description (Optional)

    ```console
    $ # Change to the petalinux project directory
    $ cd ./linux/ebaz4205
    $ # Specify the directory where you exported the XSA file
    $ # Exit the configuration without any changes
    $ petalinux-config --get-hw-description=../../vivado/ebaz4205
    ```

1. Build Linux

    ```console
    $ # Change to the petalinux project directory
    $ cd ./linux/ebaz4205
    $ # Build
    $ petalinux-build
    $ # Make BOOT.BIN
    $ ./make_BOOT.BIN.sh
    ```


## microSD card

1. Create two partitions on the microSD card using fdisk or the like

    |Device|Boot|Size|Type|
    |----|----|---:|----|
    |/dev/sdX1|*|16MByte+|FAT32|
    |/dev/sdX2||128MByte+|EXT4|

    ```console
    $ # Change to the petalinux project directory
    $ cd ./linux/ebaz4205
    $ cd ./images/linux
    $ # Unmount all microSD card partitions
    $ sudo umount /path/to/mountpoint
    $ # Create two partitions
    $ sudo fdisk /dev/sdX
    $ sudo mkfs.msdos -n ZYNQ_BOOT /dev/sdX1
    $ sudo mkfs.ext4 /dev/sdX2
    ```

1. Copy the files to the first partition and write rootfs to the second partition

    ```console
    $ # Mount the first partition and copy the files
    $ sudo mount /dev/sdX1 /path/to/mountpoint
    $ cp BOOT.BIN boot.scr image.ub /path/to/mountpoint
    $ sudo umount /path/to/mountpoint
    $ # Write rootfs. If the second partition is mounted, unmount it before dd command
    $ sudo umount /dev/sdX2
    $ sudo dd if=rootfs.ext4 of=/dev/sdX2
    $ sudo resize2fs /dev/sdX2
    ```
