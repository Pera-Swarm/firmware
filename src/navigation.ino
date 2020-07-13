
void followPath(String path) {

   char m;
   for (int i = 0; i < path.length(); ++i){
      m = path.charAt(i);
      Serial.printf("Executing the instruction %d => %c \n",  i, m);

      if (m == 'F') {
         //encoderForward(5);
      }
      else if (m == 'R') {
         //turnRight();
      }
      else if (m == 'L') {
         //turnLeft();
      }
      else {
         //stop;
      }
      delay(500);
   }
}


void encoderMove(){
   motors.encoderReset();
   motors.write(100,100);

   while(motors.encoderAverage()<50){
      motors.encoderPrint();
   }
   motors.stop();
   delay(1000);

   motors.encoderReset();
   motors.write(-100,-100);

   while(motors.encoderAverage()<50){
      motors.encoderPrint();
   }
   motors.stop();
   delay(1000);

}
