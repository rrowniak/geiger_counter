# Geiger-Muller counter

This is a simple firmware written for Attiny 2313 which controls a Geiger-Muller counter.

The electronic part is based on http://mirley.firlej.org/kieszonkowy_licznik_geigera. However, I have introduced some modifications; the microcontroller is Attiny 2313 (less powerfull than the original one) and the display is changed to alfanumeric 16x2. That gives oportunity to provide with more info and get rid of LEDs.

Original LCD driver implementation is available here: https://github.com/aostanin/avr-hd44780

## Features

The counter measures cps in realtime and ionizing radiation dose (uSv/h) is measured in 30 seconds intervals. Cound-down timer is displayed.

The counter measures range 0.1-600 uSv/h and provides radiation levels: low, !, !!, !OL. The last one means both overflow and dangerous radiation level.

## Limitations

Currently only uSv/h unit is supported. Other units like R/h, rem/h, cpm could be easily introduced.

Upper limit is ~600 uSv/h, this limit could be also extended to e.g. 1000 uSv/h. But that modification might be related to the electronic part as well.


## License

GPL-3.0

