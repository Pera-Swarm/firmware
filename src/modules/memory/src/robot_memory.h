/**
 * @brief    Swarm Robot Memory Manager Library
 * @author   Nuwan Jaliyagoda, Pasan Tennakoon, Dilshani Karunarathna
 * @version  1.0.0
 * @url      N/A
 *
 * ------------------------------------------------------------------------------
 */

#ifndef SW_Memory_h
#define SW_Memory_h

#include <Arduino.h>

class SW_Memory
{
public:
    SW_Memory();

    int getRobotId();
    int8_t getErrorCorrection(uint8_t id);

    void begin();
    void test();

    void setupRobotWithId(uint8_t id, uint8_t leftCorrection, uint8_t rightCorrection);

    void setRobotId(uint8_t id);
    void setErrorCorrection(uint8_t id, int8_t val);

    boolean getMemoryStatus();

    // REM: Better to keep following 2 private
    uint8_t read(int adr);
    void write(uint16_t address, uint8_t data);

    enum addr
    {
        EEPROM_PROGRAMMED,
        EEPROM_ROBOT_ID,
        EEPROM_LAST_STATUS,
        EEPROM_RIGHT_CORRECTION,
        EEPROM_LEFT_CORRECTION
    };

private:
    void displayMemoryDetails();
};

#endif
