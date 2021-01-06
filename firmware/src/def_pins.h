

// Move Directions
#define LEFT 4
#define RIGHT 6
#define FORWARD 8
#define BACKWARD 2

// Rotation Directions
#define CW 0
#define CCW 1

// GPIO Pins
#define PIN_LED 2
#define PIN_SW_0 19
#define PIN_SW_1 18

// Motors
#define PIN_SERVO_LEFT 12
#define PIN_SERVO_RIGHT 13

#define PIN_MOT_A 25
#define PIN_MOT_B 26

#define PIN_PWM_A 12
#define PIN_PWM_B 13

#define PIN_ENCODER_L 5
#define PIN_ENCODER_R 17

// Neopixel LED Ring
#define PIN_NEOPIXEL_LED 23

// Distance Sensor
#define PIN_DIST_SENSOR 32

// Color Sensor
// This is due to address conflict with VL53L0X, while configuring it, the color sensor should be disabled
//#define PIN_ENABLE_COLOR_SENSOR 27

// IR communication
#define PIN_IR_TX 16

#define PIN_IR_RX_0 39
#define PIN_IR_RX_1 36
#define PIN_IR_RX_2 34
#define PIN_IR_RX_3 33

// extra port I2C address
#define EXTRA_PORT_ADDRESS 0x27


// Unused or reserved pins
#define PIN_TX 1
#define PIN_RX 3

#define PIN_SCL 22
#define PIN_SDA 21

#define PIN_BATTERY_SENSE 35
