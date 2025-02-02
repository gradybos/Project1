//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Declaration and initialization of public global objects]===============

DigitalIn ignitionButton(BUTTON1);
DigitalIn driverSeatButton(D4);
DigitalIn driverBeltButton(D5);
DigitalIn passSeatButton(D6);
DigitalIn passBeltButton(D7);

DigitalInOut alarm(PE_10);
DigitalOut ignitionLed(LED1);
DigitalOut engineLed(LED2);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============

bool alarmState = OFF;

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
        if (ignitionButton && !alarmState && !engineLed) {
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
    alarm.mode(OpenDrain);
    alarm.input();
}

void outputsInit()
{
    ignitionLed = OFF;
    engineLed = OFF;
}

void welcomeMessage() {
    // Display welcome message
    uartUsb.write( "Welcome to enhanced alarm system model 218-W24\r\n", 48 );
}

void engineStartMessage() {
    // Display successful engine start message
    uartUsb.write( "Engine started.\r\n", 17 );
}

void errorMessage() {
    // Display error message based on which conditions failed
    uartUsb.write("Ignition inhibited.\r\n", 21);
    if (!driverSeatButton) {
        uartUsb.write( "Driver seat not occupied.\r\n", 27 );
    }
    if (!driverBeltButton) {
        uartUsb.write( "Driver seatbelt not fastened.\r\n", 31 );
    }
    if (!passSeatButton) {
        uartUsb.write( "Passenger seat not occupied.\r\n", 30 );
    }
    if (!passBeltButton) {
        uartUsb.write( "Passenger seatbelt not fastened.\r\n", 34 );
    }
}

void checkIgnition() {
    // Check if ignition conditions are met and turn 
    // ignitionLED on or off based on the outcome
    if (driverSeatButton && driverBeltButton && passSeatButton && passBeltButton && !engineLed) {
        ignitionLed = ON;
    }
    else {
        ignitionLed = OFF;
    }
}

void onIgnition() {
    // Determine outcome of ignition and display results
    if (ignitionLed) {
        // Successful ignition turns on engine LED and
        // displays start message
        ignitionLed = OFF;
        engineLed = ON;
        engineStartMessage();
    }
    else {
        // Failed ignition displays error message and
        // turns on alarm siren
        errorMessage();
        alarm.output();
        alarm = LOW;
        alarmState = ON;
    }
}