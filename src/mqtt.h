
#define CHANNEL "v1/"

// ***************************************************************
// ROBOT
// ***************************************************************

// Subscribe: robot/msg/{robotId}
//      "{eNum} {integer}"
#define TOPIC_ROBOT_MSG "robot/msg/"

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
#define TOPIC_SENSOR_REQUEST "sensor/distance/"

// Publish: sensor/distance
//      { "id":0, "dist":0 }
#define TOPIC_SENSOR_REPLY "sensor/distance/"

// Subscribe: {channal}/sensor/distance/{robotId}
//      {distance}
#define TOPIC_SENSOR_PUSH "sensor/distance/"

// ***************************************************************
// LOCALIZATION
// ***************************************************************

// Subscribe: localization/{robotId}
//      {X} {Y} {heading}
#define TOPIC_LOCALIZATION "localization"

// ***************************************************************
// COMMUNICATION
// ***************************************************************

// Subscribe: comm/in/{robotId}
//
#define TOPIC_COMM_IN "comm/in/{robotId}"

// Publish: comm/out/{protocol}
//      { "id":{RobotId}, "msg": "This is a sample" }
#define TOPIC_COMM_OUT_SIMPLE "comm/out/simple"
#define TOPIC_COMM_OUT_DIRECTIONAL "comm/out/directional"

// ***************************************************************
// OTHER
// ***************************************************************

// Publish: robot/log
#define TOPIC_LOG "robot/log"

// Subscribe: v1/robot/ota/{robotId}
//      "url"
#define TOPIC_OTA "v1/robot/ota/"



// char TOPIC_OTA_DEVICE[22];
// sprintf(TOPIC_OTA_DEVICE, "v1/sensor/ota/%d", deviceId);
