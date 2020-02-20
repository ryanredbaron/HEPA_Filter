int MosfetControl = 4;

void setup() {
  pinMode(MosfetControl, OUTPUT);
  Serial.begin(9600);

  Serial.println("");
  Serial.println("Booting up....");
  for (int MosfetPwm = 100; MosfetPwm <= 500; MosfetPwm++) {
      analogWrite(MosfetControl, MosfetPwm);
      delay(10);
  }
  delay(5000);
  Serial.println("Boot up done. Starting program");
}

void loop() {

  for (int MosfetPwm = 255; MosfetPwm <= 1023; MosfetPwm++) {
    if (MosfetPwm % 255 == 0){
      analogWrite(MosfetControl, MosfetPwm);
      switch (MosfetPwm) {
        case 255:
          Serial.println("Sleep Mode");
          break;
        case 510:
          Serial.println("Low Mode");
          break;
        case 765:
          Serial.println("Medium Mode");
          break;
        case 1020:
          Serial.println("High Mode");
          break;
        default:
          Serial.println("You shouldn't be seeing this.....");
          break;
      }
      delay(10000);
      Serial.println("-------------------------");
    }
  }
}
