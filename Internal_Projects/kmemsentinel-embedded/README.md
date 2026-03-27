# KmemSentinel Embedded

KmemSentinel Embedded is an embedded-C style memory leak detection project inspired by Linux `kmemleak`.
It tracks allocations, detects suspected leaks by age, and reports summary statistics.

## Why this name

`KmemSentinel` reflects:
- `kmem` inspiration from kernel memory leak tooling.
- `Sentinel` because it continuously watches allocation lifecycles.

## Features

- Lightweight allocation tracker (`KMEMS_MAX_TRACKED` slots).
- Allocation/free wrappers with source tags.
- Tick-based leak aging model (useful for RTOS/main-loop firmware style timing).
- Summary metrics: active allocations, bytes, suspected leaks, leaked bytes.
- Real Linux `kmemleak` integration mode (`/sys/kernel/debug/kmemleak`).
- Unit tests and runnable demo.

## Project layout

- `include/kmemsentinel.h` public API.
- `src/kmemsentinel.c` tracker implementation.
- `src/main.c` demo application.
- `tests/test_kmems.c` unit tests.
- `docs/DESIGN_AND_FLOW.md` architecture notes and flowchart.

## Build and run

```bash
cd kmemsentinel-embedded
make
make run-demo
make run-test
```

## Real kmemleak usage

Live mode (reads the kernel kmemleak interface):

```bash
./kmems_demo --live
```

Trigger a scan first, then read:

```bash
sudo ./kmems_demo --live --trigger-scan
```

Offline mode (parse saved kmemleak text file):

```bash
./kmems_demo --input tests/fixtures/kmemleak_sample.txt
```

## Notes for embedded integration

Replace `malloc/free` in `kmemsentinel.c` with platform allocator hooks when porting to bare metal or RTOS.
The `kmems_tick()` API can be called from a periodic timer interrupt or scheduler tick.
For `--live`, kernel config must enable kmemleak and `debugfs` must be mounted.
