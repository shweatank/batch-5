#include <linux/module.h>          // Needed for all kernel modules
#include <linux/kernel.h>          // Provides kernel functions like printk
#include <linux/gpio/consumer.h>   // For working with GPIO pins
#include <linux/interrupt.h>       // For handling hardware interrupts
#include <linux/proc_fs.h>         // To create files in /proc
#include <linux/platform_device.h> // To write platform drivers
#include <linux/of.h>              // To read device tree info
#include <linux/uaccess.h>         // To safely move data between kernel and user
#include <linux/ktime.h>           // For high-precision timestamps
#include <linux/delay.h>           // For delay functions like msleep
#include <linux/spi/spi.h>         // For SPI communication
#include "font8x8_basic.h"         // 8x8 font for drawing characters on LCD
#include <linux/cdev.h>            // For creating character devices

#define PROC_NAME "hcsr04_direction_led" // Name of proc file
#define DRIVER_NAME "ili9225"            // Name of LCD driver
#define CLASS_NAME  "ili"                // Device class name
#define DEVICE_NAME "ili9225_char"       // Name of character device
#define SCREEN_WIDTH 176                 // LCD width in pixels
#define SCREEN_HEIGHT 220                // LCD height in pixels

static dev_t dev_num;                // Device number for character device
static struct class *ili_class;     // Class pointer for the device
static struct cdev ili_cdev;        // Character device structure
static struct ili9225 *g_lcd;       // Global pointer to the LCD device

static int cursor_x = 0;            // X position of cursor for text
static int cursor_y = 0;            // Y position of cursor for text

// Structure for LCD pins and SPI
struct ili9225 {
    struct spi_device *spi;         // Pointer to SPI device
    struct gpio_desc *dc;           // Data/Command pin
    struct gpio_desc *reset;        // Reset pin
};

// GPIOs for ultrasonic sensor and LEDs
static struct gpio_desc *trig;      // Trigger pin
static struct gpio_desc *echo;      // Echo pin
static struct gpio_desc *led_near;  // LED for object coming closer
static struct gpio_desc *led_far;   // LED for object moving away

static int irq_number;              // IRQ number for echo pin
static ktime_t echo_start, echo_end; // Start and end times of ultrasonic pulse
static unsigned long distance_cm = 0; // Measured distance in cm

// Variables for calculating speed
static long prev_distance_cm = 0;   // Previous measured distance
static ktime_t prev_time;           // Previous measurement timestamp
static long speed_cm_per_sec = 0;   // Speed of object in cm/s

static struct proc_dir_entry *proc_file; // Pointer to proc file

// ------------------- LCD SPI FUNCTIONS -------------------

// Send 16-bit data via SPI
static int ili9225_write16(struct ili9225 *lcd, u16 value)
{
    u8 buf[2];
    buf[0] = value >> 8;     // Take the higher 8 bits
    buf[1] = value & 0xFF;   // Take the lower 8 bits
    return spi_write(lcd->spi, buf, 2); // Send both bytes to LCD
}

// Write value to a specific LCD register
static int ili9225_write_reg(struct ili9225 *lcd, u16 reg, u16 data)
{
    gpiod_set_value(lcd->dc, 0); // Set to command mode
    ili9225_write16(lcd, reg);   // Send register address
    gpiod_set_value(lcd->dc, 1); // Set to data mode
    return ili9225_write16(lcd, data); // Send the data to that register
}

// Reset the LCD screen
static void ili9225_reset(struct ili9225 *lcd)
{
    gpiod_set_value(lcd->reset, 1); // Pull reset high
    msleep(5);                        // Wait 5ms
    gpiod_set_value(lcd->reset, 0); // Pull reset low
    msleep(20);                       // Wait 20ms
    gpiod_set_value(lcd->reset, 1); // Pull reset high again
    msleep(50);                       // Wait 50ms to stabilize
}

