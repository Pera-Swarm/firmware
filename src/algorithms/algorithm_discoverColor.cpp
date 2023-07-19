#include "algorithm.h"

#include "modules/neopixel/neopixel.h"
#include "modules/motors/motors.h"

#include "config/global_variables.h"
#include "mqtt/mqtt.h"

#ifdef ALGO_DISCOVER_COLOR
// -----------------------------------------------------------------------------

int robotState = ROBOT_BEGIN;
struct Color obsColor;
int hopId = -1;
boolean colorUpdated = false;
boolean searching = true;

void algorithm_setup()
{
    Serial.println("algorithm: setup");
    motors.stop();
}

void algorithm_loop()
{
    if (robotState == ROBOT_RUN)
    {
        algorithm_execute();
        // mqtt_delay(1000);
    }
    else if (robotState == ROBOT_BEGIN)
    {
        algorithm_setup();
        robotState = ROBOT_WAIT;
    }
    else
    {
        // wait
        mqtt_delay(100);
    }
}

void algorithm_interrupt(robot_interrupt_t interrupt, char *msg)
{
    if (interrupt == INT_COMM_IN)
    {
        // What to do if message received to comm/in/{robotId} ?
        Serial.printf("comm interrupt");

        int newhopId = 0, hopR = 0, hopG = 0, hopB = 0;
        int formatLen = sscanf(msg, "%d %d %d %d", &newhopId, &hopR, &hopG, &hopB);

        if (formatLen == 4)
        {
            printf("received %d len:%d \n", (colorUpdated == 1) ? 1 : 0, formatLen);

            if (colorUpdated)
            {
                // a reverse message, don't forward
                // pixelShowColor(0,0,0);
            }
            else
            {
                if (newhopId > hopId)
                {

                    pixelShowColor(hopR, hopG, hopB);

                    hopId = newhopId;
                    colorUpdated = true;
                    searching = false;
                    delay(1000);

                    // Send it to the next robot
                    // sprintf(tempString1, "%s/%s", CHANNEL,TOPIC_COMM_OUT_SIMPLE);
                    sprintf(tempString2, "%d %d %d %d", hopId + 1, hopR, hopG, hopB);
                    mqtt_comm_out_simple(tempString2);
                }
            }
        }
        else
        {
            Serial.printf("invalid msg format %d \n", formatLen);
        }
    }
}

// functional part of the pattern
void algorithm_execute()
{
    motors.stop();

    if (searching == true)
    {
        int d = distance_read();
        Serial.printf("algo_dist: %d\n", d);

        if (d < 20)
        {
            int random = (rand() % 100);
            int sign = (random % 2 == 0) ? 1 : -1;

            // Get color sensor reeding and show it in using the neopixel
            color_read(&obsColor);

            if (obsColor.R > 100)
            {
                // Red color detect
                robotState = ROBOT_WAIT;
                hopId++;
                pixelShowColor(255, 0, 0);
                motors.stop();
                sprintf(tempString2, "%d %d %d %d", hopId, 255, 0, 0);
                mqtt_comm_out_simple(tempString2);
            }
            else
            {
                Serial.printf("random: %d, sign: %d \n", random, sign);

                // Go back for 1 second
                motors.write(-150, -150);
                mqtt_delay(500);
                motors.stop();
                int loopCount = 0;

                while (distance_read() < 20 && loopCount < 5)
                {
                    // TODO: avoid infinity loop in here
                    Serial.println("rotate until dist < 50\n");
                    motors.write(75 * sign, -75 * sign);
                    mqtt_delay(500);
                    motors.stop();
                    loopCount++;
                }

                // turn for 2 more second
                motors.write(75 * sign, -75 * sign);
                mqtt_delay(2000);
                motors.stop();
            }
        }
        else
        {
            motors.write(150, 150);
            mqtt_delay(1000);
        }
    }
}

// instruct to start the pattern
void algorithm_start()
{
    robotState = ROBOT_RUN;
    Serial.println("algorithm: start");

    searching = true;
    hopId = 0;
    colorUpdated = false;
    pixelShowColor(0, 0, 0);
}

// reset the pattern variables and state
void algorithm_reset()
{
    robotState = ROBOT_BEGIN;
    Serial.println("algorithm: reset");

    searching = true;
    hopId = 0;
    colorUpdated = false;
    pixelShowColor(0, 0, 0);
}

// stop the execution of the pattern
void algorithm_stop()
{
    robotState = ROBOT_WAIT;
    Serial.println("algorithm: wait");
    motors.stop();
}

#endif
