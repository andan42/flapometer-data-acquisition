# FlapometerCpp
## Raspberry Pi setup instructions for compiling project

### Enable I2C interface:
```bash
sudo raspi-config
```
### Add lines in `/boot/config.txt`  :
```bash
#A
dtoverlay=i2c-gpio,bus=6,i2c_gpio_sda=20,i2c_gpio_scl=21
#B
dtoverlay=i2c-gpio,bus=5,i2c_gpio_sda=7,i2c_gpio_scl=1
#C
dtoverlay=i2c-gpio,bus=4,i2c_gpio_sda=0,i2c_gpio_scl=5
#D
dtoverlay=i2c-gpio,bus=3,i2c_gpio_sda=19,i2c_gpio_scl=26
```
```bash
sudo reboot now
```
### Use I2C tools to debug hardware connections:
```bash
sudo apt-get install i2c-tools
```
```bash
i2cdetect -y 1
i2cdetect -y 5
i2cdetect -y 6
```
(should return 48, 64, 65)
### Install libraries:
```bash
sudo apt-get install clang
sudo apt-get install libcurl4-openssl-dev
sudo apt-get install libjsoncpp-dev
sudo apt-get install libi2c-dev
```
## Instructions for compiling tests
```
sudo apt-get install cmake
sudo apt-get install libgtest-dev
cd /usr/src/gtest
sudo cmake -Bbuild
sudo cmake --build build
sudo cp ./build/lib/libgtest* /usr/lib
sudo cp ./build/lib/libgtest_main* /usr/lib
cd ~
```

# How to Enable/Disable Pi Health Monitor Service
This section explains usage of `pi_health_monitor.sh` and `pi_health_monitor.service` to ensure the Raspberry Pi restarts every 60 minutes and runs our project on startup. The following commands allow enabling and disabling this.

## Initial Setup (After Cloning)
If you are setting up the service for the first time on a new device, follow these steps:

1. **Navigate to the project directory:**
    ```sh
    cd /home/pi/Projects/FlapometerCpp/
    ```

2. **Copy the service file to the systemd directory:**
    ```sh
    sudo ln -s /home/pi/Projects/FlapometerCpp/pi_health_monitor.service /etc/systemd/system/pi_health_monitor.service
    ```

3. **Reload systemd to recognize the service:**
    ```sh
    sudo systemctl daemon-reload
    ```

4. **Enable and start the service:**
    ```sh
    sudo systemctl enable pi_health_monitor.service
    sudo systemctl start pi_health_monitor.service
    ```

Now the service will run on startup and restart the Raspberry Pi every 60 minutes.

---

## Disable the service (Prevent it from running)
Run:
    sudo systemctl stop pi_health_monitor.service
    sudo systemctl disable pi_health_monitor.service

## Enable the service (Start it again)
Run:
    sudo systemctl enable pi_health_monitor.service
    sudo systemctl start pi_health_monitor.service

## Check service status
Run:
    systemctl status pi_health_monitor.service

---

## Troubleshooting
- If the service does not appear in `systemctl list-unit-files`, make sure the `.service` file is in the correct location and run:
    ```sh
    sudo systemctl daemon-reload
    ```
- If you cloned the project into a different folder, update the `ExecStart` path in `pi_health_monitor.service` to match your new directory.