// Initialize the LCD with correct settings
static void ili9225_init(struct ili9225 *lcd)
{
    ili9225_reset(lcd);              // Reset LCD first

    ili9225_write_reg(lcd, 0x0001, 0x011C);  // Set driver output direction
    ili9225_write_reg(lcd, 0x0002, 0x0100);  // Line inversion control
    ili9225_write_reg(lcd, 0x0003, 0x1030);  // GRAM entry mode
    ili9225_write_reg(lcd, 0x0008, 0x0808);  // Control frame frequency
    ili9225_write_reg(lcd, 0x000C, 0x0000);  // Display control
    ili9225_write_reg(lcd, 0x000F, 0x0B01);  // Power up LCD driver
    ili9225_write_reg(lcd, 0x0010, 0x0A00);  // Set internal voltage
    ili9225_write_reg(lcd, 0x0011, 0x1038);  // Set stable driving voltage
    msleep(50);                               // Small delay
    ili9225_write_reg(lcd, 0x0012, 0x1121);  // Driver output voltages
    ili9225_write_reg(lcd, 0x0013, 0x0063);  // Gate voltages
    ili9225_write_reg(lcd, 0x0014, 0x5A00);  // Brightness uniformity
    msleep(50);                               // Wait before turning on display
    ili9225_write_reg(lcd, 0x0007, 0x1017);  // Display ON
    msleep(20);                               // Wait for display to stabilize
}

// Fill entire screen with a color
static void ili9225_fill(struct ili9225 *lcd, u16 color)
{
    int x, y;

    // Set window size for entire screen
    ili9225_write_reg(lcd, 0x0036, SCREEN_WIDTH-1);  // horizontal end
    ili9225_write_reg(lcd, 0x0037, 0);              // horizontal start
    ili9225_write_reg(lcd, 0x0038, SCREEN_HEIGHT-1);// vertical end
    ili9225_write_reg(lcd, 0x0039, 0);              // vertical start
    ili9225_write_reg(lcd, 0x0020, 0);             // set X cursor
    ili9225_write_reg(lcd, 0x0021, 0);             // set Y cursor

    gpiod_set_value(lcd->dc, 0);     // Command mode
    ili9225_write16(lcd, 0x0022);    // Start writing pixels
    gpiod_set_value(lcd->dc, 1);     // Data mode

    // Loop through every pixel and set color
    for (y = 0; y < SCREEN_HEIGHT; y++)
        for (x = 0; x < SCREEN_WIDTH; x++)
            ili9225_write16(lcd, color); // Fill each pixel
}

// Draw a single pixel at X,Y
static void drawPixel(int x, int y, uint16_t color)
{
    struct ili9225 *lcd = g_lcd;

    // Set X position
    gpiod_set_value(lcd->dc, 0);
    ili9225_write16(lcd, 0x0020);
    gpiod_set_value(lcd->dc, 1);
    ili9225_write16(lcd, x);

    // Set Y position
    gpiod_set_value(lcd->dc, 0);
    ili9225_write16(lcd, 0x0021);
    gpiod_set_value(lcd->dc, 1);
    ili9225_write16(lcd, y);

    // Write the pixel color
    gpiod_set_value(lcd->dc, 0);
    ili9225_write16(lcd, 0x0022);
    gpiod_set_value(lcd->dc, 1);
    ili9225_write16(lcd, color);
}

// Draw a character using 8x8 font
static void drawChar(int x, int y, char c, uint16_t color)
{
    if(c < 32 || c > 127) return; // Ignore non-printable chars
    const uint8_t *bitmap = font8x8[c - 32]; // Get font bitmap

    // Loop through each pixel in character
    for(int row = 0; row < 8; row++)
        for(int col = 0; col < 8; col++)
            if(bitmap[row] & (1 << (7 - col))) {
                // Draw 2x2 pixels for bigger size
                drawPixel(x + col*2, y + row*2, color);
                drawPixel(x + col*2+1, y + row*2, color);
                drawPixel(x + col*2, y + row*2+1, color);
                drawPixel(x + col*2+1, y + row*2+1, color);
            }
}

// Draw a string starting at X,Y
static void drawString(int *x, int *y, const char *str, uint16_t color)
{
    int start_x = *x;
    for(int i=0; str[i]; i++) {
        if(str[i] == '\n') {       // Newline handling
            *x = start_x;           // Reset X
            *y += 16;               // Move to next line
            if(*y > SCREEN_HEIGHT-16) *y = 0; // Wrap vertically
            continue;
        }
        drawChar(*x, *y, str[i], color); // Draw each char
        *x += 16;                         // Move cursor right

        if(*x > SCREEN_WIDTH - 16) {      // Wrap line if needed
            *x = start_x;
            *y += 16;
            if(*y > SCREEN_HEIGHT-16) *y = 0;
        }
    }
}

// ------------------- Ultrasonic Sensor Interrupt -------------------

