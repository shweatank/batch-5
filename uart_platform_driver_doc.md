# UART MMIO Platform Driver – Full Documentation

## 1. Introduction

The **UART MMIO Platform Driver** is a Linux kernel module that provides a character-device interface to a memory-mapped UART controller on an SoC (for example, Raspberry Pi–class hardware). Instead of accessing UART registers directly from user space, applications interact via `/dev/uart_mmio0` for transmit (TX) and receive (RX).

- **Driver type**: `platform_driver` (Device Tree based)
- **User interface**: character device (`/dev/uart_mmio0`)
- **Hardware model**: simple PL011-like UART with:
  - Data register
  - Flag register (TX FIFO full, RX FIFO empty)
  - Control register (enable, TX enable, RX enable)
- **Mode of operation**: interrupt-driven, blocking `read()` / `write()` with wait queues (no busy polling).

## 2. System Requirements and Assumptions

- **Operating System**: Linux with:
  - Device Tree (OF) support
  - Platform bus enabled
  - Loadable module support
- **Hardware**:
  - UART controller exposed as a memory-mapped peripheral
  - Interrupt line for UART RX/TX events
- **Device Tree node** (example):

```dts
uart0: myuart@3f201000 {
    compatible = "myvendor,mmio-uart";
    reg = <0x3f201000 0x1000>;
    interrupts = <25>;
    clock-frequency = <48000000>;
    status = "okay";
};
```

- **Software constraints**:
  - No busy-waiting in the kernel (use interrupts + wait queues).
  - No direct user-space MMIO; only via `/dev/uart_mmio0`.
  - Clean probe/remove paths; all resources freed on failure or unload.

## 3. Driver Architecture

### 3.1 Core Components

- **Platform driver** (`struct platform_driver`):
  - `probe()` – initialize hardware and software resources.
  - `remove()` – disable UART and free resources.

- **Private device structure** (`struct uart_mmio_dev` – per UART instance):
  - `void __iomem *base;` – mapped UART register base.
  - `int irq;` – UART interrupt number.
  - `struct device *dev;` – back-reference to platform device.
  - `dev_t devt;` – major/minor for character device.
  - `struct cdev cdev;` – kernel char device object.
  - `struct class *class;` – sysfs class for `/dev` node.
  - `struct mutex lock;` – protects register access and shared state.
  - `wait_queue_head_t rx_wq;` – wait queue for blocking `read()`.
  - `wait_queue_head_t tx_wq;` – wait queue for blocking `write()`.
  - `bool rx_ready;` – RX data available flag.
  - `bool tx_space;` – TX FIFO has space flag.

- **File operations** (`struct file_operations`):
  - `open()` – attach `struct uart_mmio_dev` to `file->private_data`.
  - `read()` – receive bytes from UART (blocking, interruptible).
  - `write()` – send bytes to UART (blocking, interruptible).
  - `llseek = no_llseek` – random access not supported.

- **Interrupt handler** (`uart_mmio_irq()`):
  - Reads UART flag/status.
  - Wakes sleeping readers/writers via wait queues when RX data is available or TX FIFO has space.

### 3.2 Logical Register Interface

For documentation, the UART provides at least:

- `UART_DR` – Data Register
- `UART_FR` – Flag Register:
  - `TXFF` bit – TX FIFO Full
  - `RXFE` bit – RX FIFO Empty
- `UART_CR` – Control Register:
  - `UARTEN` bit – UART enable
  - `TXE` bit – TX enable
  - `RXE` bit – RX enable

Exact bit positions depend on the UART IP, but the driver logic remains the same.

## 4. Initialization Flow (Probe) – Workflow / Flowchart

**Goal**: Bind the driver to the UART hardware, enable it, and create `/dev/uart_mmio0`.

### 4.1 Step-by-Step Probe Flow

1. **Device Tree enumeration**
   - Kernel parses DT node with `compatible = "myvendor,mmio-uart"`.
   - A `platform_device` is created and registered on the platform bus.

2. **Driver match and probe**
   - `uart_mmio_driver` has `.of_match_table` with `"myvendor,mmio-uart"`.
   - Kernel calls `uart_mmio_probe(struct platform_device *pdev)`.

3. **Allocate driver context**
   - `devm_kzalloc()` allocates and zero-initializes `struct uart_mmio_dev`.
   - Pointers and flags are initialized to safe defaults.

