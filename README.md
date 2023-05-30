# Pza Emulator for hanmatek/hm310t power supply

The **main goals** of this emulator are:

- to provide the exact same USB interface that the **hanmatek/hm310t** power supply.
- to run on a RaspberryPi Pico H to be cheap and easily integrated on a test bench.

`Why not virtual USB interfaces ?`

To have a solution independent from OS and their implementation specificities


# Limitation

The State send by the Client (client.py) must be True otherwise (False or something else) the controle of the Pico crashes.


# Build and run project

mkdir build

cd build

cmake ..

make

cp file_name.uf2 /media/<user>/RPI-RP2/

# Run test

python3 client.py