// Interrupt handler for the Echo pin
static irqreturn_t echo_irq_handler(int irq, void *dev_id)
{
    if (gpiod_get_value(echo))       // If echo pin goes HIGH (pulse started)
        echo_start = ktime_get();    // Record start time
    else {                           // If echo pin goes LOW (pulse ended)
        echo_end = ktime_get();      // Record end time
        s64 duration_us = ktime_to_us(ktime_sub(echo_end, echo_start)); // Duration in microseconds
        if (duration_us > 0)
            distance_cm = duration_us / 58; // Convert to cm (HC-SR04 formula)
    }

    return IRQ_HANDLED; // Tell kernel we handled the interrupt
}

// ------------------- /proc File Read -------------------

// Function called when user reads from /proc/hcsr04_direction_led
static ssize_t proc_read(struct file *file, char __user *buf,
                         size_t count, loff_t *pos)
{
    char output[128];           // Buffer for user output
    int len, i;
    ktime_t current_time;       // Current timestamp
    s64 time_diff_ms;           // Time since last measurement
    long distance_diff;         // Difference between last and current distance
    long display_speed = 0;     // Speed in absolute value
    unsigned int blink_delay = 150; // LED blink delay in ms

    if (*pos > 0)                // If user already read, return 0 (EOF)
        return 0;

    // ---------------- Trigger ultrasonic sensor ----------------
    gpiod_set_value(trig, 0);   // Ensure trigger LOW
    udelay(2);                   // Wait 2 microseconds
    gpiod_set_value(trig, 1);   // Send HIGH pulse
    udelay(10);                  // Pulse duration 10 microseconds
    gpiod_set_value(trig, 0);   // Back to LOW

    msleep(60);                  // Wait for echo to return

    // ---------------- Speed calculation ----------------
    current_time = ktime_get();                       // Get current time
    time_diff_ms = ktime_to_ms(ktime_sub(current_time, prev_time)); // Time diff in ms

    if (time_diff_ms > 0) {
        distance_diff = distance_cm - prev_distance_cm;  // Change in distance
        speed_cm_per_sec = (distance_diff * 1000) / time_diff_ms; // Speed cm/s
    }

    prev_distance_cm = distance_cm; // Update previous distance
    prev_time = current_time;       // Update previous timestamp

    // Absolute speed for display
    display_speed = (speed_cm_per_sec < 0) ? -speed_cm_per_sec : speed_cm_per_sec;

    // ---------------- Turn off both LEDs first ----------------
    gpiod_set_value(led_near, 0);
    gpiod_set_value(led_far, 0);

    // ---------------- LED blink based on direction ----------------
    if (speed_cm_per_sec < 0) {        // Object moving closer
        for (i = 0; i < 5; i++) {     // Blink near LED 5 times
            gpiod_set_value(led_near, 1);
            msleep(blink_delay);
            gpiod_set_value(led_near, 0);
            msleep(blink_delay);
        }
    }
    else if (speed_cm_per_sec > 0) {   // Object moving away
        for (i = 0; i < 5; i++) {     // Blink far LED 5 times
            gpiod_set_value(led_far, 1);
            msleep(blink_delay);
            gpiod_set_value(led_far, 0);
            msleep(blink_delay);
        }
    }

    // ---------------- Prepare output for user ----------------
    len = sprintf(output,
                  "Distance: \n\n%lu cm\n\nSpeed: \n\n%ld cm/s\n",
                  distance_cm,
                  display_speed);

    // ---------------- Display on LCD ----------------
    ili9225_fill(g_lcd, 0xFFFF);  // Clear screen with white
    cursor_x = 0;                  // Reset cursor
    cursor_y = 20;

    drawString(&cursor_x, &cursor_y, output, 0x0000); // Draw text in black

    // ---------------- Copy to user ----------------
    if (copy_to_user(buf, output, len))
        return -EFAULT;           // Return error if copy fails

    *pos = len;                   // Update file position
    return len;                   // Return number of bytes read
}

