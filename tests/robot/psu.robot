*** Settings ***
Library           ../rsc/Client.py

*** Test Cases ***
Read PSU info
    Psu model should be    3010
    Psu class should be    19288

Read enable
    Read enable

Read voltage
    Read voltage goal
    Read voltage real

Read Amps
    Read amps goal
    Read amps real

Write voltage
    Write voltage goal    330
    Voltage goal should be    330
    Voltage real should be    330
    Write voltage goal    500
    Voltage goal should be    500
    Voltage real should be    500

Write Amps max
    Write amps goal    200
    Amps goal should be    200
    Read amps real
    Write amps goal    500
    Amps goal should be    500
    Read amps real

Turn ON OFF power supply
    Turn on power supply
    Power Supply Should Be    on
    Turn off power supply
    Power Supply Should Be    off

Read Power consumption
    Turn on power supply
    Power Supply Should Be    on
    Read voltage real
    Read amps real
    Read power consumption

Write on read only register
    Write voltage goal    20
    Write voltage real    40
    Voltage real should be    20

Current capping
    Write voltage goal    3000
    Write amps goal    30
    Amps real should be    30
    Turn off power supply
    Amps real should be    0
    