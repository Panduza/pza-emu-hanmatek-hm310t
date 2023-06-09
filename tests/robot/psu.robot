*** Settings ***
Library           ../rsc/Client.py

*** Test Cases ***
Read PSU info
    Psu model should be    3010
    Psu class should be    19288

Read voltage
    Read voltage goal
    Read voltage real

Read Amps
    Read amps goal
    Read amps real

Write voltage
    Write voltage goal    20
    Voltage goal should be    20
    Voltage real should be    20
    Write voltage goal    40
    Voltage goal should be    40
    Voltage real should be    40

Write Amps max
    Write amps goal    200
    Amps goal should be    200
    Write amps goal    500
    Amps goal should be    500

Turn ON OFF power supply
    Turn on power supply
    Power Supply Should Be    on
    Turn off power supply
    Power Supply Should Be    off

Read Power consumption
    Read power consumption

Write on read only register
    Write voltage goal    20
    Write voltage real    40
    Voltage real should be    20