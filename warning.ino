#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial sim(9, 10);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int flameSensor = 12;
const int gasSensor = 11 ;
const int relay = 3;
const int fan = 4;

bool called = false;
bool sent = false;
bool isClear = true;

String phoneNum = "0376560548";

void setup() {
  Serial.begin(9600);
  sim.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Xin chao !");

  pinMode(flameSensor, INPUT);
  pinMode(gasSensor, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(fan, OUTPUT);

  digitalWrite(relay, 0);
  digitalWrite(fan, 0);
}
void loop() {
  if (digitalRead(flameSensor) == 0) {
    isClear = true;
    if (!called) {
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Canh bao !");
      lcd.setCursor(4, 1);
      lcd.print("Co chay!");
      digitalWrite(relay, 1);
      digitalWrite(fan, 0);
      call();
      called = true;
    }
  } else if (digitalRead(gasSensor) == 0) {
    isClear = true;
    if (!sent) {
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Canh bao !");
      lcd.setCursor(2, 1);
      lcd.print("Co khi gas !");
      digitalWrite(fan, 1);
      digitalWrite(relay, 0);
      send();
      sent = true;
    }
  } else {
    if (isClear) {
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Xin chao !");
      sent = false;
      called = false;
      isClear = false;
    }
    digitalWrite(relay, 0);
    digitalWrite(fan, 0);
  }
}

void send() {
  Serial.println("Sending...");
  String SMS = "Co chay o phong 1";

  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + phoneNum + "\"\r");
  delay(1000);
  sim.println(SMS);
  delay(100);
  sim.println((char)26);
  delay(1000);
}
void call() {
  Serial.println("Calling...");
  sim.println("AT");
  delay(300);
  sim.print(F("ATD"));
  sim.print(phoneNum);
  sim.print(F(";\r\n"));
  delay(20000);
  sim.print(F("ATH;\r\n"));
}