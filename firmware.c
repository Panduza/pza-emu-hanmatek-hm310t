/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


// LIBRARY
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "tusb_config.h"

// MACROS
#define LIGHTMODBUS_SLAVE_FULL
#define LIGHTMODBUS_DEBUG
#define LIGHTMODBUS_IMPL

// LIGTHMODBUS LIB
#include "lightmodbus/lightmodbus.h"

#ifndef REG_COUNT
#define REG_COUNT 6250
#endif

// CONSTANTS
#define UART_ID uart1
#define BAUDRATE 9600
#define UART_TX_PIN 4
#define UART_RX_PIN 5

// DEBUG
#define debug(...) uart_puts(UART_ID, __VA_ARGS__);

static uint16_t registers[REG_COUNT];
static uint16_t inputRegisters[REG_COUNT];
static uint8_t coils[REG_COUNT / 8];
static uint8_t discreteInputs[REG_COUNT / 8];

// prototype functions
void init(const uint led_used);
void printErrorInfo(ModbusErrorInfo err);
void printAndSendFrameResponse(ModbusErrorInfo err, const ModbusSlave *slave);
void determinFunctionCodeError (char *responseLib);
void initHanmtekValue();
void readGoalToReal(int index);
void blink();
void setVoltageReal();
void setAmpsReal();
void setPowerRegisters();

// callback prototypes
ModbusError registerCallback(const ModbusSlave *slaveID,const ModbusRegisterCallbackArgs *args,ModbusRegisterCallbackResult *result);
ModbusError exceptionCallback(const ModbusSlave *slave,  uint8_t function, ModbusExceptionCode code);

int main() {

    int length = 9;
    int i_get=0;
    const uint LED_PIN = 25;
    uint8_t receiveBuffer[255];
    char betterArray[length];
    uint8_t single;
    
    ModbusSlave slave;
    ModbusErrorInfo error;

    stdio_init_all();
    uart_init(UART_ID, BAUDRATE);
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    initHanmtekValue();
    blink();

    debug("begining of program..\r\n");


    error = modbusSlaveInit(&slave, registerCallback, exceptionCallback, modbusDefaultAllocator, modbusSlaveDefaultFunctions, modbusSlaveDefaultFunctionCount);
    if(!modbusIsOk(error))
        debug("modbusSlaveInit() failed!\n\r");


    single = getchar();
    while(1){

        i_get=0;
        while ((single = getchar()) != EOF) {
            receiveBuffer[i_get] = single;
            error = modbusParseRequestRTU(&slave, 0x01, receiveBuffer, i_get+1);
            if(modbusIsOk(error)){
                break;
                i_get=0;
            }
            i_get++;
        }
        char str[50];

        printErrorInfo(error);
        if(modbusIsOk(error)){
            printAndSendFrameResponse(error, &slave);
        }
    }

      return 0;
}



void init(const uint led_used){
    stdio_init_all();
    uart_init(UART_ID, BAUDRATE);
    gpio_init(led_used);
    gpio_set_dir(led_used, GPIO_OUT);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

}

void initHanmtekValue(){
    //Holdings
    registers[0x0001] = 0;   // state

    registers[0x0003] = 3010; // model
    registers[0x0004] = 0x4b50; // ClassDetail 19280
    registers[0x0010] = 200; // volt real
    registers[0x0011] = 0;  // amps real
    registers[0x0012] = 0;  // Power H
    registers[0x0013] = 0;  // Power L

    registers[0x0020] = 0;  // OVP
    registers[0x0021] = 0;  // OCP
    registers[0x0022] = 0;  // OPP

    registers[0x0030] = 0;  // Volt goal
    registers[0x0031] = 0;  // Amps goal

    registers[0x9999] = 1; // PS_Device address

    registers[0x0024] = 100; //Resistor load
}

void readGoalToReal(int index){
    switch (index)
    {
        case 0x0001: //enable
            setVoltageReal();
            setAmpsReal();
            setPowerRegisters();
            break;
        case 0x0030: //voltage goal
            setVoltageReal();
            setAmpsReal();
            setPowerRegisters();
            break;
        case 0x0031: //Amps goal
            setAmpsReal();
            setPowerRegisters();
            break;
        case 0x0024: //Resistor load
            setAmpsReal();
            setPowerRegisters();
            break;
    }
}

