# EBAZ4205

## Description

This repository contains the Vivado and PetaLinux projects required to use the Zynq EBAZ4205 Board.


## Requirement

### Hardware

* Zynq EBAZ4205 Board (cost-reduced version)
  * **No** 25MHz crystal (Y3) is required. The Ethernet transceiver (U24) clock is supplied by the ZYNQ (U31). However, it also works on a board on which a crystal is mounted
  * microSD card slot (U7) required
  * SD card boot support is required. Short the resistor (R2577)
  * Short the diode (D24) to supply power from the power connector (J4) (Optional)
  * Mount the tactile switch (S3), the capacitor (C2410) and the resistor (R2641A). The resistor (R2641A) can be shorted instead of mounting a 0 ohm resistor. I used 4.7uF for the capacitor (C2410) (Optional)


### Software

* Xilinx Vivado 2021.1
* Xilinx PetaLinux 2021.1


## How to Build 

* [How to Build](./docs/how-to-build.md)


## Demo Application

* [GPIO Demo Application for Zynq EBAZ4205 Board](./linux/ebaz4205/project-spec/meta-user/recipes-apps/gpio-demo/README.md)


## References

### EBAZ4205

* [xjtuecho/EBAZ4205: A 5$ Xilinx ZYNQ development board.](https://github.com/xjtuecho/EBAZ4205)
  * First setup
  * Schematics
  * Xilinx Design Constraints
  * mtd information
* [Leungfung/ebaz4205_hw](https://github.com/Leungfung/ebaz4205_hw)
  * Document (Block desgin)
* [kan573 - Qiita](https://qiita.com/kan573)
  * Articles in Japanese
* [blkf2016/ebaz4205: Some resources for the ebaz4205 board](https://github.com/blkf2016/ebaz4205)
  * Sample project
* [nightseas/ebit_z7010: The base reference design for EBIT EBAZ4205 Zynq7010 board.](https://github.com/nightseas/ebit_z7010)
  * Sample project


### Xilinx

* [UG585 - Zynq-7000 SoC Technical Reference Manual (v1.12.2)](https://www.xilinx.com/support/documentation/user_guides/ug585-Zynq-7000-TRM.pdf)
* [UG585 - Zynq-7000 SoC テクニカル リファレンス マニュアル (v1.10)](https://japan.xilinx.com/support/documentation/user_guides/j_ug585-Zynq-7000-TRM.pdf)
* [UG865 - Zynq-7000 SoC Packaging Guide (v1.8.1)](https://www.xilinx.com/support/documentation/user_guides/ug865-Zynq-7000-Pkg-Pinout.pdf)
* [UG865 - Zynq‐7000 SoC パッケージ ガイド (v1.8.1)](https://japan.xilinx.com/support/documentation/user_guides/j_ug865-Zynq-7000-Pkg-Pinout.pdf)
* [XC7Z010CLG400 ピン配置ファイル](https://japan.xilinx.com/support/packagefiles/z7packages/xc7z010clg400pkg.txt)
* [UG1144 - PetaLinux Tools Documentation Reference Guide (v2021.1)](https://www.xilinx.com/support/documentation/sw_manuals/xilinx2021_1/ug1144-petalinux-tools-reference-guide.pdf)
* [LEDS-GPIO Driver - Linux GPIO Driver - Xilinx Wiki - Confluence](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18842398/Linux+GPIO+Driver#LinuxGPIODriver-LEDS-GPIODriver)
* [GPIO-Keys Driver - Linux GPIO Driver - Xilinx Wiki - Confluence](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18842398/Linux+GPIO+Driver#LinuxGPIODriver-GPIO-KeysDriver)
* [Vitis Tutorials 2021.1 - Vitis Platform Creation Tutorials](https://xilinx.github.io/Vitis-Tutorials/2021-1/build/html/docs/Vitis_Platform_Creation/Vitis_Platform_Creation.html)
* [Vitis チュートリアル - Vitis プラットフォームの作成チュートリアル](https://xilinx.github.io/Vitis-Tutorials/master/docs-jp/docs/Vitis_Platform_Creation/Vitis_Platform_Creation.html)


### Linux Kernel

* [LEDs connected to GPIO lines](https://www.kernel.org/doc/Documentation/devicetree/bindings/leds/leds-gpio.txt)
* [Common leds properties](https://www.kernel.org/doc/Documentation/devicetree/bindings/leds/common.yaml)
* [Device-Tree bindings for input/keyboard/gpio_keys.c keyboard driver](https://www.kernel.org/doc/Documentation/devicetree/bindings/input/gpio-keys.txt)


### Others

* [Vivadoでプロジェクトのエクスポートを極める - Qiita](https://qiita.com/nahitafu/items/de4b295ea60ce6173a83)
* [MII通信　～MACとPHYをつなぐインターフェース～ - 半導体事業 - マクニカ](https://www.macnica.co.jp/business/semiconductor/articles/microchip/134946/)
* [ARM PrimeCell Static Memory Controller (PL350 series) Revision: r2p1 Technical Reference Manual](https://developer.arm.com/documentation/ddi0380/g/?lang=en)
* [Winbond W29N01HVxINA Datasheet](https://www.winbond.com/resource-files/w29n01hvxina_revc.pdf)


## License

* MIT
