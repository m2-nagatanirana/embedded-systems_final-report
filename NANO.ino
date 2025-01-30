#include <Servo.h>
Servo myservo1; 
Servo myservo2;
Servo myservo3;

int red = 130; //blueの料金設定
int blue = 170; //redの料金設定

int temt6000Pin1 = 4;  //500yen
int temt6000Pin2 = 6;  //100yen
int temt6000Pin3 = 7;  //50yen
int temt6000Pin4 = 5;  //10yen

const int LEDpin1 = 14;
const int LEDpin2 = 15;
const int LEDpin3 = 16;
const int LEDpin4 = 17;

int coinCount1 = -1;
int coinCount2 = -1;
int coinCount3 = -1;
int coinCount4 = -1;

int tDelay = 1;    // 1ms遅延設定
int rclkPin = 2;   // RCLK (74HC595)
int srclkPin = 3;  // SRCLK (74HC595)
int dsPin = 4;     // SER (74HC595)
int dig1 = 8;      // DIG1を8番ピンに割り当て
int dig2 = 7;      // DIG2を7番ピンに割り当て
int dig3 = 6;      // DIG3を6番ピンに割り当て
int dig4 = 5;      // DIG4を5番ピンに割り当て
int a,b,c,d; //7セグメントLEDの各桁の数字

int preState = HIGH;  // 最初はトグルスイッチの状態をHIGHだと仮定
int curState = HIGH;   // 現在のスイッチ状態
bool act = false;

int t; //モータの回転数

// seven_ledsをbyte型として定義
// 1 = LED on, 0 = LED off
byte seven_leds[11] = {
  0b01111110,  //0
  0b00110000,  //1
  0b01101101,  //2
  0b01111001,  //3
  0b00110011,  //4
  0b01011011,  //5
  0b01011111,  //6
  0b01110010,  //7
  0b01111111,  //8
  0b01111011,  //9
  B00000000    // OFF
};

//シフトレジスタ部分を関数化
void funcShiftReg(int x) {
  digitalWrite(rclkPin, LOW);                          //送信中のRCLKをLowにする
  shiftOut(dsPin, srclkPin, LSBFIRST, seven_leds[x]);  //シフト演算を使って点灯するLEDを選択
  digitalWrite(rclkPin, HIGH);                         //送信終了後RCLKをHighにする
}

void displaySumOn7Segment(int sum) { //合計金額を７セグメントLEDで表示する
    if (sum > 0) {
    d = 0;
    int s = sum / 10;
    if (s < 10) {
      c = s;
    } else if (s >= 10 && s <= 99) {
      b = s / 10;
      c = s - b * 10;
    } else if (s >= 100) {
      a = s / 100;
      b = (s - a * 100) / 10;
      c = s - (a * 100 + b * 10);
    }
  } else {
    a = 10;
    b = 10;
    c = 10;
    d = 0;
  }

  //DIG1の表示
  delay(tDelay);
  digitalWrite(dig1, LOW);
  digitalWrite(dig2, HIGH);
  digitalWrite(dig3, HIGH);
  digitalWrite(dig4, HIGH);
  //DIG1に任意の数字aを表示
  funcShiftReg(a);
  delay(tDelay);
  funcShiftReg(11);
  //DIG2の表示
  delay(tDelay);
  digitalWrite(dig1, HIGH);
  digitalWrite(dig2, LOW);
  digitalWrite(dig3, HIGH);
  digitalWrite(dig4, HIGH);
  //DIG2に任意の数字bを表示
  funcShiftReg(b);
  delay(tDelay);
  funcShiftReg(11);
  //DIG3の表示
  delay(tDelay);
  digitalWrite(dig1, HIGH);
  digitalWrite(dig2, HIGH);
  digitalWrite(dig3, LOW);
  digitalWrite(dig4, HIGH);
  //DIG3に任意の数字cを表示
  funcShiftReg(c);
  delay(tDelay);
  funcShiftReg(11);
  //DIG4の表示
  delay(tDelay);
  digitalWrite(dig1, HIGH);
  digitalWrite(dig2, HIGH);
  digitalWrite(dig3, HIGH);
  digitalWrite(dig4, LOW);
  //DIG4に任意の数字dを表示
  funcShiftReg(d);
  delay(tDelay);
  funcShiftReg(11);
}

void setup() {
  Serial.begin(9600);  //シリアル通信
   
  //LED(お金のカウント用)
  pinMode(LEDpin1, OUTPUT);
  pinMode(LEDpin2, OUTPUT);
  pinMode(LEDpin3, OUTPUT);
  pinMode(LEDpin4, OUTPUT);

  //７セグメントLED
  pinMode(rclkPin, OUTPUT);   
  pinMode(dsPin, OUTPUT);     
  pinMode(srclkPin, OUTPUT);  
  pinMode(dig1, OUTPUT);      
  pinMode(dig2, OUTPUT);      
  pinMode(dig3, OUTPUT);      
  pinMode(dig4, OUTPUT);      
  digitalWrite(dig1, HIGH);   
  digitalWrite(dig2, HIGH);   
  digitalWrite(dig3, HIGH);   
  digitalWrite(dig4, HIGH);   
  funcShiftReg(11); //信号初期化

  //お釣りを出すモータ
  myservo1.attach(9);   //10yen
  myservo2.attach(11);  //100yen
  myservo3.attach(12);  //50yen

  pinMode(13, INPUT_PULLUP); //返金スイッチ
}

