#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/init.h>

#define DRIVER_NAME "i2c_simple"


static int i2c_simple_probe(struct i2c_client *client)
{
	pr_info("i2c_simple: Device probed at address 0x%x\n", client->addr);
	return 0;
}


static void i2c_simple_remove(struct i2c_client *client)
{
	pr_info("i2c_simple: Device removed\n");
}

static const struct i2c_device_id i2c_simple_id[] = {
	{ "i2c_simple", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, i2c_simple_id);

static struct i2c_driver i2c_simple_driver = {
	.driver = {
		.name = DRIVER_NAME,
	},
	.probe    = i2c_simple_probe,  
	.remove   = i2c_simple_remove,
	.id_table = i2c_simple_id,
};

module_i2c_driver(i2c_simple_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("SIMPLE_I2C_DRIVER");
