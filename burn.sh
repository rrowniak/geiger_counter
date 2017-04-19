#!/bin/bash

sudo avrdude -c usbasp -p attiny2313 -U lfuse:w:0x64:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m -U flash:w:LCD_TINY.hex -B 3
