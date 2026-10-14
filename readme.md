# Kalman Filter Visualization (Windows)

A small Windows desktop application that demonstrates a scalar Kalman filter
estimating a constant signal from noisy measurements, and plots the result with
native Win32 GDI.

> **Important: this project only runs on Windows.** It relies on the Windows API
> (`windows.h`, `WinMain`, `WNDCLASS`, `CreateWindowExA`, GDI drawing, etc.) for
> its graphical output, so it cannot be compiled or run on Linux or macOS.

## Overview

The program generates a constant "true" signal of `25.0`, corrupts it with
Gaussian noise (`sigma = 2.0`), and passes the noisy samples through a
one-dimensional Kalman filter. Three curves are drawn in a window:

| Curve | Color | Meaning                              |
|-------|-------|--------------------------------------|
| INPUT | Red   | Noisy measurement `z`                |
| REAL  | Black | True (ground-truth) value            |
| PRIOR | Green | Kalman filter estimate               |

The estimate converges toward `25.0` and progressively rejects the measurement
noise.

## Files

| File        | Description                                                   |
|-------------|---------------------------------------------------------------|
| `kalman.h` / `kalman.c` | Scalar Kalman filter implementation (C)                 |
| `noise.hpp` / `noise.cpp` | Seeded LCG + Box-Muller Gaussian noise generator (C++)  |
| `curve.hpp` / `curve.cpp` | `Curve` container holding a data series and its type    |
| `graph.hpp` / `graph.cpp` | Win32 GDI window and plotting routines                 |
| `main.cpp`  | `WinMain` entry point; prepares data and shows the graph |
| `Makefile`  | Build script for `mingw32-make`                            |

## Requirements

- **Windows** (the only supported platform)
- MinGW-w64 (`g++`) with GNU Make (`mingw32-make`)
- The code is compiled as a GUI application (`-mwindows`), so no console window
  is created.

## Build

Open a terminal in this directory and run:

```sh
mingw32-make
```

This produces `kalman_plot.exe`. To clean up build artifacts:

```sh
mingw32-make clean
```

## Run

Double-click `kalman_plot.exe`, or run it from the terminal:

```sh
.\kalman_plot.exe
```

A window titled "Kalman Filter" opens showing the noisy input (red), the true
value (black), and the filtered estimate (green).

## Notes

- The y-axis range is fixed to `18.0`-`32.0` and the x-axis step is `1.0`.
- The random number generator is seeded with `42`, so the output is reproducible.
- The Kalman filter is initialized with process noise `Q = 0.01` and measurement
  noise `R = 4.0`.
