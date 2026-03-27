#include <linux/module.h>      // Module macros (init, exit, license)
#include <linux/kernel.h>      // printk(), pr_info(), pr_err()
#include <linux/interrupt.h>   // request_irq(), free_irq()
#include <linux/io.h>          // readl(), writel()
#include <linux/gpio.h>        // GPIO control functions
#include <linux/delay.h>       // msleep(), udelay()
#include <linux/workqueue.h>   // Workqueue APIs
#include <linux/slab.h>        // kmalloc(), kfree()

/* GPIO Definitions */
#define DHT_GPIO_4  (4+512)    // GPIO4 connected to DHT data pin
#define GPIO_17     (17+512)   // GPIO17 connected to push button

/* Timer Definitions */
#define TIMER_PHY   0xFE003000 // Base address of system timer
#define TIMER_CLO   0x04       // Offset of lower 32-bit timer register
#define TIMER_SIZE  0x1C       // Size of timer register block
#define MSEC        1000       // 1000 microseconds = 1 millisecond

/* Structure to store DHT values */
struct dht_data {
    int temperature;   // Temperature value
    int humidity;      // Humidity value
};

static int c;                              // Reading counter
static struct dht_data *data;              // Pointer to sensor data
static int irq;                            // IRQ number
static struct workqueue_struct *my_wq;     // Workqueue pointer
static struct work_struct my_work;         // Work structure
static void __iomem *timer_base;           // Timer base address

/* Microsecond delay using hardware timer */
static void delay_us(unsigned int us)
{
    u32 start, now;

    start = readl(timer_base + TIMER_CLO);  // Read starting time

    while (1) {
        now = readl(timer_base + TIMER_CLO); // Read current time
        if ((now - start) >= us)             // Check if delay completed
            break;
    }
}

/* Workqueue function to read DHT sensor */
static void sensor_work(struct work_struct *work)
{
    int i, j;
    u8 buf[5] = {0};   // Buffer to store 5 bytes from DHT

    /* Send start signal to DHT */
    gpio_direction_output(DHT_GPIO_4, 0);  // Set LOW
    msleep(18);                            // Wait 18 ms
    gpio_set_value(DHT_GPIO_4, 1);         // Set HIGH
    udelay(30);                            // Wait 30 us
    gpio_direction_input(DHT_GPIO_4);      // Set as input

    /* Wait for sensor response */
    if (gpio_get_value(DHT_GPIO_4))
        pr_info("DHT response error\n");

    while (!gpio_get_value(DHT_GPIO_4));   // Wait for HIGH
    while (gpio_get_value(DHT_GPIO_4));    // Wait for LOW

    /* Read 40 bits (5 bytes) */
    for (j = 0; j < 5; j++) {
        for (i = 0; i < 8; i++) {

            while (!gpio_get_value(DHT_GPIO_4)); // Wait for HIGH
            udelay(30);                           // Wait 30 us

            if (gpio_get_value(DHT_GPIO_4))       // If still HIGH → bit = 1
                buf[j] |= (1 << (7 - i));

            while (gpio_get_value(DHT_GPIO_4));  // Wait for LOW
        }
    }

    /* Check checksum */
    if (((buf[0] + buf[1] + buf[2] + buf[3]) & 0xFF) != buf[4]) {
        pr_info("Checksum error\n");
    } else {
        data->humidity = buf[0];      // Store humidity
        data->temperature = buf[2];   // Store temperature

        pr_info("Temp=%d C Humidity=%d%% Count=%d\n",
                data->temperature,
                data->humidity,
                ++c);
    }

    enable_irq(irq);   // Enable interrupt again
}

/* Interrupt handler */
static irqreturn_t GPIO_isr(int irq, void *dev_id)
{
    pr_info("Button interrupt triggered\n");

    disable_irq_nosync(irq);         // Disable IRQ to avoid multiple triggers
    queue_work(my_wq, &my_work);     // Schedule sensor read work

    return IRQ_HANDLED;
}

/* Module Initialization */
static int __init GPIO_init(void)
{
    int ret;

    if (!gpio_is_valid(GPIO_17))
        return -EINVAL;

    ret = gpio_request(GPIO_17, "button");  // Request button GPIO
    if (ret)
        return ret;

    gpio_direction_input(GPIO_17);          // Set button as input
    irq = gpio_to_irq(GPIO_17);             // Convert GPIO to IRQ

    ret = request_irq(irq, GPIO_isr,
                      IRQF_TRIGGER_RISING,
                      "GPIO_ISR", NULL);    // Register interrupt
    if (ret)
        return ret;

    if (!gpio_is_valid(DHT_GPIO_4))
        return -EINVAL;

    ret = gpio_request(DHT_GPIO_4, "dht11_gpio"); // Request DHT GPIO
    if (ret)
        return ret;

    my_wq = create_singlethread_workqueue("dht_wq"); // Create workqueue
    if (!my_wq)
        return -ENOMEM;

    INIT_WORK(&my_work, sensor_work); // Initialize work

    data = kmalloc(sizeof(*data), GFP_KERNEL); // Allocate memory
    if (!data)
        return -ENOMEM;

    gpio_direction_output(DHT_GPIO_4, 1); // Default HIGH

    pr_info("Module loaded\n");
    return 0;
}

/* Module Exit */
static void __exit GPIO_exit(void)
{
    disable_irq(irq);              // Disable interrupt
    free_irq(irq, NULL);           // Free IRQ

    gpio_free(GPIO_17);            // Free button GPIO
    gpio_free(DHT_GPIO_4);         // Free DHT GPIO

    flush_workqueue(my_wq);        // Finish pending work
    destroy_workqueue(my_wq);      // Destroy workqueue

    kfree(data);                   // Free memory

    pr_info("Module exited\n");
}

module_init(GPIO_init);
module_exit(GPIO_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("team2");
MODULE_DESCRIPTION("GPIO + DHT11 + Workqueue Driver");
