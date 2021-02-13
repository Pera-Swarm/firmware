#pragma once

#if defined(DISTANCE_GP2Y0A21YK0F)
#include "robot_distance_GP2Y0A21YK0F.h"

#elif defined(DISTANCE_VL53LX0)
#include "robot_distance_VL53L0X.h"

#else
#include "robot_distance_disabled.h"

#endif

// Wrapper functions 
uint16_t distance_hw_read();
