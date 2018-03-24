# Wrist Controller Desktop Client
The desktop client for the Wristband Gesture Controller.

## Designed to run with the following hardware:
- HC-06 Bluetooth chip
- ATmega328p microprocessor
- LSM9DS0 3-axis sensor

## To run
1. Install dependencies
- `sudo apt-get install libbluetooth-dev`
2. Start the bluetooth daemon
- `sudo systemctl start bluetooth`
3. Confirm status
- `sudo systemctl status bluetooth`
4. Start bluetooth client
- `bluetoothctl`
- `power on`
5. Trust device to cancel PIN
- `trust 98:D3:31:FC:95:BD`
6. Pair with device
- `pair 98:D3:31:FC:95:BD`
7. Exit from bluetooth
- `exit`
8. Bind virtual port to device
- `sudo rfcomm bind 0 98:D3:31:FC:95:BD`
9. Compile and run runner.c


#### Developed for Pitt Senior Design 2018
