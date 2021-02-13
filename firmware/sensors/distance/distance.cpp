

#include "distance.h"

SW_Distance distance;

// Wrapper functions for the distance sensor

void beginDistance(){

}

uint16_t distance_hw_read(){
    return distance.getDistanceInt();
}
