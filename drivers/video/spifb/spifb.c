#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fb.h>
#include <linux/gpio.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>
#include <linux/uaccess.h>


#include "spifb.h"

static struct fb_fix_screeninfo spifb_fix = {
	.id		= "SPIFB",
	.type		= FB_TYPE_PACKED_PIXELS,
	.visual		= FB_VISUAL_TRUECOLOR,
	.xpanstep	= 0,
	.ypanstep	= 0,
	.ywrapstep	= 0,
	.line_length	= SPIFB_WIDTH*(SPIFB_BITS_PER_PIXEL / 8),
	.accel		= FB_ACCEL_NONE,
};

static struct fb_var_screeninfo spifb_var = {
	.xres		= SPIFB_WIDTH,
	.yres		= SPIFB_HEIGHT,
	.xres_virtual	= SPIFB_WIDTH,
	.yres_virtual	= SPIFB_HEIGHT,
	.bits_per_pixel	= SPIFB_BITS_PER_PIXEL,
	.grayscale  = 0,
	.nonstd		= 0,
};

/* //!!
static int st7586_write(struct st7586fb_par *par, u8 data)
{
	int ret = 0;

	// Assert out-of-band CS
	if (par->cs)
		gpio_set_value(par->cs, 0);

	par->buf[0] = data;

	ret = spi_write(par->spi, par->buf, 1);

	// Deassert out-of-band CS
	if (par->cs)
		gpio_set_value(par->cs, 1);

	return ret;
}
*/

/* //!!
static void st7586_write_data(struct st7586fb_par *par, u8 data)
{
	int ret = 0;

	// Set data mode
	gpio_set_value(par->a0, 1);

	ret = st7586_write(par, data);
	if (ret < 0)
		pr_err("%s: write data %02x failed with status %d\n",
			par->info->fix.id, data, ret);
}
*/

/* //!!
static int st7586_write_data_buf(struct st7586fb_par *par,
					u8 *txbuf, int size)
{
	int ret = 0;

	// Set data mode
	gpio_set_value(par->a0, 1);

	// Assert out-of-band CS
	if (par->cs)
		gpio_set_value(par->cs, 0);

	// Write entire buffer
	ret = spi_write(par->spi, txbuf, size);

	// Deassert out-of-band CS
	if (par->cs)
		gpio_set_value(par->cs, 1);

	return ret;
} */

/* //!!
static void st7586_write_cmd(struct st7586fb_par *par, u8 data)
{
	int ret = 0;

	// Set command mode
	gpio_set_value(par->a0, 0);

	ret = st7586_write(par, data);
	if (ret < 0)
		pr_err("%s: write command %02x failed with status %d\n",
			par->info->fix.id, data, ret);
}
*/

/* //!!
static void st7586_clear_ddrram(struct st7586fb_par *par)
{
	u8 *buf;

	buf = kzalloc(128*128, GFP_KERNEL);
	st7586_write_cmd(par, ST7586_RAMWR);
	st7586_write_data_buf(par, buf, 128*128);
	kfree(buf);
} */

/* //!!
static void st7586_run_cfg_script(struct st7586fb_par *par)
{
	int i = 0;
	int end_script = 0;

	do {
		switch (st7586_cfg_script[i].cmd)
		{
		case ST7586_START:
			break;
		case ST7586_CLR:
			st7586_clear_ddrram(par);
			break;
		case ST7586_CMD:
			st7586_write_cmd(par,
				st7586_cfg_script[i].data & 0xff);
			break;
		case ST7586_DATA:
			st7586_write_data(par,
				st7586_cfg_script[i].data & 0xff);
			break;
		case ST7586_DELAY:
			mdelay(st7586_cfg_script[i].data);
			break;
		case ST7586_END:
			end_script = 1;
		}
		i++;
	} while (!end_script);
}
*/

/* //!!
static void st7586_set_addr_win(struct st7586fb_par *par,
				int xs, int ys, int xe, int ye)
{
	st7586_write_cmd(par, ST7586_CASET);
	st7586_write_data(par, 0x00);
	st7586_write_data(par, xs);
	st7586_write_data(par, 0x00);
	st7586_write_data(par, ((xe+2)/3)-1);
	st7586_write_cmd(par, ST7586_RASET);
	st7586_write_data(par, 0x00);
	st7586_write_data(par, ys);
	st7586_write_data(par, 0x00);
	st7586_write_data(par, ye-1);
}
*/

/* //!!
static void st7586_reset(struct st7586fb_par *par)
{
	// Reset controller
	gpio_set_value(par->rst, 0);
	mdelay(10);
	gpio_set_value(par->rst, 1);
	mdelay(120);
} */

