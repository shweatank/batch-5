#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/of.h>
#include <linux/delay.h>

/* DS3231 Registers */
#define DS3231_SEC   0x00
#define DS3231_MIN   0x01
#define DS3231_HOUR  0x02
#define DS3231_DAY   0x03
#define DS3231_DATE  0x04
#define DS3231_MON   0x05
#define DS3231_YEAR  0x06


/* BCD to Decimal */
static int bcd_to_dec(u8 val)
{
    return ((val >> 4) * 10) + (val & 0x0F);
}


/* Read one register */
static int ds3231_read_reg(struct i2c_client *client, u8 reg)
{
    return i2c_smbus_read_byte_data(client, reg);
/* Write one byte to DS3231 register */
}


/* Read full time */
static void ds3231_read_time(struct i2c_client *client)
{
    int sec, min, hour, day, date, mon, year;
    sec  = ds3231_read_reg(client, DS3231_SEC);
    min  = ds3231_read_reg(client, DS3231_MIN);
    hour = ds3231_read_reg(client, DS3231_HOUR);
    day  = ds3231_read_reg(client, DS3231_DAY);
    date = ds3231_read_reg(client, DS3231_DATE);
    mon  = ds3231_read_reg(client, DS3231_MON);
    year = ds3231_read_reg(client, DS3231_YEAR);

    if (sec < 0 || min < 0 || hour < 0) {
        dev_err(&client->dev, "RTC Read Failed\n");
        return;
    }

    sec  = bcd_to_dec(sec);
    min  = bcd_to_dec(min);
    hour = bcd_to_dec(hour);
    day  = bcd_to_dec(day);
    date = bcd_to_dec(date);
    mon  = bcd_to_dec(mon & 0x1F);
    year = bcd_to_dec(year) + 2000;

    dev_info(&client->dev,
        "Time: %02d:%02d:%02d  Date: %02d/%02d/%d  Day:%d\n",
        hour, min, sec, date, mon, year, day);
}
/* Probe */
static int ds3231_probe(struct i2c_client *client)
{
    dev_info(&client->dev, "DS3231 RTC detected\n");
/* ds3231_set_time(client,
                14, 30, 0,   // HH:MM:SS
                13, 2, 26);  // DD/MM/YY
*/
    /* Read time once */
    ds3231_read_time(client);

    return 0;
}


/* Remove */
static void ds3231_remove(struct i2c_client *client)
{
    dev_info(&client->dev, "DS3231 RTC removed\n");
}
/* Device Tree Match */
static const struct of_device_id ds3231_dt_ids[] = {
    { .compatible = "maxim,ds3231" },
    { }
};

MODULE_DEVICE_TABLE(of, ds3231_dt_ids);
/* I2C Driver */
static struct i2c_driver ds3231_driver = {
    .driver = {
        .name = "ds3231_driver",
        .of_match_table = ds3231_dt_ids,
    },
    .probe = ds3231_probe,
    .remove = ds3231_remove,
};

module_i2c_driver(ds3231_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ramesh");
MODULE_DESCRIPTION("Simple DS3231 RTC I2C Driver");
