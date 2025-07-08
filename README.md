# thread-pool

>A portable C thread pool implementation

---

## Overview

This repository provides a minimal thread pool implementation in C. It is designed for simplicity and efficiency, making it suitable as a starting point or reference for integrating multithreading into C projects of mine.

## Features

- Lightweight thread pool
- Simple worker and task management
- Written in pure C for portability

## Getting Started

### Prerequisites

- GCC or any standard C compiler
- POSIX threads (pthread)

### Building

```sh
./compile.sh
```

-> This compiles and executes the benchmarking operation for an example scenario with 50 tasks ( 10 threads max )


### Usage

The external and internal library functions can be viewed in the `thrd_pl.h` and `thrd_pl_int.h`.

## Project Structure

- `bench.c` – Example entry point for running the thread pool
- `thrd_pl.c` – Core thread pool implementation
- `thrd_pl.h` – Internal function declarations
- `thrd_pl_int.h` – Internal function declarations


## Screenshots / Diagrams

![Example scenario execution](https://github.com/user-attachments/assets/4b925802-93e3-4077-90f4-8e85541681ae)


## Contributing

Nothing to contribute honestly, it is just me learning and designing.


## Author

[@shri-acha](https://github.com/shri-acha)
