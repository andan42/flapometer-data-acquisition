#!/bin/bash

# Path to your compiled C++ program
PROGRAM_PATH="/home/pi/Projects/FlapometerCpp/build/a.out"

# Command-line arguments for your program


ARG1="--serverAddress"
ARG2="https://flap.hackmed.ro"
ARG7="--operationName"
ARG8="Double_Diep_Flap"
ARG9="--patientName"
ARG10="5ac0783e-91da-44f6-af34-ae8c2bb11362"
ARG11="--runTime"
ARG12="3580"


PING_FAILURE_COUNT=0
MAX_FAILURES=10

check_ping_initial() {
    PING_TARGET="8.8.8.8"
    echo "Checking for initial network connection..."

    while true; do
        if ping -c 1 $PING_TARGET &> /dev/null; then
            echo "Network is up"
            break
        else
            echo "Network is down, waiting for it to come up..."
            sleep 10
        fi
    done
}

check_program_running() {
  PROGRAM_NAME="a.out"  # Name of the executable
  if pgrep -x "$PROGRAM_NAME" > /dev/null
  then
    echo "$PROGRAM_NAME is running"
  else
    echo "$PROGRAM_NAME is not running, rebooting..."
    sleep 120
    sudo reboot
  fi
}


# Function to check network connectivity
check_ping() {
  PING_TARGET="8.8.8.8"
  PING_FLORIN="10.180.190.1"
  if ping -c 1 $PING_TARGET &> /dev/null
  then
    echo "Network is up"
    if ping -c 1 $PING_FLORIN &> /dev/null
    then
        echo "Connected to vpn"
    else
        ((PING_FAILURE_COUNT++)) 
        echo "Not connected to vpn"
    fi
  else
    ((PING_FAILURE_COUNT++))
    if [ $PING_FAILURE_COUNT -ge $MAX_FAILURES ]
    then
      echo "Maximum ping failures reached, rebooting..."
      echo "Network and vpn is down, I must reboot"
      sudo reboot
    fi
   
   fi
}
check_ping_initial
schedule_reboot() {
  total_time=3600  # Total time in seconds (1 hour)
  interval=300     # 5-minute interval in seconds
  remaining_time=$total_time
  
  echo "System will reboot in 1 hour..."

  # Loop to echo remaining time every 5 minutes
  while [ $remaining_time -gt 0 ]; do
    sleep $interval  # Wait for 5 minutes (300 seconds)
    remaining_time=$((remaining_time - interval))  # Decrease the remaining time
    echo "Time left until reboot: $((remaining_time / 60)) minutes"
    echo "RAM usage at $(date):"
    free -h
  done

  # Once the loop finishes, reboot the system
  echo "Rebooting system after 1 hour of uptime..."
  echo "RAM before reboot $(date):"
  free -h
  sudo reboot
}

# Run the reboot scheduler in the background
schedule_reboot &
# Run the program with arguments in the background
echo "RAM before cpp startup $(date):"
free -h
$PROGRAM_PATH $ARG1 $ARG2 $ARG7 "$ARG8" $ARG9 $ARG10 $ARG11 $ARG12 &
echo "RAM usage after Cpp startup at $(date):"
free -h
# Keep checking the network every minute
while true; do
  # check_ping_initial
  check_program_running
  check_ping
  sleep 10
done