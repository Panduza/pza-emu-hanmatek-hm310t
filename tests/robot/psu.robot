*** Settings ***
Library           ../rsc/Client.py

*** Test Cases ***
Read voltage
    Read voltage goal
Write voltage
    Write voltage goal    20
    Voltage real should be    20
    Write voltage goal    40
    Voltage real should be    40
