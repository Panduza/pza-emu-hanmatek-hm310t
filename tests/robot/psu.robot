*** Settings ***
Library           ../rsc/Client.py

*** Test Cases ***
Read PSU info
    Psu model should be    3010
    Psu class should be    19280

Init
    Write voltage goal    500
    Write amps goal    300
    Turn off power supply
    Write resistor load    100

    Voltage goal should be    500
    Voltage real should be    0
    Amps goal should be    300
    Amps real should be    0
    Resistor load should be    100
    Power supply should be    off
    Power consumption should be    0
    
Test Enable
    Turn on power supply
    
    Voltage goal should be    500
    Voltage real should be    500
    Amps goal should be    300
    Amps real should be    50
    Resistor load should be    100
    Power supply should be    on
    Power consumption should be    250

Test voltage
    Write voltage goal    1000
    
    Voltage goal should be    1000
    Voltage real should be    1000
    Amps goal should be    300
    Amps real should be    100
    Resistor load should be    100
    Power supply should be    on
    Power consumption should be    1000    

Test Amps max
    Turn off power supply
    Write amps goal    50
    Turn on power supply


    Voltage goal should be    1000
    Voltage real should be    1000
    Amps goal should be    50
    Amps real should be    50
    Resistor load should be    100
    Power supply should be    on
    Power consumption should be    500

    Turn off power supply
    Write amps goal    300
    Turn on power supply

    Voltage goal should be    1000
    Voltage real should be    1000
    Amps goal should be    300
    Amps real should be    100
    Resistor load should be    100
    Power supply should be    on
    Power consumption should be    1000

Test Resistor load
    Write amps goal    300
    Write resistor load    200
    
    Voltage goal should be    1000
    Voltage real should be    1000
    Amps goal should be    300
    Amps real should be    50
    Resistor load should be    200
    Power supply should be    on
    Power consumption should be    500

Shutdown
    Turn off power supply
    Power supply should be    off

    