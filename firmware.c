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
#define BAUDRATE 115200
#define UART_TX_PIN 4
#define UART_RX_PIN 5



// DEBUG
#define debug(...) uart_puts(UART_ID, __VA_ARGS__);

// uint8_t coils[REG_COUNT / 8];
// uint16_t regs[REG_COUNT];


static uint8_t registers[REG_COUNT];
static uint8_t inputRegisters[REG_COUNT];
static uint8_t coils[REG_COUNT / 8];
static uint8_t discreteInputs[REG_COUNT / 8];



// prototype functions
void init(const uint led_used);
void decodeFrame(char reception[], int size, int sizeOfData);
void hexToASCII (char usefulData[]);
void printErrorInfo(ModbusErrorInfo err);
void printAndSendFrameResponse(ModbusErrorInfo err, const ModbusSlave *slave);
void determinFunctionCodeError (char *responseLib);
void initHanmtekValue();
void readGoal(int index);
void blink();

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
    gpio_put(LED_PIN, 1);
    sleep_ms(500);
    gpio_put(LED_PIN, 0);
    sleep_ms(250);


    debug("begining of program..\r\n");



    error = modbusSlaveInit(&slave, registerCallback, exceptionCallback, modbusDefaultAllocator, modbusSlaveDefaultFunctions, modbusSlaveDefaultFunctionCount);
    if(!modbusIsOk(error))
        debug("modbusSlaveInit() failed!\n\r");


    single = getchar();
    while(1){

        i_get=0;
        debug("Reading hex data from stdin...\n\r");
        while ((single = getchar()) != EOF) {
            debug("inside.\r\n"); 
            debug("processing \r\n");
            receiveBuffer[i_get] = single;
            error = modbusParseRequestRTU(&slave, 0x01, receiveBuffer, i_get+1);
            if(modbusIsOk(error)){
                debug("break\r\n");
                break;
                i_get=0;
            }else{
                debug("checking the data... \r\n");
                char strDebug[250];
                sprintf(strDebug, "value of i %d, value of single %.2X value of buffer %.2X value of modbus check %d \r\n", i_get, single, receiveBuffer[i_get], modbusIsOk(error));
                debug(strDebug);
                debug("test \r\n");
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
    //tusb_init();
    stdio_init_all();
    uart_init(UART_ID, BAUDRATE);
    gpio_init(led_used);
    gpio_set_dir(led_used, GPIO_OUT);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

}

void initHanmtekValue(){
    //Coils
    //modbusMaskWrite(coils, 0x01, 1); //Power output/stop setting
    //coils[0x01]=1;

    //Holdings
    registers[0x0001] = 0;   //state
    registers[0x0010] = 200; //volt real
    registers[0x0011] = 20;  //amps real
}

void readGoal(int index){
    switch (index)
    {
        case 0x0030: //voltage real 
            registers[0x0010] = registers[index];
            break;
        case 0x0031: //amps real
            registers[0x0011] = registers[index];
            break;
    }
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

    debug("inside callback\n\r");
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
                case MODBUS_HOLDING_REGISTER: registers[args->index] = args->value; readGoal(args->index); break;
                case MODBUS_COIL:             blink(); modbusMaskWrite(coils, args->index, args->value); break;
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
		//printf("%s: it comes from the following element : %s",
		debug(modbusErrorSourceStr(modbusGetErrorSource(err)));
		debug(modbusErrorStr(modbusGetErrorCode(err)));
    }
}

/*
* https://rapidscada.net/modbus/
*/
void decodeFrame(char reception[], int size, int sizeOfDataFrame){

    char slaveID;
    char functionCode;
    char addressStart;
    char sizeOfData;
    char byteCount;
    char data[sizeOfDataFrame];
    char checksum[2];
    int j = 0;
    
    slaveID = reception[0];
    functionCode = reception[1];
    addressStart = reception[3];
    sizeOfData = reception[5];
    byteCount = reception[6];
    checksum[0]= reception[size - 2];
    checksum[1] = reception[size - 1];

    for(int i = 7; i < byteCount + 7; i++){
        data[j] = reception[i];
        //printf(" %02X", data[j]);
        j++;
    }

   // hexToASCII(data);

    //printf("\r\n");
    //printf("frame format : \r\n");
    //printf (" slave address -- function code -- starting address -- quantity -- byte count -- data -- CRC \r\n");
    //printf ("The value of slave ID is %02X \r\n the value of function code is %02X \r\n the value of starting address is %02X \r\n the value of quantity is %02X \r\n  the value of byte count is %02X \r\n the CRC bytes are %02X and %02X \r\n", slaveID, functionCode, addressStart, sizeOfData, byteCount, checksum[0], checksum[1]);
    sleep_ms(2000);
}


void hexToASCII(char usefulData[]){
    char dataInASCII[14];

    for(int i =0; i< strlen(usefulData); i+=2){
        sscanf(&usefulData[i], "%2x", &dataInASCII[i/2]);
    }

    //printf("data in interpretable String : ");
    for(int i = 0; i< sizeof(dataInASCII); i++){
        //printf("%s", dataInASCII[i]);
    }
    //printf("\r\n");


}
