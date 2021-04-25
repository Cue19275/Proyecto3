
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
pinMode(23, OUTPUT);//Disparo
pinMode(21, OUTPUT);//Avanzar
pinMode(19, OUTPUT);//Derecha
pinMode(18, OUTPUT);//Izquierda
pinMode(5, OUTPUT);//Arriba
pinMode(15, OUTPUT);//Abajo
}

void loop() {
  io.run();
  // put your main code here, to run repeatedly:
}
void handleMessage1(AdafruitIO_Data *data) { 
  arriba=data->toInt();
  if(arriba==1){
    Serial.println("Arriba");
    digitalWrite(5, HIGH);
  }
  else{
    digitalWrite(5, LOW);
  } 
}
void handleMessage2(AdafruitIO_Data *data) {
  derecha=data->toInt();
    if(derecha==1){
    Serial.println("Derecha");
   digitalWrite(19, HIGH);
  }
  else{
    digitalWrite(19, LOW);
  } 
}
void handleMessage3(AdafruitIO_Data *data) {
  izquierda=data->toInt();
    if(izquierda==1){
    Serial.println("izquierda");
   digitalWrite(18, HIGH);
  }
  else{
    digitalWrite(18, LOW);
  } 
}
void handleMessage4(AdafruitIO_Data *data) {
  abajo=data->toInt();
    if(abajo==1){
    Serial.println("Abajo");
   digitalWrite(15, HIGH);
  }
  else{
    digitalWrite(15, LOW);
  } 
}
void handleMessage5(AdafruitIO_Data *data) {
  disparo=data->toInt();
    if(disparo==1){
    Serial.println("disparo");
   digitalWrite(23, HIGH);
  }
  else{
    digitalWrite(23, LOW);
  } 
}
void handleMessage6(AdafruitIO_Data *data) {
  mover=data->toInt();
    if(mover==1){
    Serial.println("mover");
   digitalWrite(21, HIGH);
  }
  else{
    digitalWrite(21, LOW);
  } 
}
