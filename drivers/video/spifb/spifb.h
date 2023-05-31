#define SPIFB_DISPLAY_GENERIC_LCD 0
#define SPIFB_WIDTH 480
#define SPIFB_HEIGHT 272
#define SPIFB_BITS_PER_PIXEL 32


struct spifb_par {
	struct spi_device *spi;
	struct fb_info *info;
	u8 *buf;
};