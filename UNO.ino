#include <Servo.h>
Servo myservo1;
Servo myservo2;

int BUTTON_PIN_RED = 9;
int BUTTON_PIN_BLUE = 8;

void setup() {
  Serial.begin(9600);  //シリアル通信

  pinMode(BUTTON_PIN_RED, INPUT_PULLUP);
  pinMode(BUTTON_PIN_BLUE, INPUT_PULLUP);

  myservo1.attach(6);  //red
  myservo2.attach(5);  //blue
  myservo1.write(0);
  myservo2.write(45);
}

int s;  //押されたボタンの値

void loop() {
  while (Serial.available()) {
    byte Sum = (byte)Serial.read();
    if (Sum >= 170) {  //投入金額が170円以上のとき
      if (digitalRead(BUTTON_PIN_RED) == LOW) {
        s = 1;  //red
        myservo1.write(0);
        delay(1000);
        myservo1.write(45);
        delay(1000);
        myservo1.write(0);
        delay(1000);
      } else if (digitalRead(BUTTON_PIN_BLUE) == LOW) {
        s = 2;  //blue
        myservo2.write(45);
        delay(1000);
        myservo2.write(0);
        delay(1000);
        myservo2.write(45);
        delay(1000);
      }
    } else if (Sum >= 130) {  //投入金額が130円以上のとき
      if (digitalRead(BUTTON_PIN_BLUE) == LOW) {
        s = 2;  //blue
        myservo2.write(45);
        delay(1000);
        myservo2.write(0);
        delay(1000);
        myservo2.write(45);
        delay(1000);
      }
    } else {
      s = -1;
    }
  }
  Serial.write(s);
  delay(300);
}
