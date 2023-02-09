# Geiger-Muller counter

This repository contains firmware for the Attiny 2313 microcontroller which controls a Geiger-Muller counter. The electronic part is based on the design from "http://mirley.firlej.org/kieszonkowy_licznik_geigera" with modifications such as using Attiny 2313 as the microcontroller and an alphanumeric 16x2 display for providing more information.

The counter has a range of 0.1-600 uSv/h, with the ability to measure radiation levels and provide a warning in case of dangerous radiation levels. This project is important for individuals who want to keep track of their surrounding's radiation levels, ensuring safety and peace of mind.

Original LCD driver implementation is available here: https://github.com/aostanin/avr-hd44780

## Features

The counter measures real-time cps and provides ionizing radiation dose measurement (uSv/h) in 30-second intervals. A countdown timer is also displayed. The counter has a measurement range of 0.1-600 uSv/h, providing radiation levels such as low, !, !!, and !OL (overflow and dangerous radiation level).

## Limitations

Please note that currently, only the uSv/h unit is supported, although other units like R/h, rem/h, cpm can be easily introduced. The upper limit is approximately 600 uSv/h, and can be extended to 1000 uSv/h with additional modifications to the electronic part.

## License

This repository is licensed under GPL-3.0.

