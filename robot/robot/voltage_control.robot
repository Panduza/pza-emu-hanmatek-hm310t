*** Settings ***
Library           Client.py

*** Variables ***
${start}      0
${end}        3000
${step}       1

*** Test Cases ***

Test All Possible Values
    
    Turn Off Power Supply

    ${voltage}=    Set Variable    ${start}
    FOR    ${voltage}    IN RANGE    ${start}    ${end}    ${step}
        Log    ${voltage}
        Write voltage goal    ${voltage}
        Voltage goal should be      ${voltage}
    END

