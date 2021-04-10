#pragma once

#if defined(DISTANCE_GP2Y0A21YK0F)
#include "./src/robot_distance_GP2Y0A21YK0F.h"

#elif defined(DISTANCE_VL53LX0)
#include "./src/robot_distance_VL53L0X.h"

#else
#include "./src/robot_distance_disabled.h"

#endif

extern SW_Distance distance;
