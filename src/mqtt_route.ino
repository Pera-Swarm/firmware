void mqtt_onMessageArrived(char* topic, byte* message, unsigned int length) {

    // For message, convert byte array into a char array
    char msg[length+1];
    msg[length] = '\0';
    for (int i = 0; i < length; i++) {
        msg[i] =  (char)message[i];
    }

    // For topic, split by '/'
    char g[5][16] = {{0}};
    int j=0, k=0;
    for (int i = 0; topic[i] != '\0'; i++) {
        g[j][k++] = topic[i];
        if(topic[i]== '/'){
            g[j][k-1] = '\0';
            //Serial.printf("%d,%d,%d - %s\n", i,j,k, g[j]);
            j++;k=0;
        }
    }
    Serial.printf("\n>> topic:\t %s \n>> msg:\t\t %s\n", topic, msg);

    if(String(g[1]).equals("robot")){

        // v1/robot/msg/0
        if(String(g[2]).equals("msg")){
            // v1/robot/msg/{id} -or-  v1/robot/msg/broadcast
            int num;
            sscanf(msg, "%s %d",  g[3], &num);
            printf("msg: %s  val:%d\n", g[3], num);
        }

    } else if(String(g[1]).equals("comm")){
        Serial.println("communnication message");
        // v1/comm/in/0



    }else{
        Serial.println("other message");
    }
    /*
    scanf("%d %d", &x &y);
    */
}