// ------------------- LCD SPI Driver -------------------
static int ili9225_probe(struct spi_device *spi)
{
    struct ili9225 *lcd;

    lcd = devm_kzalloc(&spi->dev, sizeof(*lcd), GFP_KERNEL); // Allocate memory
    if (!lcd) return -ENOMEM;      // Fail if allocation fails

    lcd->spi = spi;                 // Save SPI device pointer
    spi_set_drvdata(spi, lcd);      // Attach our structure to SPI device

    // Get GPIO pins from device tree
    lcd->dc = devm_gpiod_get(&spi->dev, "dc", GPIOD_OUT_LOW);
    if (IS_ERR(lcd->dc)) return PTR_ERR(lcd->dc);

    lcd->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH);
    if (IS_ERR(lcd->reset)) return PTR_ERR(lcd->reset);

    spi->mode = SPI_MODE_0;         // Set SPI mode
    spi->bits_per_word = 8;         // 8 bits per transfer
    spi_setup(spi);                 // Apply SPI settings

    ili9225_init(lcd);              // Initialize LCD
    ili9225_fill(lcd, 0xFFFF);      // Clear screen

    g_lcd = lcd;                    // Save global pointer
    pr_info("ILI9225 initialized\n");

    return 0;
}

// Remove LCD driver
static void ili9225_remove(struct spi_device *spi)
{
    device_destroy(ili_class, dev_num); // Destroy character device
    class_destroy(ili_class);           // Destroy class
    cdev_del(&ili_cdev);                // Delete cdev
    unregister_chrdev_region(dev_num, 1); // Free device numbers

    pr_info("ILI9225 removed\n");
}

// ------------------- HC-SR04 Platform Driver -------------------
// Match with device tree
static const struct of_device_id hcsr04_of_match[] = {
    { .compatible = "mycompany,hcsr04", },
    {},
};
MODULE_DEVICE_TABLE(of, hcsr04_of_match);

// Probe function called when device is found
static int hcsr04_probe(struct platform_device *pdev)
{
    int ret;

    // Get GPIOs from device tree
    trig = devm_gpiod_get(&pdev->dev, "trig", GPIOD_OUT_LOW);
    echo = devm_gpiod_get(&pdev->dev, "echo", GPIOD_IN);
    led_near = devm_gpiod_get(&pdev->dev, "led-near", GPIOD_OUT_LOW);
    led_far  = devm_gpiod_get(&pdev->dev, "led-far", GPIOD_OUT_LOW);

    if (IS_ERR(trig) || IS_ERR(echo) ||
        IS_ERR(led_near) || IS_ERR(led_far)) {
        dev_err(&pdev->dev, "Failed to get GPIOs\n");
        return -ENODEV; // Fail if GPIOs not available
    }

    // Get IRQ number for echo pin
    irq_number = gpiod_to_irq(echo);
    if (irq_number < 0)
        return irq_number;

    // Request IRQ for echo pin (trigger on rising and falling edge)
    ret = request_irq(irq_number,
                      echo_irq_handler,
                      IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                      "hcsr04_irq",
                      NULL);
    if (ret) return ret;

    // Create proc file for reading distance/speed
    proc_file = proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    if (!proc_file) {
        free_irq(irq_number, NULL);
        return -ENOMEM;
    }

    prev_time = ktime_get(); // Initialize previous timestamp

    dev_info(&pdev->dev, "HC-SR04 Direction Detection Driver Loaded\n");
    return 0;
}

// Remove function for HC-SR04
static void hcsr04_remove(struct platform_device *pdev)
{
    proc_remove(proc_file);    // Remove proc entry
    free_irq(irq_number, NULL); // Free the IRQ

    gpiod_set_value(led_near, 0); // Turn off LEDs
    gpiod_set_value(led_far, 0);
}

// ------------------- Driver Registration -------------------
static struct platform_driver hcsr04_driver = {
    .probe  = hcsr04_probe,
    .remove = hcsr04_remove,
    .driver = {
        .name = "hcsr04_direction_led",
        .of_match_table = hcsr04_of_match,
    },
};

// ------------------- Module init/exit -------------------
static int __init my_driver_init(void)
{
    int ret;

    ret = spi_register_driver(&ili9225_driver); // Register SPI LCD driver
    if (ret) return ret;

    ret = platform_driver_register(&hcsr04_driver); // Register HC-SR04 platform driver
    if (ret) {
        spi_unregister_driver(&ili9225_driver); // Cleanup if fails
        return ret;
    }

    return 0;
}

static void __exit my_driver_exit(void)
{
    platform_driver_unregister(&hcsr04_driver); // Unregister platform driver
    spi_unregister_driver(&ili9225_driver);     // Unregister SPI driver
}

// Declare module entry and exit points
module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("team 4");
MODULE_DESCRIPTION("HC-SR04 Direction Detection with Two LEDs (Positive Speed Output)");
