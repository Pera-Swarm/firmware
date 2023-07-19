#include "features.h"
#include "mqtt.h"

#include "modules/neopixel/neopixel.h"
#include "modules/gpio/gpio.h"
#include "algorithms/algorithm.h"

#ifdef ENABLE_MQTT

void mqtt_onMessageArrived(char *topic, byte *message, unsigned int length)
{

   // For message, convert byte array into a char array
   char msg[length + 1];
   msg[length] = '\0';
   for (int i = 0; i < length; i++)
   {
      msg[i] = (char)message[i];
   }

   // For topic, split by '/'
   char g[5][16] = {{0}};
   int j = 0, k = 0;
   for (int i = 0; topic[i] != '\0'; i++)
   {
      g[j][k++] = topic[i];
      if (topic[i] == '/')
      {
         g[j][k - 1] = '\0';
         // Serial.printf("%d,%d,%d - %s\n", i,j,k, g[j]);
         j++;
         k = 0;
      }
   }

   if (String(g[1]).equals("robot"))
   {

      // v1/robot/msg/
      if (String(g[2]).equals("msg"))
      {
         // v1/robot/msg/{id} -or-  v1/robot/msg/broadcast
         int num;
         sscanf(msg, "%s %d", g[3], &num);

         mqtt_robot_msg(g[3], &num);
      }
   }
   else if (String(g[1]).equals("sensor"))
   {
      if (String(g[2]).equals("distance"))
      {
         // v1/sensor/distance

         if (!String(g[3]).equals("") && atoi(g[3]) == mqtt_robot_id)
         {
            // v1/sensor/distance/{robotId}
            mqtt_distance_handle(msg);
         }
         else
         {
            Serial.println("distance sensor message");
            Serial.printf("\n>> topic:\t %s \n>> msg:\t\t %s\n", topic, msg);
         }
      }
      else if (String(g[2]).equals("color"))
      {
         // v1/sensor/color

         if (!String(g[3]).equals("") && atoi(g[3]) == mqtt_robot_id)
         {
            // v1/sensor/color/{robotId}
            mqtt_color_handle(msg);
         }
         else
         {
            Serial.println("color sensor message");
            Serial.printf("\n>> topic:\t %s \n>> msg:\t\t %s\n", topic, msg);
         }
      }
   }
   else if (String(g[1]).equals("comm"))
   {

      // v1/comm/in/simple/{robotId}
      // v1/comm/in/direct/{robotId}
      // Serial.println("communnication message");
      Serial.printf(">> topic:\t %s \n>> msg:\t\t %s\n", topic, msg);

      algorithm_interrupt(INT_COMM_IN, msg);
      // algorithm_execute(msg);
   }
   else if (String(g[1]).equals("output"))
   {
      // output/neopixel/{robotId}
      mqtt_neopixel_handle(msg);
   }
   else
   {
      Serial.println("other message");
      Serial.printf("\n>> topic:\t %s \n>> msg:\t\t %s\n", topic, msg);
   }

   // An indication about message arrival
   gpio.blinkLED(1, 200);
}

#endif
