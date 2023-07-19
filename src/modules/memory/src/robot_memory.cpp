/**
 * @brief    Swarm Robot Memory Manager Library
 * @author   Nuwan Jaliyagoda, Pasan Tennakoon, Dilshani Karunarathna
 * @version  1.0.0
 * @url      N/A
 *
 * ------------------------------------------------------------------------------
 */

#include <Arduino.h>
#include "EEPROM.h"
#include "robot_memory.h"

#include "features.h"
#include "config/pins.h"
#include "config/definitions.h"

#define MIN_ROBOT_ID 0
#define MAX_ROBOT_ID 31

#define MIN_CORRECTION -100
#define MAX_CORRECTION 100

#define EEPROM_SIZE 10

SW_Memory::SW_Memory()
{ // Memory constructor
}

void SW_Memory::begin()
{
    this->displayMemoryDetails();
    EEPROM.begin(EEPROM_SIZE);
    Serial.println(F(">> Memory\t:enabled"));
}

void SW_Memory::displayMemoryDetails()
{
    Serial.printf("\n\nThe program uses %d kB and has free space of %d kB\n\n", ESP.getSketchSize() / 1000, ESP.getFreeSketchSpace() / 1000);
}

int SW_Memory::getRobotId()
{ // TODO: implement a variable as cache for reduce the number of memory read occurances
    return (int)this->read(EEPROM_ROBOT_ID);
}

int8_t SW_Memory::getErrorCorrection(uint8_t id)
{
    if (id == RIGHT)
    {
        return this->read(EEPROM_RIGHT_CORRECTION) - 128;
    }
    else if (id == LEFT)
    {
        return this->read(EEPROM_LEFT_CORRECTION) - 128;
    }
    return 0;
}

boolean SW_Memory::getMemoryStatus()
{
    return (this->read(EEPROM_PROGRAMMED) == 1);
}

uint8_t SW_Memory::read(int adr)
{
    return EEPROM.read(adr);
}
void SW_Memory::write(uint16_t address, uint8_t data)
{ // address: [0-511], data: [0-255]
    EEPROM.write(address, data);
    delay(200);
    EEPROM.commit();
}
void SW_Memory::test()
{ // A dummy Function for now
}

void SW_Memory::setupRobotWithId(uint8_t id, uint8_t leftCorrection, uint8_t rightCorrection)
{
    this->setRobotId(id);
    this->setErrorCorrection(LEFT, leftCorrection);
    this->setErrorCorrection(RIGHT, rightCorrection);
}

void SW_Memory::setRobotId(uint8_t id)
{
    if (id >= MIN_ROBOT_ID && id <= MAX_ROBOT_ID)
    {
        EEPROM.write(EEPROM_ROBOT_ID, id);
        delay(200);
        EEPROM.write(EEPROM_PROGRAMMED, 1);
        delay(200);
        EEPROM.commit();
        Serial.printf("Mem_Done\t: %d is written as Robot Id\n", id);
    }
    else
    {
        Serial.printf("Mem_Error\t: %d is an invalid Robot Id\n", id);
    }
}
void SW_Memory::setErrorCorrection(uint8_t id, int8_t val)
{
    if (id == RIGHT && (id >= MIN_CORRECTION && id <= MAX_CORRECTION))
    {
        this->write(EEPROM_RIGHT_CORRECTION, val + 128);
    }
    else if (id == LEFT && (id >= MIN_CORRECTION && id <= MAX_CORRECTION))
    {
        this->write(EEPROM_LEFT_CORRECTION, val + 128);
    }
    else
    {
        Serial.println("Mem_Error\t: Invalid parameters");
    }
}