4. **Initialize synchronization primitives**
   - `mutex_init(&ud->lock);`
   - `init_waitqueue_head(&ud->rx_wq);`
   - `init_waitqueue_head(&ud->tx_wq);`
   - `ud->rx_ready = false;`
   - `ud->tx_space = true;`

5. **Map UART registers**
   - `platform_get_resource(IORESOURCE_MEM, 0)` retrieves the address/size of the UART MMIO region.
   - `devm_ioremap_resource()` maps the physical address into kernel virtual address space (`ud->base`).

6. **Configure interrupt**
   - `platform_get_irq(pdev, 0)` retrieves the UART IRQ number.
   - `devm_request_irq()` registers `uart_mmio_irq()` as the interrupt handler for this device.

7. **Enable UART hardware**
   - `uart_mmio_hw_enable()`:
     - Reads `UART_CR`.
     - Sets `UARTEN | TXE | RXE` bits.
     - Writes the modified value back to `UART_CR`.

8. **Character device registration**
   - `alloc_chrdev_region()` reserves a major/minor number range.
   - `cdev_init(&ud->cdev, &uart_mmio_fops);`
   - `cdev_add(&ud->cdev, ud->devt, 1);`
   - `class_create()` (once) and `device_create()` create a device node `/dev/uart_mmio0`.

9. **Store driver data**
   - `platform_set_drvdata(pdev, ud)` associates `ud` with the platform device for later retrieval.

10. **Probe completion**
    - If all steps succeed, `probe()` returns 0 and the driver is ready.
    - Any intermediate error cleans up and returns a negative error code.

### 4.2 Probe Flow – Text Flowchart

- Start  
  ↓  
- Device Tree match → `uart_mmio_probe()`  
  ↓  
- Allocate `uart_mmio_dev` structure  
  ↓  
- Initialize mutex + wait queues  
  ↓  
- Map MMIO registers (`devm_ioremap_resource`)  
  ↓  
- Request IRQ (`devm_request_irq`)  
  ↓  
- Enable UART hardware (set control bits)  
  ↓  
- Register character device (`alloc_chrdev_region`, `cdev_add`, `device_create`)  
  ↓  
- Save driver context (`platform_set_drvdata`)  
  ↓  
- **Driver ready** (`/dev/uart_mmio0` available to user space)

## 5. Runtime Operation – Overall Workflow

### 5.1 Write Path (User → UART TX)

1. **User space write**
   - Application calls `write(fd, buf, count)` on `/dev/uart_mmio0`.

2. **Kernel: `uart_mmio_write()`**
   - Retrieves `struct uart_mmio_dev *ud` from `file->private_data`.
   - Acquires `mutex_lock(&ud->lock)` to serialize access to UART registers.
   - For each byte:
     - While TX FIFO is full (`UART_FR_TXFF` set):
       - Releases `ud->lock`.
       - Sleeps on `ud->tx_wq` via `wait_event_interruptible()`.
       - Upon wake-up, reacquires `ud->lock`.
     - Writes the byte to `UART_DR`.
   - Releases `ud->lock`.
   - Returns the number of bytes successfully written or an error.

3. **Interrupt-driven wakeup**
   - When the hardware indicates TX FIFO space:
     - UART asserts interrupt.
     - `uart_mmio_irq()` reads `UART_FR`.
     - If TX not full, sets `ud->tx_space = true`.
     - Calls `wake_up_interruptible(&ud->tx_wq)` to wake waiting writers.

**Properties**:

- No busy-wait loops in the kernel.
- Mutual exclusion provided by `mutex`.
- Writers block and wake efficiently using interrupts and wait queues.

### 5.2 Read Path (UART RX → User)

1. **User space read**
   - Application calls `read(fd, buf, count)` on `/dev/uart_mmio0`.

2. **Kernel: `uart_mmio_read()`**
   - Retrieves `ud` from `file->private_data`.
   - Acquires `mutex_lock(&ud->lock)`.
   - For each byte requested:
     - While RX FIFO is empty (`UART_FR_RXFE` set):
       - Releases `ud->lock`.
       - Sleeps on `ud->rx_wq` via `wait_event_interruptible()`.
       - On wake-up, reacquires `ud->lock`.
     - Reads data from `UART_DR`.
     - Copies data to user buffer with `copy_to_user()`.
   - Releases `ud->lock`.
   - Returns the number of bytes copied or an appropriate error.

3. **Interrupt-driven wakeup**
   - When a character arrives on RX:
     - UART asserts interrupt.
     - `uart_mmio_irq()` reads `UART_FR`.
     - If RX not empty, sets `ud->rx_ready = true`.
     - Calls `wake_up_interruptible(&ud->rx_wq)` to wake blocked readers.

