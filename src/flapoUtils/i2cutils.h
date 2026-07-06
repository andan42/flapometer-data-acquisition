#pragma once
#include <fcntl.h>
#include <sys/ioctl.h>
extern "C"
{
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}
#include <unistd.h>
#include <cstdio>

int scanBusFlapDevice(int busNumber);
