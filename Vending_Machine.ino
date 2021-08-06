/* 3 April 2021 -----
  -- Vending Machine --
  4x tipe masker, 4x tombol IR, 4x monitoring masker,
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int ledState = LOW;
int btnA, btnB, btnC, btnD;
int cekA, cekB, cekC, cekD;

const int motor1 = 4;
const int motor2 = 5;
const int motor3 = 6;
const int motor4 = 7;

int btnpin[] = {8, 9, 10, 11};  // pin

int cekpin[] = {A0, A1, A2, A3};

const int nodem = 13;
const int nodemrst = 2;
unsigned long previousMillis;
const long interval = 5000;
unsigned long currentMillis;
int step_step;
bool ambil;
void setup() {
  for (int thisPin = 0; thisPin < 4; thisPin++) {
    pinMode(btnpin[thisPin], INPUT);
    pinMode(cekpin[thisPin], INPUT);
  }
  pinMode(nodemrst, OUTPUT);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motor3, OUTPUT);
  pinMode(motor4, OUTPUT);
  pinMode(nodem, OUTPUT);
  digitalWrite(nodemrst, HIGH);
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, HIGH);
  digitalWrite(motor3, HIGH);
  digitalWrite(motor4, HIGH);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  lcd.setCursor(0, 0); lcd.print("Vending Machine");
  lcd.setCursor(0, 1); lcd.print("     Masker    ");
  delay(2000);
  digitalWrite(nodemrst, LOW);
  delay(1000);
  lcd.clear();
}

void loop() {
  digitalWrite(nodemrst, HIGH);
  currentMillis = millis();
  bacaBtn();
  cekBtn();
  ambilMasker();
}

void bacaBtn () {
  btnA = digitalRead(btnpin[0]);
  btnB = digitalRead(btnpin[1]);
  btnC = digitalRead(btnpin[2]);
  btnD = digitalRead(btnpin[3]);
}

void cekBtn () {
  cekA = digitalRead(cekpin[0]);
  cekB = digitalRead(cekpin[1]);
  cekC = digitalRead(cekpin[2]);
  cekD = digitalRead(cekpin[3]);
}

void ambilMasker() {
  bacaBtn();
  cekBtn();
  switch (step_step) {
    case 0:
      Serial.println("Scanning : ");
      lcd.setCursor(0, 0); lcd.print(" Vending Masker ");
      lcd.setCursor(0, 1); lcd.print("     Standby    ");
      if (!btnA) step_step = 10;
      else if (!btnB) step_step = 20;
      else if (!btnC) step_step = 30;
      else if (!btnD) step_step = 40;
      break;
    // ----------------------- NO 1 ------------------
    case 10:
      mengambilMasker(motor1);
      if (!cekA) step_step += 1;
      break;
    case 11:
      maskerDiambil(motor1);
      step_step = 0;
      break;
    // ----------------------- NO 2 ------------------
    case 20:
      mengambilMasker(motor2);
      if (!cekB) step_step += 1;
      break;
    case 21:
      maskerDiambil(motor2);
      step_step = 0;
      break;
    // ----------------------- NO 3 ------------------
    case 30:
      mengambilMasker(motor3);
      if (!cekC) step_step += 1;
      break;
    case 31:
      maskerDiambil(motor3);
      step_step = 0;
      break;
    // ----------------------- NO 4 ------------------
    case 40:
      mengambilMasker(motor4);
      if (!cekD) step_step += 1;
      break;
    case 41:
      maskerDiambil(motor4);
      step_step = 0;
      break;
  }
}

void mengambilMasker(int pin) {
  lcd.setCursor(0, 0); lcd.print("Mengambil Masker");
  lcd.setCursor(0, 1); lcd.print(" -------------- ");
  Serial.println(String() + "Mengambil Masker");
  digitalWrite(pin, LOW);
}

void maskerDiambil(int pin) {
  digitalWrite(pin, HIGH);
  Serial.println(String() + "Masker Berhasil di ambil");
  lcd.setCursor(0, 0); lcd.print(" Masker berhasil");
  lcd.setCursor(0, 1); lcd.print(" diambil        ");
  digitalWrite(nodem, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("   Mengunggu   ");
  lcd.setCursor(0, 1); lcd.print("       5       ");
  delay(1000);
  lcd.setCursor(0, 1); lcd.print("       4       ");
  delay(1000);
  lcd.setCursor(0, 1); lcd.print("       3       ");
  delay(1000);
  digitalWrite(nodem, LOW);
  lcd.setCursor(0, 1); lcd.print("       2       ");
  delay(1000);
  lcd.setCursor(0, 1); lcd.print("       1       ");
  delay(1000);
}