/* //!!
static void st7586fb_update_display(struct st7586fb_par *par)
{
	int ret = 0;
	u8 *vmem = par->info->screen_base;

	st7586_set_addr_win(par, 0, 0, WIDTH, HEIGHT);
	st7586_write_cmd(par, ST7586_RAMWR);

	// Blast framebuffer to ST7586 internal display RAM
	ret = st7586_write_data_buf(par, vmem, (WIDTH+2)/3*HEIGHT);

	if (ret < 0)
		pr_err("%s: spi_write failed to update display buffer\n",
			par->info->fix.id);
}
*/

static void spifb_deferred_io(struct fb_info *info,
				struct list_head *pagelist)
{
	//!! st7586fb_update_display(info->par);
}

/* //!!
static int st7586fb_init_display(struct st7586fb_par *par)
{
        gpio_request(par->rst, "ST7586 Reset Pin");
	gpio_direction_output(par->rst, 1);
        gpio_request(par->a0, "ST7586 A0 Pin");
	gpio_direction_output(par->a0, 0);
	if (par->cs) {
		gpio_request(par->cs, "ST7586 CS Pin");
		gpio_direction_output(par->cs, 1);
	}

	st7586_reset(par);

	st7586_run_cfg_script(par);

	// Set row/column data window
	st7586_set_addr_win(par, 0, 0, WIDTH, HEIGHT);

	st7586_write_cmd(par, ST7586_DISPON);

	return 0;
}
*/

void spifb_fillrect(struct fb_info *info, const struct fb_fillrect *rect)
{
	//!! struct spifb_par *par = info->par;

	//!! sys_fillrect(info, rect);

	//!! st7586fb_update_display(par);
}

void spifb_copyarea(struct fb_info *info, const struct fb_copyarea *area)
{
	//!! struct spifb_par *par = info->par;

	//!! sys_copyarea(info, area);

	//!! st7586fb_update_display(par);
}

void spifb_imageblit(struct fb_info *info, const struct fb_image *image)
{
	//!! struct spifb_par *par = info->par;

	//!! sys_imageblit(info, image);

	//!! st7586fb_update_display(par);
}


static ssize_t spifb_read(struct fb_info *info, char __user *buf,
		size_t count, loff_t *ppos)
{
    unsigned long p = *ppos;
	void *src;
	int err = 0;
	unsigned long total_size;

	if (info->state != FBINFO_STATE_RUNNING)
		return -EPERM;

	total_size = info->screen_size;

	if (total_size == 0)
		total_size = info->fix.smem_len;

	if (p >= total_size)
		return 0;

	if (count >= total_size)
		count = total_size;

	if (count + p > total_size)
		count = total_size - p;

	src = (void __force *)(info->screen_base + p);

	if (info->fbops->fb_sync)
		info->fbops->fb_sync(info);

	if (copy_to_user(buf, src, count))
		err = -EFAULT;

	if  (!err)
		*ppos += count;

	return (err) ? err : count;
}


static ssize_t spifb_write(struct fb_info *info, const char __user *buf,
		size_t count, loff_t *ppos)
{
   	unsigned long p = *ppos;
	void *dst;
	int err = 0;
	unsigned long total_size;

	if (info->state != FBINFO_STATE_RUNNING)
		return -EPERM;

	total_size = info->screen_size;

	if (total_size == 0)
		total_size = info->fix.smem_len;

	if (p > total_size)
		return -EFBIG;

	if (count > total_size) {
		err = -EFBIG;
		count = total_size;
	}

	if (count + p > total_size) {
		if (!err)
			err = -ENOSPC;

		count = total_size - p;
	}

	dst = (void __force *) (info->screen_base + p);

	if (info->fbops->fb_sync)
		info->fbops->fb_sync(info);

	if (copy_from_user(dst, buf, count))
		err = -EFAULT;

	if  (!err)
		*ppos += count;

    //!! st7586fb_update_display(par);

	return (err) ? err : count;
}


static struct fb_ops spifb_ops = {
	.owner		= THIS_MODULE,
	.fb_read	= spifb_read,
	.fb_write	= spifb_write,
	.fb_fillrect	= cfb_fillrect /* spifb_fillrect */,
	.fb_copyarea	= cfb_copyarea /* spifb_copyarea */,
	.fb_imageblit	= cfb_imageblit /* spifb_imageblit */,
};

static struct fb_deferred_io spifb_defio = {
	.delay		= HZ / 20,
	.deferred_io	= spifb_deferred_io,
};

