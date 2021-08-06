
#include "Arduino.h"
#include <EMailSender.h>
#include <ESP8266WiFi.h>

const char* ssid = "prokes";
const char* password = "jagaselalukesehatan";
int nyampak;
uint8_t connection_state = 0;
uint16_t reconnect_interval = 10000;

const int trig_pin_1 = 5;  // D1
const int echo_pin_1 = 16;   // D0
const int trig_pin_2 = 4;   // D2
const int echo_pin_2 = A0;   // D3
const int trig_pin_3 = 14;  // D5
const int echo_pin_3 = 12;  // D6
const int trig_pin_4 = 13;  // D7
const int echo_pin_4 = 15;  // D8
const int uno = A0;     // A0
bool konds1[] = {true, true, true, true, true};
bool konds2[] = {true, true, true, true, true};
bool konds3[] = {true, true, true, true, true};
bool konds4[] = {true, true, true, true, true};


int distance1, distance2, distance3, distance4;

#define emailFrom "taproject82@gmail.com"
#define passwordFrom "pembersihudara"
//#define emailTo "zafierfazamuhammad@gmail.com"
#define emailTo "srirezkii86@gmail.com"
EMailSender emailSend(emailFrom, passwordFrom);


uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr) {
  static uint16_t attempt = 0;
  Serial.print("Connecting to ");
  if (nSSID) {
    WiFi.begin(nSSID, nPassword);
    Serial.println(nSSID);
  }

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 50)
  {
    delay(200);
    Serial.print(".");
  }
  ++attempt;
  Serial.println("");
  if (i == 51) {
    Serial.print("Connection: TIMEOUT on attempt: ");
    Serial.println(attempt);
    if (attempt % 2 == 0)
      Serial.println("Check if access point available or SSID and Password\r\n");
    return false;
  }
  Serial.println("Connection: ESTABLISHED");
  Serial.print("Got IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

void Awaits() {
  uint32_t ts = millis();
  while (!connection_state)
  {
    delay(50);
    if (millis() > (ts + reconnect_interval) && !connection_state) {
      connection_state = WiFiConnect();
      ts = millis();
    }
  }
}

void setup() {
  pinMode(uno, INPUT);
  pinMode(trig_pin_1, OUTPUT);
  pinMode(echo_pin_1, INPUT);
  //  pinMode(trig_pin_2, OUTPUT);
  //  pinMode(echo_pin_2, INPUT);
  pinMode(trig_pin_3, OUTPUT);
  pinMode(echo_pin_3, INPUT);
  pinMode(trig_pin_4, OUTPUT);
  pinMode(echo_pin_4, INPUT);

  Serial.begin(115200);
  connection_state = WiFiConnect(ssid, password);
  if (!connection_state) // if not connected to WIFI
    Awaits();          // constantly trying to connect

}

void loop() {
  // max masker habis = 42 cm
  // max masker penuh = 4 cm
  nyampak = analogRead(uno);
  Serial.println(String() + "Uno = " + nyampak);
  if (nyampak > 1000) {
    kirimEmail(5, "Diambil");
  }
  distance2 = 70;
  sonic_1();
  //  sonic_2();
  sonic_3();
  sonic_4();
  checkoutEmail(konds1, 1, distance1);
  checkoutEmail(konds2, 2, distance2);
  checkoutEmail(konds3, 3, distance3);
  checkoutEmail(konds4, 4, distance4);
  serialPrint();
  //  delay(500);
}

void serialPrint() {
  Serial.print(String() + "Distance 1 = " + distance1 + "% | ");
  Serial.print(String() + "Distance 2 = " + distance2 + "% | ");
  Serial.print(String() + "Distance 3 = " + distance3 + "% | ");
  Serial.println(String() + "Distance 4 = " + distance4 + "% | ");
  Serial.print(String() + "Kondisi 1 = " + masker(distance1) + " | ");
  Serial.print(String() + "Kondisi 2 = " + masker(distance2) + " | ");
  Serial.print(String() + "Kondisi 3 = " + masker(distance3) + " | ");
  Serial.println(String() + "Kondisi 4 = " + masker(distance4) + " | ");
}

String masker(int sensor) {
  String masker;
  if (sensor == 0 ) masker = "Habis";
  else if (sensor > 0 && sensor <= 30) masker = "Sedikit";
  else if (sensor > 30 && sensor <= 60) masker = "Setengah";
  else if (sensor > 60 && sensor <= 90) masker = "Terisi";
  else if (sensor > 90 && sensor <= 100) masker = "Penuh";
  return masker;
}

void sonic_1() {
  digitalWrite(trig_pin_1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin_1, LOW);
  const unsigned long duration1 = pulseIn(echo_pin_1, HIGH);
  distance1 = duration1 / 29 / 2;
  distance1 = constrain(distance1, 3, 42);
  distance1 = map(distance1, 42, 3, 0, 100);

}

//void sonic_2() {
//  digitalWrite(trig_pin_2, LOW);
//  delayMicroseconds(2);
//  digitalWrite(trig_pin_2, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trig_pin_2, LOW);
//  const unsigned long duration = pulseIn(echo_pin_2, HIGH);
//  distance2 = duration / 29 / 2;
//  distance2 = constrain(distance2, 3, 42);
//  distance2 = map(distance2, 42, 3, 0, 100);
//}

void sonic_3() {
  digitalWrite(trig_pin_3, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin_3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin_3, LOW);
  const unsigned long duration1 = pulseIn(echo_pin_3, HIGH);
  distance3 = duration1 / 29 / 2;
  distance3 = constrain(distance3, 3, 42);
  distance3 = map(distance3, 42, 3, 0, 100);

}

void sonic_4() {
  digitalWrite(trig_pin_4, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin_4, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin_4, LOW);
  const unsigned long duration1 = pulseIn(echo_pin_4, HIGH);
  distance4 = duration1 / 29 / 2;
  distance4 = constrain(distance4, 3, 42);
  distance4 = map(distance4, 42, 3, 0, 100);

}

void kirimEmail(int i, String kondisi) {

  EMailSender::EMailMessage message;
  if (i == 1) message.subject = "Pemberitahuan Masker 1 saat ini";
  if (i == 2) message.subject = "Pemberitahuan Masker 2 saat ini";
  if (i == 3) message.subject = "Pemberitahuan Masker 3 saat ini";
  if (i == 4) message.subject = "Pemberitahuan Masker 4 saat ini";
  if (i == 5) message.subject = "Masker baru saja diambil";

  if (kondisi == "Habis") message.message = "<h2>Hai masker telah habiss<br>Silahkan isi ulang</h2>";
  if (kondisi == "Sedikit") message.message = "<h2>Hai masker tinggal Sedikit<br>Silahkan isi ulang</h2>";
  if (kondisi == "Setengah") message.message = "<h2>Hai masker tersisa setengah lagi<br>Jangan lupa untuk mengisi masker!</h2>";
  if (kondisi == "Terisi")  message.message = "<h2>Hai masker kini terisi.<br></h2>";
  if (kondisi == "Penuh") message.message = "<h2>Hai masker kini terisi penuh.<br>Terimakasih telah mengisi ulang</h2>";
  if (kondisi == "Diambil") message.message = "<h2>Hai masker baru saja telah diambil.</h2>";

  EMailSender::Response resp = emailSend.send(emailTo, message);
  Serial.println("Sending status: ");
  Serial.println(resp.status);
  Serial.println(resp.code);
  Serial.println(resp.desc);
}


void checkoutEmail(bool kolomnya[], int maskernya, int jaraknya ) {
  if (masker(jaraknya) == "Habis" && kolomnya[0] == true) {
    kirimEmail(maskernya, "Habis");
    kolomnya[1] = true; kolomnya[2] = true; kolomnya[3] = true; kolomnya[4] = true; kolomnya[0] = false;
  }
  if (masker(jaraknya) == "Sedikit" && kolomnya[1] == true) {
    kirimEmail(maskernya, "Sedikit");
    kolomnya[0] = true; kolomnya[2] = true; kolomnya[3] = true; kolomnya[4] = true; kolomnya[1] = false;
  }
  if (masker(jaraknya) == "Setengah" && kolomnya[2] == true) {
    kirimEmail(maskernya, "Setengah");
    kolomnya[0] = true; kolomnya[1] = true; kolomnya[3] = true; kolomnya[4] = true; kolomnya[2] = false;
  }
  if (masker(jaraknya) == "Terisi" && kolomnya[3] == true) {
    kirimEmail(maskernya, "Terisi");
    kolomnya[0] = true; kolomnya[1] = true; kolomnya[2] = true; kolomnya[4] = true;  kolomnya[3] = false;
  }
  if (masker(jaraknya) == "Penuh" && kolomnya[4] == true) {
    kirimEmail(maskernya, "Penuh");
    kolomnya[0] = true; kolomnya[1] = true; kolomnya[2] = true; kolomnya[3] = true; kolomnya[4] = false;
  }
}
