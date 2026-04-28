#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>

#define DRIVER_NAME "spi_sd_rpi4"

/* CMD0 - GO_IDLE_STATE */
static u8 cmd0[] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};

/* OF match table for Device Tree */
static const struct of_device_id spi_sd_dt_ids[] = {
    { .compatible = "spi-mmc", },
    { }
};
MODULE_DEVICE_TABLE(of, spi_sd_dt_ids);

/* SPI device ID table */
static const struct spi_device_id spi_sd_ids[] = {
    { "spi-mmc", 0 },
    { }
};
MODULE_DEVICE_TABLE(spi, spi_sd_ids);

static int spi_sd_probe(struct spi_device *spi)
{
    int ret;
    u8 response[6] = {0};

    dev_info(&spi->dev, "SPI SD card probe\n");

    /* Send CMD0 */
    ret = spi_write_then_read(spi, cmd0, sizeof(cmd0), response, sizeof(response));
    if (ret) {
        dev_err(&spi->dev, "SPI write/read failed: %d\n", ret);
        return ret;
    }

    dev_info(&spi->dev, "CMD0 response: %*ph\n", (int)sizeof(response), response);

    if (response[0] == 0x01)
        dev_info(&spi->dev, "✅ SD card responded to CMD0\n");
    else
        dev_warn(&spi->dev, "❌ SD card did NOT respond properly\n");

    return 0;
}

static void spi_sd_remove(struct spi_device *spi)
{
    dev_info(&spi->dev, "SPI SD card removed\n");
//    return 0;
}

static struct spi_driver spi_sd_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = spi_sd_dt_ids,
    },
    .id_table = spi_sd_ids,
    .probe = spi_sd_probe,
    .remove = spi_sd_remove,
};

module_spi_driver(spi_sd_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("SPI SD card driver for Raspberry Pi 4 CE1");

