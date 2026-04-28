#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/of.h>
#include <linux/delay.h>
#include <linux/workqueue.h>

/* DS3231 Registers */
#define DS3231_SEC   0x00
#define DS3231_MIN   0x01
#define DS3231_HOUR  0x02
#define DS3231_DAY   0x03
#define DS3231_DATE  0x04
#define DS3231_MON   0x05
#define DS3231_YEAR  0x06

static const char* days[] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
static struct i2c_client *rtc_client;  // global reference

/* BCD to Decimal */
static int bcd_to_dec(u8 val)
{
    return ((val >> 4) * 10) + (val & 0x0F);
}

/* Read one register */
static int ds3231_read_reg(struct i2c_client *client, u8 reg)
{
    return i2c_smbus_read_byte_data(client, reg);
}

static u8 dec_to_bcd(int val)
{
    return ((val / 10) << 4) | (val % 10);
}

static int ds3231_write_reg(struct i2c_client *client, u8 reg, u8 val)
{
    u8 buf[2];
    int ret;

    buf[0] = reg;
    buf[1] = val;

    ret = i2c_master_send(client, buf, 2);
    if (ret < 0) {
        dev_err(&client->dev, "DS3231 write failed (reg=0x%02x)\n", reg);
        return -EIO;
    }
    return 0;
}

/* Read full time and print */
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
    mon  = bcd_to_dec(mon);
    year = bcd_to_dec(year);

    dev_info(&client->dev,
        "Time: %02d:%02d:%02d  Date: %02d/%02d/%02d  Day:%s\n",
        hour, min, sec, date, mon, year, days[day-1]);
}

/* Workqueue to print time every second */
static struct delayed_work rtc_work;

static void rtc_work_func(struct work_struct *work)
{
    if (rtc_client)
        ds3231_read_time(rtc_client);

    /* Reschedule work after 1 second */
    schedule_delayed_work(&rtc_work, msecs_to_jiffies(1000));
}

/* Probe function */
static int ds3231_probe(struct i2c_client *client)
{
    dev_info(&client->dev, "DS3231 RTC detected\n");
    rtc_client = client;  // save reference

    /* Optional: set initial time */
    ds3231_write_reg(client, DS3231_SEC, dec_to_bcd(0));
    ds3231_write_reg(client, DS3231_MIN, dec_to_bcd(0));
    ds3231_write_reg(client, DS3231_HOUR, dec_to_bcd(12));

    /* Start periodic read */
    INIT_DELAYED_WORK(&rtc_work, rtc_work_func);
    schedule_delayed_work(&rtc_work, msecs_to_jiffies(1000));

    return 0;
}

/* Remove function */
static void ds3231_remove(struct i2c_client *client)
{
    cancel_delayed_work_sync(&rtc_work);
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
MODULE_AUTHOR("Sai Kumar");
MODULE_DESCRIPTION("DS3231 RTC I2C Driver - Prints time every second");

