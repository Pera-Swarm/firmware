#include "mqtt_proximity.h"

uint8_t proximity_lock;
struct Proximity proximity_virt;

#ifdef ENABLE_MQTT

void mqtt_proximity_handle(char *msg)
{
#ifdef ENABLE_VIRT_READINGS
   Serial.printf("proximity update from server received: %s n", msg);

   // TODO: read directly into proximity_virt
   uint8_t bl, l, f, r, br;
   int resp = sscanf(msg, "%u %u %u %u %u", (unsigned int *)&bl, (unsigned int *)&l, (unsigned int *)&f, (unsigned int *)&r, (unsigned int *)&br);

   if (resp == 5)
   {
      proximity_lock = 0;
      proximity_virt.BackLeft = bl;
      proximity_virt.Left = l;
      proximity_virt.Front = f;
      proximity_virt.Right = r;
      proximity_virt.BackRight = br;
   }
   else
   {
      // TODO: 0 or infinity ?
      proximity_virt.BackLeft = 0;
      proximity_virt.Left = 0;
      proximity_virt.Front = 0;
      proximity_virt.Right = 0;
      proximity_virt.BackRight = 0;
      Serial.print(F("Error_ProximityResponseArgs"));
   }
#endif
}

void proximity_read(proximity_t *proximity)
{
#ifdef ENABLE_VIRT_READINGS

   // Publish: sensor/proximity
   //      { "id":0, "dist":0 }
   sprintf(tempString1, "%s/%s", CHANNEL, TOPIC_PROXIMITY_REQ_TO_SERVER);
   sprintf(tempString2, "{\"id\":\"%d\", \"reality\":\"M\"}", mqtt_robot_id);

   proximity_lock = 1;

   mqtt_publish(tempString1, tempString2, false);
   mqtt_wait(&dist_lock);

   // if(proximity_lock == 0){
   //    // have a reply
   // }else{
   //    // no reply
   // }

   Serial.printf("Proximity: %u %u %u %u %u\n\n", proximity_virt.BackLeft, proximity_virt.Left, proximity_virt.Front, proximity_virt.Right, proximity_virt.BackRight);

   // display_clear();
   // display_print(0, dist_phy);
   // display_print(1, dist_virt);

#else
   // Sensor is not available, return value is 65535
   proximity_virt.BackLeft = -1;
   proximity_virt.Left = -1;
   proximity_virt.Front = -1;
   proximity_virt.Right = -1;
   proximity_virt.BackRight = -1;

#endif
}

#endif
