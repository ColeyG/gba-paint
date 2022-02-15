#!/usr/bin/env bash

/opt/devkitpro/devkitARM/bin/arm-none-eabi-gcc -c src/main.c -mthumb-interwork -fno-strict-aliasing -mthumb -O2 -o bin/main.o
/opt/devkitpro/devkitARM/bin/arm-none-eabi-gcc bin/main.o -mthumb-interwork -mthumb -specs=gba.specs -o bin/main.elf
/opt/devkitpro/devkitARM/bin/arm-none-eabi-objcopy -v -O binary bin/main.elf bin/main.gba