void loop() {

  int curState = digitalRead(13); //返金スイッチの状態

  //光センサ(お金のカウント用)
  int value1 = analogRead(temt6000Pin1);  //500yen
  int value2 = analogRead(temt6000Pin2);  //100yen
  int value3 = analogRead(temt6000Pin3);  //50yen
  int value4 = analogRead(temt6000Pin4);  //10yen

  //お金の計算
  int sum = 0;  //全部の合計
  int sum1 = 0;    //500円の合計
  int sum2 = 0;    //100円の合計
  int sum3 = 0;    //50円の合計
  int sum4 = 0;    //10円の合計

  //LEDの点灯
  digitalWrite(LEDpin1, HIGH);
  digitalWrite(LEDpin2, HIGH);
  digitalWrite(LEDpin3, HIGH);
  digitalWrite(LEDpin4, HIGH);

  //通過した回数のカウント，金額の計算
  if (value1 < 150) {  //500yen
    delay(300);
    coinCount1++;
    sum1 = 500 * coinCount1;
  } else {
  }
  if (value2 < 150) {  //100yen
    delay(700);
    coinCount2++;
    sum2 = 100 * coinCount2;
  } else {
  }
  if (value3 < 150) {  //50yen
    delay(1000);
    coinCount3++;
    sum3 = 50 * coinCount3;
  } else {
  }
  if (value4 < 150) {  //10yen
    delay(300);
    coinCount4++;
    sum4 = 10 * coinCount4;
  } else {
  }

  //合計金額
  sum= 500 * coinCount1 + 100 * coinCount2 + 50 * coinCount3 + 10 * coinCount4;
  displaySumOn7Segment(sum);

  while (Serial.available()) {
    byte inChar = (byte)Serial.read();
    if (inChar == 1) { //redのスイッチが押されたとき
      int oturi1 = sum - 170;
      int hyaku1 = oturi1 / 100;
      int gojyu1 = (oturi1 - (100 * hyaku1)) / 50;
      int jyu1 = (oturi1 - ((100 * hyaku1) + 50 * gojyu1)) / 10;

      t = 1;
      if (jyu1 > 0) {
        while (t <= jyu1) {
          myservo1.write(40);
          delay(1000); 
          myservo1.write(90);
          delay(2000);
          t++;
        }
      }
      t = 1;
      if (gojyu1 > 0) {
        while (t <= gojyu1) {
          myservo3.write(15);
          delay(700);  
          myservo3.write(90);
          delay(2000);
          t++;
        }
      }
      t = 1;
      if (hyaku1 > 0) {
        while (t <= hyaku1) {
          myservo2.write(60);
          delay(900); 
          myservo2.write(90);
          delay(2000);
          t++;
        }
      }
      sum= 0;  // sumを0にリセット(７セグメントLEDの表示を0にする)
      coinCount1 = 0;
      coinCount2 = 0;
      coinCount3 = 0;
      coinCount4 = 0;
    }
    else if (inChar == 2) { //blueのスイッチが押されたとき
      int oturi2 = sum - 130;
      int hyaku2 = oturi2 / 100;
      int gojyu2 = (oturi2 - (100 * hyaku2)) / 50;
      int jyu2 = (oturi2 - ((100 * hyaku2) + 50 * gojyu2)) / 10;

      t = 1;
      if (jyu2 > 0) {
        while (t <= jyu2) {
          myservo1.write(40);
          delay(1000);  
          myservo1.write(90);
          delay(2000);
          t++;
        }
      }
      t = 1;
      if (gojyu2 > 0) {
        while (t <= gojyu2) {
          myservo3.write(15);
          delay(700);  
          myservo3.write(90);
          delay(2000);
          t++;
        }
      }
      t = 1;
      if (hyaku2 > 0) {
        while (t <= hyaku2) {
          myservo2.write(60);
          delay(900);  
          myservo2.write(90);
          delay(2000);
          t++;
        }
      }
      sum = 0;  // sumを0にリセット(７セグメントLEDの表示を0にする)
      coinCount1 = 0;
      coinCount2 = 0;
      coinCount3 = 0;
      coinCount4 = 0;
    }
  }

  if (curState == LOW && preState == HIGH && !act) {  //そのまま返金
    act = true;
    int oturi3 = sum;
    int hyaku3 = oturi3 / 100;
    int gojyu3 = (oturi3 - (100 * hyaku3)) / 50;
    int jyu3 = (oturi3 - ((100 * hyaku3) + 50 * gojyu3)) / 10;

    t = 1;
    if (jyu3 > 0) {
      while (t <= jyu3) {
        myservo1.write(40);
        delay(1000); 
        myservo1.write(90);
        delay(2000);
        t++;
      }
    }
    t = 1;
    if (gojyu3 > 0) {
      while (t <= gojyu3) {
        myservo3.write(15);
        delay(700); 
        myservo3.write(90);
        delay(2000);
        t++;
      }
    }
    t = 1;
    if (hyaku3 > 0) {
      while (t <= hyaku3) {
        myservo2.write(60);
        delay(900);  
        myservo2.write(90);
        delay(2000);
        t++;
      }
    }
    sum= 0;  // sumを0にリセット(７セグメントLEDの表示を0にする)
    coinCount1 = 0;
    coinCount2 = 0;
    coinCount3 = 0;
    coinCount4 = 0;
  }

  if (curState == HIGH && preState == LOW) {
    act = false;  // 再度LOWになったときに処理が実行されるようにする
  }
  preState = curState;
}
