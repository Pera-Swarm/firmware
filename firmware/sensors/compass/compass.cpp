#include "compass.h"

SW_LSM303::vector<int16_t> running_min = {-524,   -547,   -407};
SW_LSM303::vector<int16_t> running_max = { +328,   +327,   +282};

SW_LSM303 compass(running_min, running_max);
