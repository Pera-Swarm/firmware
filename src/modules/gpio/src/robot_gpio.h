/**
* @brief 	PCF8574 arduino library
* @author 	SkyWodd <skywodd@gmail.com>
* @version 	2.0
* @url		https://github.com/skywodd/pcf8574_arduino_library
*
* Licensed under, GNU GENERAL PUBLIC LICENSE (Version 3, 29 June 2007)
* ------------------------------------------------------------------------------
*/

#ifndef SW_GPIO_H
#define SW_GPIO_H

#include "Arduino.h"

#define I2CWRITE(x) Wire.write(x)
#define I2CREAD() Wire.read()

class SW_GPIO {
public:

	SW_GPIO();

	//Start the I2C controller and store the PCF8574 chip address
	void begin();

	// Mode button value, [0,3]
	uint8_t readModeButton();
	void blinkLED(uint8_t count, int delay);

	//===========================================================================
	// I2C Port Expander Related Functions

	//Set the direction of a pin (OUTPUT, INPUT or INPUT_PULLUP)
	void gpioMode(uint8_t pin, uint8_t mode);

	//Set the state of a pin (HIGH or LOW)
	void gpioWrite(uint8_t pin, uint8_t value);

	//Read the state of a pin
	uint8_t gpioRead(uint8_t pin);

	//Toggle the state of a pin
	void gpioToggle(uint8_t pin);

	//Mark a pin as "pulled up"
	void pullUp(uint8_t pin);

	// Mark a pin as "pulled down"
	void pullDown(uint8_t pin);

	//Make a pin gpioBlink N times for T duration
	void gpioBlink(uint8_t pin, uint16_t count, uint32_t duration);

	//===========================================================================

private:
	//Set the state of all pins in one go
	void write(uint8_t value);

	//Read the state of all pins in one go
	uint8_t read();

	//Exactly like write(0x00), set all pins to LOW
	void clear();

	//Exactly like write(0xFF), set all pins to HIGH
	void set();

protected:

	volatile uint8_t _PORT;	// Output pins value
	volatile uint8_t _PIN;	// Current input pins values
	volatile uint8_t _DDR;	// Pins modes values (OUTPUT or INPUT)

	uint8_t _address;			// PCF8574 I2C address

	void _readGPIO();			//Read GPIO states and store them in _PIN variable
	void _updateGPIO();		//Write value of _PORT variable to the GPIO
};

#endif
