#include "mqtt_robot.h"

#ifdef ENABLE_MQTT

void mqtt_robot_live()
{
    sprintf(tempString1, "%s/%s", CHANNEL, TOPIC_ROBOT_LIVE);
    sprintf(tempString2, "{\"id\": %d,\"reality\":\"R\"}", mqtt_robot_id);
    mqtt_publish(tempString1, tempString2, false);
}

void mqtt_robot_msg(char *msg, int *value)
{
    printf("msg: %s  val:%d\n", msg, *value);

    if (String(msg).equals("ID?"))
    {
        mqtt_robot_live();
    }
    else if (String(msg).equals("START"))
    {
        // Things to do when start action
        algorithm_start();
    }
    else if (String(msg).equals("STOP"))
    {
        // Things to do when stop action
        algorithm_stop();
    }
    else if (String(msg).equals("RESET"))
    {
        // Things to do when reset action
        algorithm_reset();
    }
    else if (String(msg).equals("MODE"))
    {
        // Things to do when mode action
    }
    else
    {
    }
}

#endif
