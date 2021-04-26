


#include <ESP32Servo.h>
#include "config.h"
int x;
int y;
int Bot1 = 0;
int Bot2 = 0;
int Bot3 = 0;
int Bot4 = 0;
int Bot5 = 0;
int Bot6 = 0;

AdafruitIO_Feed *UP = io.feed("UP");
AdafruitIO_Feed *DOWN = io.feed("DOWN");
AdafruitIO_Feed *LEFT = io.feed("LEFT");
AdafruitIO_Feed *RIGHT = io.feed("RIGHT");
AdafruitIO_Feed *FWT = io.feed("FWT");
AdafruitIO_Feed *SHOOT = io.feed("SHOOT");

void setup() {
  // put your setup code here, to run once:

  pinMode(5, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(21, OUTPUT);

  UP->onMessage(handleMessage);
  DOWN->onMessage(handleMessage2);
  LEFT->onMessage(handleMessage3);
  RIGHT->onMessage(handleMessage4);
  FWT->onMessage(handleMessage5);
  SHOOT->onMessage(handleMessage6);

  Serial.begin(9600);

  io.connect(); //se conecta al servido
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());
  UP->get();
  DOWN->get();
  LEFT->get();
  RIGHT->get();
  FWT->get();
  SHOOT->get();
}



void loop() {
  io.run();



}


void handleMessage(AdafruitIO_Data * data) {
  Bot1 = data->toInt();
  if (Bot1 == 1) {
    digitalWrite(5, HIGH);
    Serial.println("UP");
  }
  else {
    digitalWrite(5, LOW);
  }
}


void handleMessage2(AdafruitIO_Data * data) {
  Bot2 = data->toInt();
  if (Bot2 == 1) {
    digitalWrite(15, HIGH);
    Serial.println("DOWN");
  }
  else {
   digitalWrite(15, LOW);
  }
}

void handleMessage3(AdafruitIO_Data * data) {
  Bot3 = data->toInt();
  if (Bot3 == 1) {
    digitalWrite(18, HIGH);
    Serial.println("IZQ");
  }
  else {
    digitalWrite(18, LOW);
  }
}


void handleMessage4(AdafruitIO_Data * data) {
  Bot4 = data->toInt();
  if (Bot4 == 1) {
    digitalWrite(19, HIGH);
    Serial.println("DER");
  }
  else {
    digitalWrite(19, LOW);
  }
}

void handleMessage5(AdafruitIO_Data * data) {
  Bot5 = data->toInt();
  if (Bot5 == 1) {
    digitalWrite(21, HIGH);
    Serial.println("AVA");
  }
  else {
    digitalWrite(21, LOW);
  }
}


void handleMessage6(AdafruitIO_Data * data) {
  Bot6 = data->toInt();
  if (Bot6 == 1) {
    digitalWrite(23, HIGH);
    Serial.println("!");
  }
  else {
    digitalWrite(23, LOW);
  }
}
