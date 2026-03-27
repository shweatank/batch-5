# GPIO Interrupt Button Driver - Documentation

This project provides a Linux kernel module (LKM) that turns a GPIO button into a Linux interrupt source using:

- Device Tree matching (`of_match_table`)
- `gpio_to_irq()` GPIO-to-IRQ mapping
- `request_irq()` ISR registration
- `printk`/`pr_info` logging and a press counter

It is intended for learning and beginner-friendly experimentation on Raspberry Pi or other Device-Tree capable Linux boards.

---

## 1) High-level Concept

A physical push button connects a GPIO input pin to a defined electrical level (commonly GND). When the pin transitions (edge), the interrupt controller routes that event to the kernel IRQ line. The kernel then calls your ISR, which:

1. Optionally applies a small debounce window
2. Increments a press counter
3. Prints a log message

---

## 2) What the Driver Implements

### Kernel module behavior

When the module is loaded, it registers a platform driver. On matching a Device Tree node, the driver:

1. Reads the GPIO pin from Device Tree property: `button-gpios`
2. Requests the GPIO and sets it to input
3. Converts GPIO -> IRQ using `gpio_to_irq()`
4. Registers an interrupt handler using `request_irq()`
5. Exposes `press_count` via sysfs (optional interface)

### Sysfs interface

The driver creates a read-only sysfs attribute:

- `press_count`: total number of accepted button interrupts

Path example (platform dependent):

- `/sys/bus/platform/devices/button-irq@0/press_count`

---

## 3) Device Tree Binding

The driver matches Device Tree nodes with:

- `compatible = "example,gpio-button-irq"`

### Required property

- `button-gpios`: GPIO definition for the input pin

### Optional properties

- `irq-both-edges` (boolean): enable IRQ on both rising and falling edges
- `debounce-ms` (u32): software debounce time window in milliseconds (default: 30)

---

## 4) IRQ Flags (request_irq) - Beginner Notes

The driver uses these trigger flags to tell the kernel what edge to listen for:

- `IRQF_TRIGGER_FALLING`
  - triggers on high -> low transition
  - typical when wiring uses a pull-up resistor and button pulls GPIO to GND
- `IRQF_TRIGGER_RISING`
  - triggers on low -> high transition
- `IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING`
  - triggers on both transitions (useful if you want count on both press/release)

In this project:

- Default is falling edge (`IRQF_TRIGGER_FALLING`)
- If `irq-both-edges;` is present in DT, both-edge triggering is enabled

---

## 5) Optional Debounce Logic (Bonus)

Mechanical buttons bounce (one press can generate multiple fast edges). This driver adds a simple software debounce:

- Each IRQ handler checks the time since the last accepted interrupt
- If a new interrupt arrives within `debounce-ms`, it is ignored
- Default `debounce-ms` is 30 ms

Device Tree:

```dts
debounce-ms = <30>;
```

---

## 6) Project Files

- `gpio_button_irq.c`
  - platform driver
  - ISR (`gpio_button_isr`)
  - sysfs `press_count`
- `Makefile`
  - out-of-tree module build rules
- `dts/button_irq_overlay_rpi.dts`
  - Raspberry Pi overlay example
- `dts/button_irq_generic.dtsi`
  - generic Device Tree snippet

---

## 7) Build and Install (Linux)

From the project directory (`gpio-button-irq`):

```bash
make
sudo insmod gpio_button_irq.ko
```

To remove:

```bash
sudo rmmod gpio_button_irq
```

### Notes

- If your system restricts kernel logs, `dmesg` may require root.
- This module is compiled as an out-of-tree kernel module; you must have matching kernel headers installed.

---

## 8) Testing the Interrupt

1. Load the module
2. Ensure your Device Tree overlay (or board DTS node) is active
3. Wire the button:
   - push button between GPIO pin and GND (recommended to use a pull-up)
4. Trigger interrupts by pressing the button physically
5. Observe:
   - kernel log output (from ISR)
   - increasing `press_count`

Recommended log check:

```bash
sudo dmesg -w
```

Check counter:

```bash
cat /sys/bus/platform/devices/button-irq@0/press_count
```

---

## 9) Debugging Guide (When Interrupt Not Triggering)

Follow this order; it saves time:

1. Confirm the driver is loaded
   - `lsmod | rg gpio_button_irq`
2. Confirm probe succeeded (look for GPIO and IRQ numbers)
   - `dmesg | rg "gpio_button_irq|probe|gpio_to_irq|request_irq"`
3. Confirm Device Tree binding worked
   - verify your node exists under `/proc/device-tree/` (platform dependent)
4. Verify the GPIO number from DT
   - ensure `button-gpios` points to the correct controller + pin
5. Verify IRQ mapping
   - probe log should show computed `GPIO=<n> IRQ=<m>`
6. Check `/proc/interrupts`
   - you should see IRQ activity when pressing the button
7. Electrical checks
   - correct pull-up/pull-down
   - no floating pin
   - correct active level (falling vs rising)

Common mistakes:

- Wrong `compatible` string -> driver won’t bind
- Wrong GPIO pin -> interrupts won’t fire
- Wrong trigger edge -> you may only see interrupts on press or only on release
- Overlay not applied / not loaded -> DT node won’t exist

---

## 10) Flowchart (Button -> IRQ -> ISR)

```mermaid
flowchart LR
    A[Button Press] --> B[GPIO Pin Edge Detected]
    B --> C[gpio_to_irq() Maps GPIO to IRQ]
    C --> D[Kernel IRQ Dispatch]
    D --> E[ISR: gpio_button_isr()]
    E --> F[Debounce Check (optional)]
    F --> G[printk/pr_info Log]
    F --> H[Increment press_count]
```

---

## 11) Optional Enhancements (Future Work)

If you want to improve learning value:

- Use the modern gpiod API (`gpiod_get()` / `gpiod_to_irq()`) instead of legacy `gpio_request()`
- Add a character device or ioctl interface for user-space communication
- Add debouncing via workqueues/timers instead of timing inside ISR

