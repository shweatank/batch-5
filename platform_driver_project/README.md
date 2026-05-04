# Linux Platform Driver Example (DT + MMIO + IRQ + Sysfs + Optional GPIO)

This project provides a production-style **platform driver** kernel module in C.

It demonstrates:

- Device Tree matching with `compatible`
- `probe()` / `remove()` lifecycle
- MMIO resource mapping (`platform_get_resource` + `devm_ioremap_resource`)
- Optional interrupt handling (`platform_get_irq_optional` + `devm_request_irq`)
- Optional GPIO control via descriptor API (`gpiod`)
- Sysfs control entry for runtime interaction

## Files

- `driver.c` - main platform driver source
- `Makefile` - out-of-tree kernel module build file
- `device_tree.dts` - sample DT node
- `README.md` - usage guide

## Driver Behavior

On module load / device bind:

1. Logs probe start in `dmesg`
2. Fetches MMIO resource using `platform_get_resource()`
3. Maps MMIO registers with `devm_ioremap_resource()`
4. Optionally acquires GPIO named `enable-gpios`
5. Initializes fake hardware registers
6. Creates sysfs entry:
   - `/sys/bus/platform/devices/<device>/enable`
7. Optionally requests IRQ and handles interrupts

On remove / module unload:

- Disables the fake device
- Removes sysfs entry
- Logs cleanup in `dmesg`

## Build

Install kernel headers first (example for Debian/Ubuntu):

```bash
sudo apt-get update
sudo apt-get install -y build-essential linux-headers-$(uname -r)
```

Build module:

```bash
cd platform_driver_project
make
```

Expected output includes `driver.ko`.

## Device Tree Integration

Use `device_tree.dts` as a reference and merge the node into your board DTS:

```dts
demo_device@10000000 {
	compatible = "demo,my-platform-device";
	reg = <0x10000000 0x1000>;
	interrupts = <0 42 4>;
	enable-gpios = <&gpio0 17 0>;
	status = "okay";
};
```

Notes:

- `reg` must match a valid MMIO region for your hardware
- `interrupts` must match a real IRQ line if IRQ mode is used
- `enable-gpios` is optional

If no IRQ is present in DT, the driver still probes and logs polling mode.

## Load and Test

### 1) Insert module

```bash
sudo insmod driver.ko
```

### 2) Check kernel logs

```bash
dmesg | tail -n 50
```

Look for messages like:

- `probe started`
- `probe completed successfully`
- `IRQ <n> registered` (if IRQ configured)

### 3) Find platform device path

```bash
ls /sys/bus/platform/devices
```

Identify your device (example: `10000000.demo_device`).

### 4) Read/Write sysfs `enable`

```bash
cat /sys/bus/platform/devices/<device>/enable
echo 0 | sudo tee /sys/bus/platform/devices/<device>/enable
echo 1 | sudo tee /sys/bus/platform/devices/<device>/enable
cat /sys/bus/platform/devices/<device>/enable
```

### 5) Verify logs

```bash
dmesg | tail -n 100
```

You should see `sysfs set enable=...` entries.

### 6) Remove module

```bash
sudo rmmod driver
dmesg | tail -n 50
```

Look for `removed`.

## Raspberry Pi Notes

1. Add equivalent node in Raspberry Pi DT overlay with:
   - `compatible = "demo,my-platform-device";`
   - Correct `reg`, `interrupts`, and optional `enable-gpios`
2. Build module on target Pi or cross-compile against exact Pi kernel headers
3. Insert with `insmod` and validate via `dmesg`

## Key Kernel APIs Used

- `platform_driver` and `module_platform_driver()`
- `of_device_id` + `MODULE_DEVICE_TABLE(of, ...)`
- `platform_get_resource()`
- `devm_ioremap_resource()`
- `readl()` / `writel()`
- `platform_get_irq_optional()`
- `devm_request_irq()`
- `devm_gpiod_get_optional()`
- `device_create_file()` / `device_remove_file()`

## Clean

```bash
make clean
```
