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

bool canIgnite = false;

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();

void alarmActivationUpdate();
void alarmDeactivationUpdate();

void welcomeMessage();
void engineStartMessage();
void uartTask();
void availableCommands();

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();
    outputsInit();
    welcomeMessage();
    while (true) {
        alarmActivationUpdate();
        alarmDeactivationUpdate();
        uartTask();
    }
}

//=====[Implementations of public functions]===================================

void welcomeMessage() {
    uartUsb.write( "Welcome to enhanced alarm system model 218-W24\r\n", 48 );
}

void engineStartMessage() {
    uartUsb.write( "Engine started.\r\n", 17 );
}

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

void checkIgnition() {
    if (canIgnite) {
        ignitionLed = ON;
    }
    if (canIgnite && ignitionButton) {
        ignitionLed = OFF;
        engineLed = ON;
        engineStartMessage();
    }
    else if (!canIgnite && ignitionButton) {

    }
}

void uartTask(){
	char receivedChar = '\0';
	if( uartUsb.readable() ) {
		uartUsb.read( &receivedChar, 1 );
		if ( receivedChar == '2') {
			if ( alarmState ) {
				uartUsb.write( "The alarm is activated\r\n", 24 );
			} 
            else {
				uartUsb.write( "The alarm is not activated\r\n", 28 );
			}
		} 
        else if ( receivedChar == '1') {
			availableCommands();
		}
	}
}


void availableCommands()
{
    uartUsb.write( "Available commands:\r\n", 21 );
    uartUsb.write( "Press '2' to get the alarm state\r\n\r\n", 36 );
}
