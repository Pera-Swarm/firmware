/**
* @brief 	PCF8574 arduino library
* @author 	SkyWodd <skywodd@gmail.com>
* @version 	2.0
* @url		https://github.com/skywodd/pcf8574_arduino_library
*
* Licensed under, GNU GENERAL PUBLIC LICENSE (Version 3, 29 June 2007)
* ------------------------------------------------------------------------------
*/

#include <Wire.h>
#include "robot_gpio.h"

#include "features.h"
#include "config/pins.h"

SW_GPIO::SW_GPIO() : _PORT(0), _PIN(0), _DDR(0){}

void SW_GPIO::begin() {

	// Configure inbuilt GPIO pins
	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_SW_0, INPUT_PULLUP);
	pinMode(PIN_SW_1, INPUT_PULLUP);

	#if defined(ENABLE_EXTERNAL_PORT)
	// Enable External Port
	Wire.begin();
	_readGPIO();
	#endif
}

uint8_t SW_GPIO::readModeButton(){
	// Read the mode button and return the value as an integer, range: 0-3 (4 modes)
	return (!digitalRead(PIN_SW_1) * 2) + (!digitalRead(PIN_SW_0));
}

void SW_GPIO::blinkLED(uint8_t count, int _delay){
	// Blink the built in LED of the ESP32 module by given count within given time interval
	while (count--) {
		digitalWrite(PIN_LED, HIGH);
		delay(_delay/2);
		digitalWrite(PIN_LED, LOW);
		delay(_delay/2);
	}
}

//------------------------------------------------------------------------------
#if defined(ENABLE_EXTERNAL_PORT)
//------------------------------------------------------------------------------

void SW_GPIO::gpioMode(uint8_t pin, uint8_t mode) {

	/* Switch according mode */
	switch (mode) {
		case INPUT:
		_DDR &= ~(1 << pin);
		_PORT &= ~(1 << pin);
		break;

		case INPUT_PULLUP:
		_DDR &= ~(1 << pin);
		_PORT |= (1 << pin);
		break;

		case OUTPUT:
		_DDR |= (1 << pin);
		_PORT &= ~(1 << pin);
		break;

		default:
		break;
	}

	/* Update GPIO values */
	_updateGPIO();
}
void SW_GPIO::gpioWrite(uint8_t pin, uint8_t value) {

	/* Set PORT bit value */
	if (value)
	_PORT |= (1 << pin);
	else
	_PORT &= ~(1 << pin);

	/* Update GPIO values */
	_updateGPIO();
}
uint8_t SW_GPIO::gpioRead(uint8_t pin) {

	/* Read GPIO */
	_readGPIO();

	#ifdef PCF8574_INTERRUPT_SUPPORT
	/* Check for interrupt (manual detection) */
	//checkForInterrupt();
	#endif

	/* Read and return the pin state */
	return (_PIN & (1 << pin)) ? HIGH : LOW;
}

void SW_GPIO::write(uint8_t value) {

	/* Store pins values and apply */
	_PORT = value;

	/* Update GPIO values */
	_updateGPIO();
}
uint8_t SW_GPIO::read() {

	/* Read GPIO */
	_readGPIO();

	#ifdef PCF8574_INTERRUPT_SUPPORT
	/* Check for interrupt (manual detection) */
	//checkForInterrupt();
	#endif

	/* Return current pins values */
	return _PIN;
}
void SW_GPIO::pullUp(uint8_t pin) {

	/* Same as gpioMode(INPUT_PULLUP) */
	gpioMode(pin, INPUT_PULLUP); // /!\ gpioMode form THE LIBRARY
}
void SW_GPIO::pullDown(uint8_t pin) {

	/* Same as gpioMode(INPUT) */
	gpioMode(pin, INPUT); // /!\ gpioMode form THE LIBRARY
}
void SW_GPIO::clear() {

	/* User friendly wrapper for write() */
	write(0x00);
}
void SW_GPIO::set() {

	/* User friendly wrapper for write() */
	write(0xFF);
}
void SW_GPIO::gpioToggle(uint8_t pin) {

	/* gpioToggle pin state */
	_PORT ^= (1 << pin);

	/* Update GPIO values */
	_updateGPIO();
}
void SW_GPIO::gpioBlink(uint8_t pin, uint16_t count, uint32_t duration) {

	/* Compute steps duration */
	duration /= count * 2;

	/* Loop n times */
	while (count--) {

		/* gpioToggle pin 2 times */
		gpioToggle(pin);
		delay(duration);
		gpioToggle(pin);
		delay(duration);
	}
}

void SW_GPIO::_readGPIO() {

	#ifdef PCF8574_INTERRUPT_SUPPORT
	/* Store old _PIN value */
	_oldPIN = _PIN;
	#endif

	/* Start request, wait for data and receive GPIO values as byte */
	Wire.requestFrom((uint16_t)I2C_PCF8574, (uint8_t) 0x01);

	// REM: if IC is not available, the program will be paused in here
	// while (Wire.available() < 1);
	_PIN = I2CREAD();
}
void SW_GPIO::_updateGPIO() {

	/* Read current GPIO states */
	//_readGPIO(); // Experimental

	/* Compute new GPIO states */
	//uint8_t value = ((_PIN & ~_DDR) & ~(~_DDR & _PORT)) | _PORT; // Experimental
	uint8_t value = (_PIN & ~_DDR) | _PORT;

	/* Start communication and send GPIO values as byte */
	Wire.beginTransmission(I2C_PCF8574);
	I2CWRITE(value);
	Wire.endTransmission();
}

//------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------

void SW_GPIO::gpioMode(uint8_t pin, uint8_t mode){}
void SW_GPIO::gpioWrite(uint8_t pin, uint8_t value) {}
uint8_t SW_GPIO::gpioRead(uint8_t pin){ return 0;}

void SW_GPIO::write(uint8_t value){}
uint8_t SW_GPIO::read(){return 0;}
void SW_GPIO::pullUp(uint8_t pin){}
void SW_GPIO::pullDown(uint8_t pin){}
void SW_GPIO::clear(){}
void SW_GPIO::set(){}
void SW_GPIO::gpioToggle(uint8_t pin){}
void SW_GPIO::gpioBlink(uint8_t pin, uint16_t count, uint32_t duration){}

void SW_GPIO::_readGPIO() {}
void SW_GPIO::_updateGPIO(){}


//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
