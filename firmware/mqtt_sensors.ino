

int distance_read(){

    // Get physical sensor reading
    uint16_t dist_phy = distance.getDistanceInt();
    uint16_t dist_final;

    // Publish: sensor/distance
    //      { "id":0, "dist":0 }
    sprintf(tempString1, "%s/%s", CHANNEL,TOPIC_DISTANCE_REQ_TO_SERVER);
    sprintf(tempString2, "{\"id\":\"%d\",\"dist\":\"%d\"}", ROBOT_ID, dist_phy);

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
    return dist_final;
}
