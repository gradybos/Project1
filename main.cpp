//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Declaration and initialization of public global objects]===============

DigitalIn ignitionButton(BUTTON1);
DigitalIn driverSeatButton(D4);
DigitalIn driverBeltButton(D5);
DigitalIn passSeatButton(D6);
DigitalIn passBeltButton(D7);

DigitalOut ignitionLed(LED1);
DigitalOut engineLed(LED2);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();

void checkIgnition();
void onIgnition();

void welcomeMessage();
void engineStartMessage();
void errorMessage();

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();
    outputsInit();
    welcomeMessage();
    while (true) {
        checkIgnition();
        if (ignitionButton) {
            onIgnition();
        }
    }
}

//=====[Implementations of public functions]===================================

void inputsInit()
{
    driverSeatButton.mode(PullDown);
    driverBeltButton.mode(PullDown);
    passSeatButton.mode(PullDown);
    passBeltButton.mode(PullDown);
}

void outputsInit()
{
    ignitionLed = OFF;
    engineLed = OFF;
}

void welcomeMessage() {
    uartUsb.write( "Welcome to enhanced alarm system model 218-W24\r\n", 48 );
}

void engineStartMessage() {
    uartUsb.write( "Engine started.\r\n", 17 );
}

void errorMessage() {
    if (!driverSeatButton) {
        uartUsb.write( "Engine started.\r\n", 17 );
    }
    if (!driverBeltButton) {
        uartUsb.write( "Engine started.\r\n", 17 );
    }
    if (!passSeatButton) {
        uartUsb.write( "Engine started.\r\n", 17 );
    }
    if (!passBeltButton) {
        uartUsb.write( "Engine started.\r\n", 17 );
    }
}

void checkIgnition() {
    if (driverSeatButton && driverBeltButton && passSeatButton && passBeltButton && !engineLed) {
        ignitionLed = ON;
    }
    else {
        ignitionLed=OFF;
    }
}

void onIgnition() {
    if (ignitionLed) {
        ignitionLed = OFF;
        engineLed = ON;
        engineStartMessage();
    }
    else {
        errorMessage();
    }
}