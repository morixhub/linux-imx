
# spifb
The spifb project is a tentative approach to implement a Linux framebuffer served via spifb

## Quick-start
For compiling the driver simply trigger a "make" command in the driver folder; remember to set ARCH and CROSS_COMPILE environment variables, if requested. Then copy the "spifb.ko" to target system and try loading it via "insmod spifb.ko" (readout error messages from dmesg, if any).

For SPI device probing to be triggered the device tree of the system must be set correspondingly. On Aesys 1504B, for example, do the following:

    &ecspi1 {
    
            spifb@0 {
                    compatible = "aesys,spifb";
                    reg = <0>;
                    spi-max-frequency = <10000000>;
                    spi-cs-high;
            };
    };
