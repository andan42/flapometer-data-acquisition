#include "i2cutils.h"

int scanBusFlapDevice(int busNumber)

{
    int file;
    int addresses[] = {0x64, 0x65};
    char busPath[16];

    // Create the bus path string, e.g., "/dev/i2c-1"
    snprintf(busPath, sizeof(busPath), "/dev/i2c-%d", busNumber);

    // Open the I2C bus
    file = open(busPath, O_RDWR);
    if (file < 0)
    {
        // Failed to open the I2C bus
        return -1;
    }

    // Iterate over the possible addresses
    for (int address : addresses)
    {
        // Set the I2C address to the current device address
        if (ioctl(file, I2C_SLAVE, address) < 0)
        {
            // Skip this address if setting fails
            continue;
        }

        // Attempt to read a byte to confirm the device's presence
        int readResult = i2c_smbus_read_byte(file);
        if (readResult >= 0)
        {
            // Device is present at the current address
            close(file);
            return address;
        }
    }

    // Close the file descriptor and return -1 if no device was found
    close(file);
    return -1;
}