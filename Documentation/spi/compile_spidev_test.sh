#!/bin/sh

ARCH=arm
CROSSCOMPILE=/opt/poky/1.7/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-

${CROSS_COMPILE}gcc -o spidev_test spidev_test.c -I ../../usr/include -mfloat-abi=hard

