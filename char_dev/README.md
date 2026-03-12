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

If device nodes don't appear automatically:
```bash
sudo make create-nodes
```

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