**Properties**:

- `read()` is blocking by default and wakes when data is available.
- No polling loops; the design is interrupt-driven and power-efficient.

## 6. Interrupt Handling Flow

**Function**: `uart_mmio_irq(int irq, void *dev_id)`

Workflow:

1. **Entry**
   - `dev_id` is cast back to `struct uart_mmio_dev *ud`.

2. **Status read**
   - `fr = uart_readl(ud, UART_FR);`

3. **RX handling**
   - If `!(fr & UART_FR_RXFE)`:
     - Set `ud->rx_ready = true;`
     - `wake_up_interruptible(&ud->rx_wq);`

4. **TX handling**
   - If `!(fr & UART_FR_TXFF)`:
     - Set `ud->tx_space = true;`
     - `wake_up_interruptible(&ud->tx_wq);`

5. **Interrupt acknowledge**
   - On real hardware, the ISR would also clear or acknowledge specific interrupt status bits in UART interrupt registers (implementation depends on the IP).

6. **Return**
   - `return IRQ_HANDLED;`

**Design goal**: Keep ISR work minimal. All heavy processing (copying data, loops) is done in process context (`read`/`write`), not in interrupt context.

## 7. Remove Flow (Driver Unload)

**Goal**: Disable UART, remove `/dev/uart_mmio0`, and free all resources safely.

### 7.1 Step-by-Step Remove Sequence

1. **Triggered by** `rmmod` or manual unbind of the device.
2. Kernel calls `uart_mmio_remove(struct platform_device *pdev)`.
3. Driver:
   - Retrieves context: `ud = platform_get_drvdata(pdev);`
   - Calls `uart_mmio_hw_disable(ud);`
     - Clears enable, TX, and RX bits in `UART_CR`, stopping the UART.
   - Calls `device_destroy(ud->class, ud->devt);`
     - Removes `/dev/uart_mmio0`.
   - Calls `cdev_del(&ud->cdev);`
   - Calls `unregister_chrdev_region(ud->devt, 1);`
   - Returns 0 on success.
4. Devm-managed resources (MMIO mapping, IRQ, allocated memory) are automatically released by the kernel when the device is removed.

### 7.2 Remove Flow – Text Flowchart

- Start remove  
  ↓  
- `uart_mmio_remove()` invoked  
  ↓  
- Disable UART hardware (clear control bits)  
  ↓  
- Remove `/dev/uart_mmio0` device node  
  ↓  
- Delete `cdev` and free device numbers  
  ↓  
- Devm frees MMIO and IRQ resources  
  ↓  
- **Driver fully unloaded without leaks**

## 8. Build, Deployment, and Usage

### 8.1 Build

**Makefile example (out-of-tree module)**:

```make
obj-m += uart_mmio_platform.o

KDIR ?= /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
```

Build:

```bash
make
```

This produces `uart_mmio_platform.ko`.

### 8.2 Load and Test

Load module:

```bash
sudo insmod uart_mmio_platform.ko
dmesg | tail -n 40
```

Check device node:

```bash
ls -l /dev/uart_mmio0
```

TX test (send data to external UART):

```bash
echo "Hello UART" | sudo tee /dev/uart_mmio0
```

RX test (receive from external UART):

```bash
sudo cat /dev/uart_mmio0
```

Unload:

```bash
sudo rmmod uart_mmio_platform
dmesg | tail -n 40
```

## 9. Testing and Validation Checklist

- **Probe & device node**
  - [ ] Module inserts without error.
  - [ ] `dmesg` shows probe success.
  - [ ] `/dev/uart_mmio0` is created.

- **TX behavior**
  - [ ] Data written to `/dev/uart_mmio0` is observed on an external UART terminal.
  - [ ] Under repeated writes, no kernel warnings or oops occur.

- **RX behavior**
  - [ ] Incoming data from external device appears via `cat /dev/uart_mmio0`.
  - [ ] `read()` blocks when no data and wakes correctly when RX data arrives.

- **Concurrency and robustness**
  - [ ] Multiple processes can `read()` and `write()` without deadlocks or races.
  - [ ] No busy-wait loops; CPU usage remains low when idle.

- **Unload / reload**
  - [ ] `rmmod` works cleanly; `/dev/uart_mmio0` is removed.
  - [ ] Re-inserting the module restores full functionality.

