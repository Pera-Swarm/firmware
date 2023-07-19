
#pragma once

#define CHANNEL "v1"

// ***************************************************************
// ROBOT
// ***************************************************************

// Subscribe: robot/msg/{robotId}
//      "{eNum} {integer}"
#define TOPIC_ROBOT_MSG "robot/msg/%d"

// Subscribe: robot/msg/broadcast
//      "{eNum} {value}"
#define TOPIC_ROBOT_BROADCAST "robot/msg/broadcast"

// Publish: robot/live
//      "{robotId} {reality}"
#define TOPIC_ROBOT_LIVE "robot/live"

// Publish: robot/create
//      "{"id":[robotId],"x":10,"y":10,"heading":0.0 }""
#define TOPIC_ROBOT_CREATE "robot/create"

// Publish: robot/delete
//      "{ "id":0 }"
#define TOPIC_ROBOT_DELETE "robot/delete"

// ***************************************************************
// DISTANCE SENSOR
// ***************************************************************

// Subscribe: sensor/distance/{robotId}/?
#define TOPIC_DISTANCE_REQ_FROM_SERVER "sensor/distance/%d/?"

// Publish: sensor/distance
//      { "id":0, "dist":0 }
#define TOPIC_DISTANCE_REQ_TO_SERVER "sensor/distance"

// Subscribe: {channal}/sensor/distance/{robotId}
//      {distance}
#define TOPIC_DISTANCE_RESP_FROM_SERVER "sensor/distance/%d"

// ***************************************************************
// COLOR SENSOR
// ***************************************************************

// Subscribe: sensor/color/{robotId}/?
#define TOPIC_COLOR_REQ_FROM_SERVER "sensor/color/%d/?"

// Publish: sensor/color
//      { "id":[robotID], "R":[R], "G":[G], "B":[B], "ambient":[ambient], "reality": "M" }
#define TOPIC_COLOR_REQ_TO_SERVER "sensor/color"

// Subscribe: {channal}/sensor/color/{robotId}
//      {R} {G} {B} {ambient}
#define TOPIC_COLOR_RESP_FROM_SERVER "sensor/color/%d"

// ***************************************************************
// VIRTUAL PROXIMITY SENSOR
// ***************************************************************

// Publish: sensor/proximity
//      { "id":[robotID], "reality": "M" }
#define TOPIC_PROXIMITY_REQ_TO_SERVER "sensor/proximity"

// Subscribe: {channal}/sensor/proximity/{robotId}
//      {-150} {-90} {0} {90} {150}
#define TOPIC_PROXIMITY_RESP_FROM_SERVER "sensor/proximity/%d"

// ***************************************************************
// LOCALIZATION
// ***************************************************************

// Publish: localization
//  {robotId}
#define TOPIC_LOCALIZATION_REQUEST "localization"

// Subscribe: localization/{robotId}
//  {X} {Y} {heading}
#define TOPIC_LOCALIZATION "localization/%d"

// ***************************************************************
// COMMUNICATION
// ***************************************************************

// Subscribe: comm/in/simple/{robotId}
// Subscribe: comm/in/direct/{robotId}
// {msg}
#define TOPIC_COMM_IN_SIMPLE "comm/in/simple/%d"
#define TOPIC_COMM_IN_DIRECT "comm/in/direct/%d"

// Publish: comm/out/{protocol}
//      { "id":{RobotId}, "msg": "This is a sample", "dist":[dist] }
#define TOPIC_COMM_OUT_SIMPLE "comm/out/simple"
#define TOPIC_COMM_OUT_DIRECT "comm/out/direct"

// ***************************************************************
// NEOPIXEL
// ***************************************************************

// output/neopixel/{robotId}
// {R} {G} {B}
#define TOPIC_NEOPIXEL_IN "output/neopixel/%d"

// output/neopixel/
// { "id":"0", "R":"0", "G":"0", "B":"0" }
#define TOPIC_NEOPIXEL_PUBLISH "output/neopixel"

// ***************************************************************
// OTHER
// ***************************************************************

// Publish: robot/log
#define TOPIC_LOG "robot/log"

// Subscribe: v1/robot/ota/{robotId}
//      "url"
#define TOPIC_OTA "robot/ota/%d"

// char TOPIC_OTA_DEVICE[22];
// sprintf(TOPIC_OTA_DEVICE, "v1/sensor/ota/%d", deviceId);
