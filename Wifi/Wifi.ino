
int WifiData1, WifiData2, WifiData3, WifiData4, WifiData5, WifiData6;
int flag1, flag2, flag3, flag4, flag5, flag6=0;
int arriba, abajo, izquierda, derecha, disparo, mover;  
#include "config.h"
AdafruitIO_Feed *up = io.feed("up");
AdafruitIO_Feed *right = io.feed("right");
AdafruitIO_Feed *left = io.feed("left");
AdafruitIO_Feed *down = io.feed("down");
AdafruitIO_Feed *shoot = io.feed("shoot");
AdafruitIO_Feed *fwd = io.feed("fwd");

void setup() {
  Serial.begin(9600);
 io.connect();
  up->onMessage(handleMessage1);
  right->onMessage(handleMessage2);
  left->onMessage(handleMessage3);
  down->onMessage(handleMessage4);
  shoot->onMessage(handleMessage5);
  fwd->onMessage(handleMessage6);
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
}
up->get();
right->get();
left->get();
down->get();
shoot->get();
fwd->get();
pinMode(4, OUTPUT);
}

void loop() {
  io.run();
  // put your main code here, to run repeatedly:
}
void handleMessage1(AdafruitIO_Data *data) {

  
  arriba=data->toInt();
  if(arriba==1){
    Serial.println("Arriba");
    digitalWrite(4, HIGH);
  }
  else{
    digitalWrite(4, LOW);
  }
  

}
void handleMessage2(AdafruitIO_Data *data) {

  
  derecha=data->toInt();
    if(derecha==1){
    Serial.println("Derecha");
  }
  

}
void handleMessage3(AdafruitIO_Data *data) {

  
  izquierda=data->toInt();
    if(izquierda==1){
    Serial.println("izquierda");
  }

}
void handleMessage4(AdafruitIO_Data *data) {

  
  abajo=data->toInt();
    if(abajo==1){
    Serial.println("Abajo");
  }

}
void handleMessage5(AdafruitIO_Data *data) {

  
  disparo=data->toInt();
    if(disparo==1){
    Serial.println("disparo");
  }

}
void handleMessage6(AdafruitIO_Data *data) {

  
  mover=data->toInt();
    if(mover==1){
    Serial.println("mover");
  }

}
