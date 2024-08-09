# Raspberry Pi Kernel Development

This project involves developing a small kernel for the Raspberry Pi, implementing a command-line interface (CLI) with dynamic memory allocation, Caesar cipher functionality, and more. The goal is to gain hands-on experience with low-level programming, kernel development, and working with ARM architecture.

## Getting Started

### Prerequisites

1. **Cross Compiler**: Download the `gcc-arm-none-eabi` cross compiler from [developer.arm.com](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads).
2. **QEMU**: Install `qemu-system-arm` for emulating the Raspberry Pi hardware.

### Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/fai08-Ibrahim/raspi-kernel.git
   cd raspi-kernel
   ```

2. **Download Necessary Packages**:
   ```bash
   sudo apt update
   sudo apt install python2 qemu-system-arm
   sudo ln -sf python2 /usr/bin/python
   ```

3. **Set Up the Project Structure**:
   - Create directories: `build`, `src/kernel`, and `include/kernel`.
   - Copy `boot.S` and `kernel.c` into `src/kernel`.
   - Place `Makefile` and `linker.ld` in the `build` directory.

## Building the Kernel

1. **Compile the Source Files**:
   ```bash
   cd build
   make
   ```

2. **Run the Kernel on QEMU**:
   ```bash
   make run
   ```

3. **Clean Build Artifacts**:
   ```bash
   make clean
   ```

## Understanding the Code

### boot.S
- **_start**: Entry point where control is handed from the bootloader to the kernel.
- **Stack Pointer**: Set up in `boot.S`.
- **BSS Segment**: Cleared and initialized for C global variables.
- **Kernel Main**: Execution jumps to `kernel_main` after setup.

### kernel.c
- **UART**: Handles input/output through the Raspberry Pi's UART interface.
- **Memory Mapping**: Interactions with UART are memory-mapped.
- **Registers**: Access to UART's data and flags registers.

### linker.ld
- **Memory Layout**: Defines the layout, starting at address `0x8000`.
- **Sections**: `.text`, `.rodata`, `.data`, and `.bss` are aligned for memory paging.

## Features

- **Dynamic Memory Allocation**: Implemented with custom `malloc` and `free` functions.
- **Command-Line Interface (CLI)**: Supports basic command processing and Caesar cipher encryption/decryption.
- **Custom `printf` Implementation**: Supports `%s` for strings and `%d` for integers.
- **Linked Lists**: Managed with dynamic memory allocation.

## Project Structure

```
project-root/
├── build/
│   ├── Makefile
│   ├── linker.ld
├── include/
│   ├── common/
│   └── kernel/
├── src/
│   ├── common/
│   └── kernel/
```