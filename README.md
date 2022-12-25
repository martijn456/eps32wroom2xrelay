# ESP32 wroom 2x relay firmware to drive a Sparta Ion motor.
This is a fork from viod-spark he has written this code and is still working on it, there are regular updates. At the moment this firmware works on the bike but there are still some things that can be improved. So i didn't write this code i just modified it a bit for the esp32 wroom 2x relay. The goal for me is to have a solution for the banana battery (Sparta, Batavus, Koga) as cheap and as easily as possible. When you use the esp32 wroom 2x relay on a 36 volt system then you run the risk that the relay cannot handle this here the relay can handle max 30 volts 10A on a 24 volt system i have not encountered any problems yet. For flashing the board you can use online flash software only works with Chrome Link to flasher: https://espressif.github.io/esptool-js/

- 1000 = bootloader.bin
- 8000 = partition-table.bin
- 10000 = ion1-nowifi.bin


More information can be found here:
https://www.pedelecforum.de/forum/index.php?threads/zweiter-fruehling-fuer-ion-antrieb-sparta-batavus-koga.90186/

![foto3](https://user-images.githubusercontent.com/48860011/202907439-33300e9e-e016-4954-8844-a1e31efc0116.jpg)

For sale on: https://nl.aliexpress.com/item/1005004244895380.html?spm=a2g0o.ppclist.product.70.7d5ezVYvzVYvvW&pdp_npi=2%40dis%21EUR%21%E2%82%AC%2011%2C14%21%E2%82%AC%208%2C02%21%21%21%21%21%40211b442016689517013615945e3218%2112000028506964651%21btf&_t=pvid%3A91865980-3bbb-4290-836d-310a92afeaae&afTraceInfo=1005004244895380__pc__pcBridgePPC__xxxxxx__1668951701&gatewayAdapt=glo2nld

# Pins used on the esp32 wroom 2x relay.
- 34 - ADC
- 35 - charge GND
- 16 - Light
- 17 - Relay
- 21 - RX
- 22 - TX


# Banana battery file for the 3d printer for the housing.
Housing consists of 4 parts and a top and bottom, also a mold has been added to weld the cells.
The house can be glued together there after melting together with a soldering iron. When you use 18650 cells you need a suitable BMS and can no longer use the original ion charger you need a lion charger.

I want to change the housing a bit:
- 1 at the bottom I would like a hole for a glass fuse holder
- 2 Reinforcement where the screws come to secure the battery in the bicycle

[Battery STL.zip](https://github.com/martijn456/esp32wroom2xrelay/files/10049747/Battery.STL.zip)

![banaan](https://user-images.githubusercontent.com/48860011/202905891-85e28ba2-2510-446c-997a-edc0be11ee79.jpg)

# PCB for the esp32 wroom 2x relay.

Design made by amaker2 when i get the gerben file's from amaker2 i will upload the file's here.
PCB fits right on top of the esp32 wroom 2x relay to take up as little space as possible
I havent tested the PCB when have the right files, i want to make one later and I will test it.

![visualisation-PCB-KOGA-24V-WROOM-2ch-VER_1_0_2_2](https://user-images.githubusercontent.com/48860011/205482686-4ec94d61-19d3-4d0a-96ba-10f4e1b58ca0.JPG)
![PCB - wroom-relay x2 - scheme](https://user-images.githubusercontent.com/48860011/202905905-8e9aedb5-467d-4f33-bae5-79e0d8726dae.JPG)


# HANDLING OF LI-ION BATTERY CELLS - RISKS AND PRECAUTIONS.
The user must have appropriate understanding of lithium ion batteries.
Use caution when working with and using lithium ion batteries as they are very sensitive to charging characteristics and may explode, burn, or cause a fire if misused or mishandled. Always charge in or on a fire-proof surface. Never leave batteries charging unattended. Charge only with a smart charger designed for this specific type of lithium ion battery.

- MISUSING OR MISHANDLING LITHIUM ION BATTERIES CAN POSE A SERIOUS RISK OF PERSONAL INJURY, PROPERTY DAMAGE, OR DEATH
- BATTERIES MAY EXPLODE, BURN, OR CAUSE A FIRE IF MISUSED OR MISHANDLED
- ONLY USE WITH PROPER CIRCUITRY IN A PROTECTED BATTERY PACK
- ONLY USE WITHIN MANUFACTURER LISTED SPECIFICATIONS
- DO NOT STORE LOOSE BATTERY IN A POCKET, PURSE ETC. - ALWAYS USE A PROTECTIVE CASE OR BOX
- KEEP AWAY from metal objects to prevent short circuiting
- DO NOT short circuit
- DO NOT use if wrapper or insulator is damaged or torn
- DO NOT use if damaged in any way
- DO NOT overcharge or over-discharge
- DO NOT modify, disassemble, puncture, cut, crush, or incinerate
- DO NOT expose to liquids or high temperatures
- DO NOT solder, spot weld only
- User must be familiar with handling lithium ion batteries
- Usage of batteries is AT YOUR OWN RISK
- ALWAYS charge in or on a fire-proof surface and never leave charging batteries unattended 
