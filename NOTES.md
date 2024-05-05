# FireBeetle 2 ESP32 C6

https://www.dfrobot.com/product-2771.html

[Schematic](https://dfimg.dfrobot.com/5d57611a3416442fa39bffca/wiki/65df25004a7d1e8bc128894c75ce4089.pdf)

[Wiki](https://wiki.dfrobot.com/SKU_DFR1075_FireBeetle_2_Board_ESP32_C6)

- SRAM: 512 KiB
- Flash: 4 MiB

Hardware:

 - BOOT button on GPIO9
 - LED on GPIO15
 - VBAT on a 1M:1M voltage divider on Pin 6 / XTAL_32K_P / GPIO0

# esphome

Closest board is `esp32-c6-devkitm-1`. Has 4 MiB flash.

ADC:

- GPIO2
-