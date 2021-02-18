#include "features.h"
#include "mqtt.h"
#include "sensors/distance/distance.h"
#include "config/global_variables.h"
#include "modules/display/display.h"

uint8_t dist_lock;
uint16_t dist_virt;

#ifdef ENABLE_MQTT

void mqtt_distance_handle(char* msg){
    //Serial.printf("distance update from server: %d -> %d \n", atoi(g[3]), atoi(msg));

    dist_lock = 0;
    dist_virt=atoi(msg);
}

int distance_read(){

    // Get physical sensor reading
    uint16_t dist_phy = 0;
    uint16_t dist_final;

    for(int i=0;i<3;i++){
        // obtain 3 reedings and get the average
        dist_phy += distance.getDistanceInt();
        delay(50);
    }
    dist_phy = (dist_phy/3)/10; // in cm

    // Publish: sensor/distance
    //      { "id":0, "dist":0 }
    sprintf(tempString1, "%s/%s", CHANNEL,TOPIC_DISTANCE_REQ_TO_SERVER);
    sprintf(tempString2, "{\"id\":\"%d\",\"dist\":\"%d\"}", mqtt_robot_id, dist_phy);

    dist_lock=1;
    dist_virt=-1;
    mqtt_publish(tempString1, tempString2, false);

    mqtt_wait(&dist_lock);

    if(dist_lock == 0){
        // have both sensor readings
        dist_final =  (dist_phy < dist_virt) ? dist_phy : dist_virt;
    }else{
        // only physical readings
        dist_final = dist_phy;
    }

    Serial.printf("Distance: phy: %d virt: %d | final: %d\n\n", dist_phy, dist_virt, dist_final);

    display_clear();
    display_print(0, dist_phy);
    display_print(1, dist_virt);

    return dist_final;
}


#endif