static int spifb_probe (struct spi_device *spi)
{
    int retval = -ENOMEM;

    /* //!!
	int chip = spi_get_device_id(spi)->driver_data;
	struct st7586fb_platform_data *pdata = spi->dev.platform_data;
    */

	int vmem_size = SPIFB_WIDTH * (SPIFB_BITS_PER_PIXEL / 8) * SPIFB_HEIGHT;
	u8 *vmem;
	struct fb_info *info;
	struct spifb_par *par;

    printk(KERN_INFO
		"spifb: probing...\n");
	
    /* //!!
	if (chip != ST7586_DISPLAY_GENERIC_LCD) {
		pr_err("%s: only the %s device is supported\n", DRVNAME,
			to_spi_driver(spi->dev.driver)->id_table->name);
		return -EINVAL;
	}

	if (!pdata) {
		pr_err("%s: platform data required for rst and a0 info\n",
			DRVNAME);
		return -EINVAL;
	} */

	vmem = (u8 *)kmalloc(vmem_size, GFP_KERNEL);
	if (!vmem)
		return retval;

	info = framebuffer_alloc(sizeof(struct spifb_par), &spi->dev);
	if (!info)
		goto fballoc_fail;

	info->screen_base = vmem;
	info->fbops = &spifb_ops;
	info->fix = spifb_fix;
	info->fix.smem_start = virt_to_phys(vmem);
	info->fix.smem_len = vmem_size;
	info->var = spifb_var;
	info->var.red.offset = 16;
	info->var.red.length = 8;
	info->var.green.offset = 8;
	info->var.green.length = 8;
	info->var.blue.offset = 0;
	info->var.blue.length = 8;
	info->var.transp.offset = 24;
	info->var.transp.length = 8;
	info->flags = FBINFO_FLAG_DEFAULT | FBINFO_VIRTFB;
	info->fbdefio = &spifb_defio;
	fb_deferred_io_init(info);

	par = info->par;
	par->info = info;
	par->spi = spi;
    /* //!!
	par->rst = pdata->rst_gpio;
	par->a0 = pdata->a0_gpio;
	par->cs = pdata->cs_gpio;
	par->buf = kmalloc(1, GFP_KERNEL);
    */

	retval = register_framebuffer(info);
	if (retval < 0)
		goto fbreg_fail;

	spi_set_drvdata(spi, info);

    /* //!!
	retval = st7586fb_init_display(par);
	if (retval < 0)
		goto init_fail;
    */

	printk(KERN_INFO
		"fb%d: %s frame buffer device, using %d KiB of video memory\n",
		info->node, info->fix.id, vmem_size/1024);

	return 0;

/* //!!
init_fail:
	spi_set_drvdata(spi, NULL);
    */

fbreg_fail:
	//!! kfree(par->buf);
	framebuffer_release(info);

fballoc_fail:
	kfree(vmem);

	return retval;
}

static int spifb_remove(struct spi_device *spi)
{
	struct fb_info *info = spi_get_drvdata(spi);

	if (info) {
		//!!struct spifb_par *par = info->par;

		unregister_framebuffer(info);
		
		//!!kfree(par->buf);
		//!!gpio_free(par->rst);
		//!!gpio_free(par->a0);
		//!!if (par->cs)
		//!!	gpio_free(par->cs);
		framebuffer_release(info);

		kfree(info->screen_base);
	}

	spi_set_drvdata(spi, NULL);

	return 0;
}

static const struct spi_device_id spifb_ids[] = {
	{ "spifb_lcd", SPIFB_DISPLAY_GENERIC_LCD },
	{ },
};

MODULE_DEVICE_TABLE(spi, spifb_ids);

static const struct of_device_id spifb_of_match[] = {
    { .compatible = "aesys,spifb" },
    { },
};

MODULE_DEVICE_TABLE(of, spifb_of_match);

static struct spi_driver spifb_driver = {
	.driver = {
		.name   = "spifbdrv",
		.owner  = THIS_MODULE,
        .of_match_table = of_match_ptr(spifb_of_match),
	},
	.id_table = spifb_ids,
	.probe  = spifb_probe,
	.remove = spifb_remove,
};

static int __init spifb_init(void)
{
    printk(KERN_INFO
		"spifb: initializing...\n");

	return spi_register_driver(&spifb_driver);
}

static void __exit spifb_exit(void)
{
    printk(KERN_INFO
		"spifb: terminating...\n");

	spi_unregister_driver(&spifb_driver);
}

/* ------------------------------------------------------------------------- */

module_init(spifb_init);
module_exit(spifb_exit);

MODULE_DESCRIPTION("FB driver custom framebuffer");
MODULE_AUTHOR("Moris Ravasio <moris.ravasio@aesys.com>");
MODULE_LICENSE("GPL");