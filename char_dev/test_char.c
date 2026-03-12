#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define DEVICE_PATH "/dev/my_char0"
#define BUFFER_SIZE 256

int main(int argc, char *argv[])
{
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    char *device_path = DEVICE_PATH;
    
    /* Allow user to specify device path */
    if (argc > 1) {
        device_path = argv[1];
    }
    
    printf("Testing character device: %s\n", device_path);
    
    /* Open the device file */
    fd = open(device_path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Error opening device %s: %s\n", 
                device_path, strerror(errno));
        printf("Make sure:\n");
        printf("1. The kernel module is loaded (lsmod | grep my_char)\n");
        printf("2. The device node exists (ls -l /dev/my_char*)\n");
        printf("3. You have read permissions on the device\n");
        return EXIT_FAILURE;
    }
    
    printf("Device opened successfully.\n");
    
    /* Clear the buffer */
    memset(buffer, 0, BUFFER_SIZE);
    
    /* Read from the device */
    printf("Reading from device...\n");
    bytes_read = read(fd, buffer, BUFFER_SIZE - 1);
    
    if (bytes_read < 0) {
        fprintf(stderr, "Error reading from device: %s\n", strerror(errno));
        close(fd);
        return EXIT_FAILURE;
    }
    
    if (bytes_read == 0) {
        printf("No data read (EOF)\n");
    } else {
        printf("Read %zd bytes: %s", bytes_read, buffer);
    }
    
    /* Try reading again to test EOF behavior */
    printf("Trying to read again...\n");
    bytes_read = read(fd, buffer, BUFFER_SIZE - 1);
    
    if (bytes_read < 0) {
        fprintf(stderr, "Error on second read: %s\n", strerror(errno));
    } else if (bytes_read == 0) {
        printf("Second read returned EOF (expected behavior)\n");
    } else {
        printf("Second read got %zd bytes: %s", bytes_read, buffer);
    }
    
    /* Close the device */
    close(fd);
    printf("Device closed successfully.\n");
    
    return EXIT_SUCCESS;
}