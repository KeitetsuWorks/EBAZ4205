# GPIO Demo Application for Zynq EBAZ4205 Board {#mainpage}

## Description

This is a demo application of LED and tactile switches on a Zynq EBAZ4205 board using the leds-gpio and gpio-keys drivers.

## How to Build

Running `petalinux-build` will generate a root filesystem containing this application. To disable the built-in of this application, use `petalinux-config -c rootfs` command.

## How to Use

After logging in to Linux, run the application with `gpio-demo` command.  
The message "Ready to use the LED" is displayed and the green LED (LED) blinks.
The blinking speed can be changed in the range of 0 to 9. The default is 5.
If you press the tactile switch (S3), the blinking speed will slow down.
If you press the tactile switch (S2), the blinking speed will increase.  
You can quit the application with `Ctrl + C` or `Ctrl + \` 

```console
root@ebaz4205:~# gpio-demo
Ready to use the LED
Blinking Speed: 6 # Blinking speed increases with S3
Blinking Speed: 7
Blinking Speed: 8
Blinking Speed: 9
Blinking Speed: 9
Blinking Speed: 9
Blinking Speed: 8 # Blinking speed slows down with S2
Blinking Speed: 7
Blinking Speed: 6
Blinking Speed: 5
Blinking Speed: 4
Blinking Speed: 3
Blinking Speed: 2
Blinking Speed: 1
Blinking Speed: 0
Blinking Speed: 0
Blinking Speed: 0
Blinking Speed: 1
Blinking Speed: 2
Blinking Speed: 3
Blinking Speed: 4
Blinking Speed: 5
^Croot@ebaz4205:~# # Exit with Ctrl + C
```

## Doxygen Document

To generate document for this application with Doxygen, run the following commands.

```console
$ sudo apt update
$ sudo apt install -y doxygen-gui graphviz
$ cd /path/to/gpio-demo
$ doxygen
```

You can view the document in your browser.

```console
$ google-chrome ./spec_html/index.html
```

## References

* [Demo Application - Linux GPIO Driver - Xilinx Wiki - Confluence](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18842398/Linux+GPIO+Driver#LinuxGPIODriver-DemoApplication)
