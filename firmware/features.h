

#define ENABLE_SERIAL_COMMUNICATION 1     // Enable or disable serial monitor for libraries
#define ENABLE_MEMORY

#define ENABLE_MOTORS

// -----------------------------------
#if defined(ENABLE_MOTORS)

//#define DRIVE_SERVO
#define DRIVE_PWM

#define WHEEL_ENCODER

#endif
// -----------------------------------

#define ENABLE_NEOPIXEL_RING              // Enable NeoPixel Ring


#define ENABLE_DISTANCE_SENSOR            // Enable VL53LX0 ToF Sensor (required xkB space)
// #define DISTANCE_GP2Y0A21YK0F          // Enable GP2Y0A21YK0F analog distance sensor, GP2Y0A21YK0F (required 2kB space)
#define DISTANCE_VL53LX0


//#define ENABLE_COLOR_SENSOR               // Enable TCS34725 Color sensor (required 2kB space)
//#define ENABLE_COMPASS_SENSOR             // Enable GY-511 compass + accelerometer (required 3kB space)

//#define ENABLE_EXTERNAL_PORT              // Enable PCF8564 I2C port expander (required 1kB space)

//#define ENABLE_INFARED                    // Enable IR Transmiter and receiver (required 1kB space)


//#define ENABLE_OTA_UPLOAD                 // Enable or disable OTA (On the air upload via WiFi, required 531kB space)
//#define ENABLE_WIFI_MONITOR               // Enable Wifi Monitor
//#define ENABLE_WIFI_CLIENT


// On testing
// #define ENABLE_ESPNOW                     // Enable ESP Now protocol (No WiFi allowed in this mode)

#define ENABLE_MQTT                       // MQTT protocol for communication
#define ENABLE_WIFI

/*
I2C Map

Compass     25 (0x19)
Compass     30 (0x1E)
PCF8574     39 (0x27)
VLX53L0X    42 (0x2A)
Color       41 (0x29)

*/


// Dependecy check for motors
#if (defined(ENABLE_MOTORS) && !(defined(DRIVE_SERVO) || defined(DRIVE_PWM)))
#error "DEPENDENCY ERROR: Need to be defined either DRIVE_SERVO or DRIVE_PWM in features.h"
#endif

// Dependecy check for distance sensor
#if (defined(ENABLE_DISTANCE_SENSOR) && !(defined(DISTANCE_GP2Y0A21YK0F) || defined(DISTANCE_VL53LX0)))
#error "DEPENDENCY ERROR: Need to enable either GP2Y0A21YK0F or VL53LX0 sensor in features.h"
#endif
#if (defined(ENABLE_DISTANCE_SENSOR) && defined(DISTANCE_GP2Y0A21YK0F) && defined(DISTANCE_VL53LX0))
#error "DEPENDENCY ERROR: You can't enable both GP2Y0A21YK0F and VL53LX0 sensors at once in features.h"
#endif

// Dependency check for MQTT
#if (defined(ENABLE_MQTT) && !(defined(ENABLE_WIFI)))
#error "DEPENDENCY ERROR: Need to enable WiFi by uncommenting #define ENABLE_WIFI in features.h to enable the MQTT"
#endif
