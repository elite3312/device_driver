# Character Device Driver Test

This directory contains a simple character device driver and a userspace test program.

## Files
- `my_char.c` - The kernel module (character device driver)
- `test_char.c` - Userspace test program
- `Makefile` - Build system for both driver and test program

## Quick Start

### 1. Build everything
```bash
make all
```

### 2. Load the driver and run test
```bash
# Load the kernel module
# sudo apt update && sudo apt install -y flex bison
sudo make install

# Run the test program
./test_char
```

### 3. Clean up
```bash
# Unload the kernel module
sudo make uninstall

# Clean build files
make clean
```

## Manual Testing

### Build and load driver
```bash
# Build the kernel module
make driver

# Load the module
sudo insmod my_char.ko

# Check if loaded
lsmod | grep my_char

# Check device nodes (should be created automatically)
ls -l /dev/my_char*
```

### Test with various tools
```bash
# Simple cat
cat /dev/my_char0

# Using dd
dd if=/dev/my_char0 bs=10 count=1

# Using our test program
./test_char

# Test different device nodes
./test_char /dev/my_char1
```

### Unload driver
```bash
sudo rmmod my_char
```

## Troubleshooting

### Missing kernel headers or build errors
If you get errors about missing header files (like `classmap.h`) or build failures:
```bash
# Try installing additional kernel development packages
sudo apt update
sudo apt install -y linux-headers-$(uname -r) linux-source build-essential
sudo apt install -y raspberrypi-kernel-headers  # For Raspberry Pi specifically

# If still failing, try building with verbose output
make V=1 driver
```

### Device nodes don't appear automatically
If device nodes don't appear automatically:
```bash
sudo make create-nodes
```

### Permission errors
If you get permission errors:
```bash
sudo chmod 666 /dev/my_char*
```

## What the test program does
1. Opens the specified device file (default: /dev/my_char0)
2. Reads data from the device
3. Prints the received data
4. Tries to read again to test EOF behavior
5. Closes the device

The driver returns "Hello from kernel space!\n" on the first read and EOF on subsequent reads.