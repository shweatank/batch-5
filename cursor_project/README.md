# Linux Platform Driver Demo

This project contains a Linux kernel **platform driver** module:

- DT match: `compatible = "demo,my-platform-device"`
- MMIO path: `platform_get_resource()` + `devm_ioremap_resource()`
- Optional IRQ handling
- Optional GPIO via `gpiod`
- Sysfs control: `enable`
- **x86/desktop self-test mode** (no DT required): `selftest=1`

## Files

- `driver.c` - main platform driver
- `Makefile` - out-of-tree module build rules
- `device_tree.dts` - standalone sample DTS
- `my-platform-device.dts` - alternate sample DTS

## Build

```bash
make clean
make
```

This produces `driver.ko`.

## Test on Ubuntu/x86 (No Device Tree Overlay)

Use self-test mode to force creation of a software platform device.

### 1) Load module in self-test mode

```bash
sudo rmmod driver 2>/dev/null || true
sudo insmod driver.ko selftest=1
```

### 2) Check logs

```bash
sudo dmesg | tail -n 80
```

Look for:

- `selftest platform device registered`
- `probe started`
- `selftest mode: using shadow registers`
- `probe completed successfully`

### 3) Find sysfs node

```bash
ls /sys/bus/platform/devices | rg demo_platform_device
```

### 4) Read and toggle `enable`

```bash
DEV=$(ls /sys/bus/platform/devices | rg demo_platform_device | head -n 1)
cat /sys/bus/platform/devices/$DEV/enable
echo 0 | sudo tee /sys/bus/platform/devices/$DEV/enable
echo 1 | sudo tee /sys/bus/platform/devices/$DEV/enable
cat /sys/bus/platform/devices/$DEV/enable
```

### 5) Verify logs again

```bash
sudo dmesg | tail -n 80
```

### 6) Unload

```bash
sudo rmmod driver
```

## Test on Real DT Hardware (Raspberry Pi / ARM SoC)

1. Add a board-specific DT node or overlay with:
   - `compatible = "demo,my-platform-device"`
   - valid `reg`
   - optional `interrupts`
   - optional `enable-gpios`
2. Boot with that DT/overlay
3. Load module normally (without selftest):

```bash
sudo insmod driver.ko
```

Then verify probe logs in `dmesg`.

## Notes

- On non-DT desktop systems, DT overlays are usually not used.
- `loading out-of-tree module taints kernel` is expected for external modules.
- `module verification failed` indicates unsigned module (common with Secure Boot).
