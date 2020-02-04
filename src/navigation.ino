
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