void setVoltageReal(){
    uint32_t state = registers[0x0001];
    if(state == 0){
        registers[0x0010] = 0;
        return;
    }
    //if voltage asked is upper 30V, then set 30V
    if(registers[0x0030] <= 3000){
        registers[0x0010] = registers[0x0030];
    }else{
        registers[0x0010] = 3000;
    }
}

void setPowerRegisters(){
    uint32_t state = registers[0x0001];
    uint32_t voltage = registers[0x0010]; // volt real
    uint32_t current = registers[0x0011];  // amps real
    uint32_t power = state * (voltage * current) / 100;
    
    registers[0x0012] = power >> 16;  // Power H
    registers[0x0013] = power & 0xFFFF;  // Power L
}

void setAmpsReal(){
    uint32_t state = registers[0x0001];
    uint32_t voltage = registers[0x0010]; // volt real
    uint32_t current = 0;  // amps real
    uint32_t currentMax = registers[0x0031];  // amps goal max
    uint32_t resistorLoad = registers[0x0024];

    current = (state * voltage*10) / resistorLoad;
    if (current > currentMax){
        current = currentMax;
    }

    registers[0x0011] = current;
}

void blink(){
    gpio_put(25, 1);
    sleep_ms(300);
    gpio_put(25, 0);
    sleep_ms(100);
}

void printAndSendFrameResponse(ModbusErrorInfo err , const ModbusSlave *slave){

    for(int i = 0; i<modbusSlaveGetResponseLength(slave); i++) {
        printf("%c", modbusSlaveGetResponse(slave)[i]);
    }
}

/*
* goes in this callback when a frame is received
*/
ModbusError registerCallback(const ModbusSlave *slaveID,const ModbusRegisterCallbackArgs *args,ModbusRegisterCallbackResult *result){
    char str[1024];
    sprintf(str,"Register query:\r\ntquery: %s\r\n type: %s\r\n  id: %d\r\nvalue: %c\r\n  fun: %d\r\n",
        modbusRegisterQueryStr(args->query),
        modbusDataTypeStr(args->type),
		args->index,
		args->value,
		args->function
	);
    debug(str);

    switch (args->query)
    {
        // R/W access check
        case MODBUS_REGQ_R_CHECK:
        case MODBUS_REGQ_W_CHECK:
            result->exceptionCode = args->index < REG_COUNT ? MODBUS_EXCEP_NONE : MODBUS_EXCEP_ILLEGAL_ADDRESS;
            break;

        // Read register
        case MODBUS_REGQ_R:
            switch (args->type)
            {
                case MODBUS_HOLDING_REGISTER: result->value = registers[args->index]; break;
                case MODBUS_INPUT_REGISTER:   result->value = inputRegisters[args->index]; break;
                case MODBUS_COIL:             result->value = modbusMaskRead(coils, args->index); break;
                case MODBUS_DISCRETE_INPUT:   result->value = modbusMaskRead(discreteInputs, args->index); break;
            }
            break;
 
        // Write register
        case MODBUS_REGQ_W:
            switch (args->type)
            {
                case MODBUS_HOLDING_REGISTER: registers[args->index] = args->value; readGoalToReal(args->index); break;
                case MODBUS_COIL:             modbusMaskWrite(coils, args->index, args->value); break;
                default:                      break;
            }
            break;
    }
 
    return MODBUS_OK;

}
	
ModbusError exceptionCallback(const ModbusSlave *slave,  uint8_t function, ModbusExceptionCode code){
    char exceptionInfo[15];
    debug("in error register callback\r\n");
    debug(modbusExceptionCodeStr(code)); // send the error cause to the debug
	return MODBUS_OK;

}


/*
* check if modbus is initialized
*/
void printErrorInfo(ModbusErrorInfo err)
{
	if (modbusIsOk(err)){
		debug("FRAME IS SEND TO THE LIB \r\n");
  
    }else{
        debug("THERE IS A PROBLEM WITH THE INIT OF\r\n");
		debug(modbusErrorSourceStr(modbusGetErrorSource(err)));
		debug(modbusErrorStr(modbusGetErrorCode(err)));
    }
}
