# Wrist Controller Desktop Client
A destop client to process data from the Wrist Controller device 

## To run
1. Install dependencies
`sudo apt-get install libbluetooth-dev`
2. Start the bluetooth daemon
`sudo sysemctl start bluetooth`
3. Confirm status
`sudo systemctl status bluetooth`
4. Start bluetooth client
`bluetoothctl`
`power on`
5. Pair with device
`pair 98:D3:31:FC:95:BD`
6. Create virtual terminal device
`sudo rfcomm bind 0 98:D3:31:FC:95:BD`
7. Read from /dev/rfcomm0


#### Developed for Pitt Senior Design 2018
