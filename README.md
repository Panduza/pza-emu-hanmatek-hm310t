# Pza Emulator for hanmatek/hm310t power supply

The **main goals** of this emulator are:

- to provide the exact same USB interface that the **hanmatek/hm310t** power supply.
- to run on a RaspberryPi Pico H to be cheap and easily integrated on a test bench.

`Why not virtual USB interfaces ?`

To have a solution independent from OS and their implementation specificities


# Limitation

The Voltage send by the Client (client.py) must be in the range 0 2.55.

The Amperage send by the Client (client.py) must be in the range 0 0.255.

The State send by the Client (client.py) must be True otherwise (False or something else) the Pico crashes.


# build and run project

mkdir build

cd build

cmake ..

make

cp file_name.uf2 /media/<user>/RPI-RP2/
