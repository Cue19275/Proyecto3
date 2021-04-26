#include <SPI.h>
#include <SD.h>
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
File myFile;
File root;
//***********************************************************************************************************************************************************************************
//VARIABLES
//***********************************************************************************************************************************************************************************
int entrada;
int entrada2;
int x_t1;
int y_t1;
int x_b1;
int y_b1;
int x_t2;
int y_t2;
int x_b2;
int y_b2;

byte record1_1;
byte record1_2;
byte record1_3;
byte record2_1;
byte record2_2;
byte record2_3;
int record1;
int record1U;
int record2;
int record2U;
byte basura;
String archivo;
String Datos;

int end_game;

int con1 = 0;
int con2 = 0;
int con3 = 0;
int con4 = 0;

int con1_ = 0;
int con2_ = 0;
int con3_ = 0;
int con4_ = 0;

int pos_t1[17][17];
int pos_t2[17][17];

const int y_limsup = 31;
const int y_liminf = 224;
const int x_limsup = 287;
const int x_liminf = 15;

int flag_orient;
int disparo_vertU;
int disparo_vertA;
int disparo_horI;
int disparo_horD;
int disparo_activo;

int t1 = 0;
int t2 = 0;

int flag_orient2;
int disparo_vertU2;
int disparo_vertA2;
int disparo_horI2;
int disparo_horD2;
int disparo_activo2;

int hit1;
int hit2;
int life1;
int life2;

int enable_sprites = 0;
int enable_personajes = 0;
int cont_personajesp1 = 1;
int cont_personajesp2 = 1;
int gano_p1 = 0;
int gano_p2 = 0;

uint8_t BO1;
uint8_t BO2;
uint8_t BO3;
uint8_t BO4;
uint8_t BO5;
uint8_t BO6;
uint8_t BO8;
uint8_t BO9;
uint8_t BO10;
uint8_t BO11;
uint8_t BO12;
uint8_t BO13;
uint8_t fon;
uint8_t fon1;
uint8_t FLAG;
uint8_t FLAG2;
uint8_t FLAG3;
uint8_t FLAG4;
uint8_t FLAGO;
uint8_t FLAGO1;
uint8_t FLAGO2;
uint8_t FLAGO3;
uint8_t FLAGO4;
uint8_t FLAGO5;
uint8_t FLAGO6;
uint8_t FLAGO7;
uint8_t FLAGO8;
uint8_t FLAGO9;
uint8_t FLAGO10;
uint8_t FLAGO11;
uint8_t FLAGO12;
uint8_t FLAGO13;
uint8_t FLAGO14;
uint8_t FLAGO15;
uint8_t FINAL;

extern uint8_t otacon[];
extern uint8_t snake[];
extern uint8_t meryl[];
extern uint8_t liquid[];
extern uint8_t naomi[];
extern uint8_t sniper[];


//************************************************************************************************************************************************************************************
// PROTOTIPOS DE FUNCION
//*************************************************************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset);
void pista(void);
void ini(void);
void gameover(void);
void ploteo(int origenx, int origeny, int origenx2, int origeny2, int dir);
void HIT(int x_b1, int y_b1, int x_t2, int y_t2);
int num_ascii(char num);
void character_select(int p1, int p2);
//Indice de la imagen, 0, 0



extern uint8_t fondo[];
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  SPI.setModule(0);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial2.begin(9600);

  //ENTRADAS Y SALIDAS
  pinMode(PA_3, OUTPUT);
  pinMode(PF_4, INPUT_PULLUP);
  pinMode(PA_6, INPUT_PULLUP);
  pinMode(PA_7, INPUT_PULLUP);
  pinMode(PF_1, INPUT_PULLUP);
  pinMode(PE_3, INPUT_PULLUP);
  pinMode(PE_5, INPUT_PULLUP);
  pinMode(PF_3, INPUT_PULLUP);
  pinMode(PF_2, INPUT_PULLUP);
  pinMode(PC_4, INPUT_PULLUP);
  pinMode(PC_5, INPUT_PULLUP);
  pinMode(PC_6, INPUT_PULLUP);
  pinMode(PC_7, INPUT_PULLUP);
  pinMode(PD_6, OUTPUT);
  pinMode(PD_7, OUTPUT);

//INICIALIZACION DE TARJETA SD
  if (!SD.begin(PA_3)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");


  Serial.println("Inicio");

  //INTRO DEL JUEGO

  LCD_Init();
  LCD_Clear(0x00);
  //x1,y1,x2,y2
  FillRect(0, 0, 319, 206, 0x00);

  String text1 = "MONSTRU INC.";
  String text2 = "&";
  String text3 = "CUE CORP.";
  //text, x, y ,tamaño de font, color, background
  LCD_Print(text1, 0, 100, 2, 0xffff, 0x00);
  LCD_Print(text2, 0, 120, 2, 0xffff, 0x00);
  LCD_Print(text3, 0, 140, 2, 0xffff, 0x00);  //SE COLOCAN LOS TEXTOS CON
  delay (1000);

  LCD_Clear(0x00);

  String text4 = "PRESENT";
  //text, x, y ,tamaño de font, color, background
  LCD_Print(text4, 100, 100, 2, 0xffff, 0x00);
  delay (3000);
  ini();  //AL TERMINAR LA INTRO, SE VA A LA PANTALLA DE INICIO





  //



  //LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);

  //LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);

  //VALORES INICIALES
  x_t1 = 80;
  y_t1 = 100;
  x_t2 = 160;
  y_t2 = 100;
  disparo_vertU = 0;
  disparo_vertA = 0;
  disparo_horI = 0;
  disparo_horD = 0;
  disparo_activo = 0;
  flag_orient = 0;
  disparo_vertU2 = 0;
  disparo_vertA2 = 0;
  disparo_horI2 = 0;
  disparo_horD2 = 0;
  disparo_activo2 = 0;
  flag_orient2 = 0;
  hit1 = 0;
  hit2 = 0;
  life1 = 3;
  life2 = 3;
  end_game = 0;
  t1 = millis();



}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
  //***********************************Conteo de Tiempo****************************************************
  t2 = millis();
  if ((t1 + 100) < t2) {
    t1 = millis(),
    enable_sprites = 1;
  }

//TOGGLE PARA LA MUSICA

  if (FLAG == 1 && FLAG2 == 0 && FLAG3 == 0) { //SI ESTA EN LA PANTALLA DE INICIO SUENA LA CANCION DE BLACK SABBATH

    digitalWrite (PD_7, HIGH);
    digitalWrite (PD_6, LOW); //TOGGLE DE PINES, ESTO SE HACE PARA ENCENDER Y APAGAR PINES CONECTADOS A UN ARDUINO

  }

  if (FLAG2 == 1 && FLAG == 0 && FLAG3 == 0) { // SI ESTA EN EL MODO DE JUEGO SUENA LA CANCION DE ROCKY
    digitalWrite (PD_6, HIGH);
    digitalWrite (PD_7, LOW);
  }

  if (FLAG2 == 0 && FLAG == 0 && FLAG3 == 1) { //SI ESTA EN LA PANTALLA DE GAME OVER SUENA LA CANCION DE SWEET CHILD O'MINE
    digitalWrite (PD_6, HIGH);
    digitalWrite (PD_7, HIGH);
  }





  //***************************************************************************************************************************************
  // BOTONES
  //***************************************************************************************************************************************
  delay(10);

  //BOTONES P1
  BO1 = digitalRead(PA_7); //AVANZAR
  BO2 = digitalRead(PA_6); //ARRIBA
  BO3 = digitalRead(PF_4); //DERECHA
  BO4 = digitalRead(PF_1); //IZQUIERDA
  BO5 = digitalRead(PE_5); //ABAJO
  BO6 = digitalRead(PE_3); //DISPARO
  //BO7 = digitalRead(PF_3);

  // BOTONES P2
  BO8 = digitalRead(PC_4);//IZQUIERDA
  BO9 = digitalRead(PC_5);//DERECHA
  BO10 = digitalRead(PC_6);//ARRIBA
  BO11 = digitalRead(PC_7);//ABAJO
  BO12 = digitalRead(PF_2);//DISPARO
  BO13 = digitalRead(PF_3);//AVANZAR

  //HABILITAR BOTONES

  if (FLAG4 == 1) {  //SEGUN LA BANDERA ACTIVA, VA A SER EL FUNCIONAMIENTO DE LOS BOTONES
    if (BO1 == LOW) { //EN ESTE CASO LA BANDERA ACTIVA ES PARA EL CHARACTER SELECT
      fon = 1;
    }
    if (fon == 1 && BO1 == HIGH) {
      fon = 0;
      enable_personajes = 0;
      chara();
    }

  }

  if (FLAG == 1) {
    if (BO1 == LOW) { //EN ESTE CASO LA BANDERA ACTIVA ES PARA EL MODO DE JUEGO
      fon = 1;
    }
    if (fon == 1 && BO1 == HIGH) {
      fon = 0;
      enable_personajes = 0;
      pista();
    }

  }
  //********************************************************************************************
  //********************************SELECCION DE PERSONAJE***********************************
  //********************************************************************************************
  //PERSONAJE1 p1
  if (FLAG == 1) {
    if (BO3 == LOW) {
      FLAGO12 = 1;
      Serial.println("FaseUno");
    }
    if (FLAGO12 == 1 && BO3 == HIGH) {
      FLAGO12 = 0;
      cont_personajesp1++;

      if (cont_personajesp1 > 3) {
        cont_personajesp1 = 1;
      }

    }
  }
  //PERSONAJE2 p1
  if (FLAG == 1) {
    if (BO4 == LOW) {
      FLAGO13 = 1;
    }
    if (FLAGO13 == 1 && BO4 == HIGH) {
      FLAGO13 = 0;
      cont_personajesp1--;
      if (cont_personajesp1 < 1) {
        cont_personajesp1 = 3;
      }

    }

  }
  //PERSONAJE1 p2
  if (FLAG == 1) {
    if (BO9 == LOW) {
      FLAGO14 = 1;
    }
    if (FLAGO14 == 1 && BO9 == HIGH) {
      FLAGO14 = 0;
      cont_personajesp2++;
      if (cont_personajesp2 > 3) {
        cont_personajesp2 = 1;
      }

    }
  }
  //PERSONAJE2 p3
  if (FLAG == 1) {
    if (BO8 == LOW) {
      FLAGO15 = 1;
    }
    if (FLAGO15 == 1 && BO8 == HIGH) {
      FLAGO15 = 0;
      cont_personajesp2--;
      if (cont_personajesp2 < 1) {
        cont_personajesp2 = 3;
      }

    }

  }


  if (FLAG3 == 1) {
    if (BO1 == LOW) { //EN ESTE CASO LA BANDERA ACTIVA ES PARA EL GAME OVER
      fon = 1;
    }
    if (fon == 1 && BO1 == HIGH) {
      fon = 0;
      FLAG3 = 0;
      ini();
    }

  }


  //********************************************************************************************
  //********************************SELECCION DE PERSONAJE***********************************
  //********************************************************************************************

  //PERSONAJE1 p1
  if (FLAG == 1) {
    if (BO3 == LOW) {
      FLAGO12 = 1;
      Serial.println("FaseUno");
    }
    if (FLAGO12 == 1 && BO3 == HIGH) {
      FLAGO12 = 0;
      cont_personajesp1++;

      if (cont_personajesp1 > 3) {
        cont_personajesp1 = 1;
      }

    }
  }
  //PERSONAJE2 p1
  if (FLAG == 1) {
    if (BO4 == LOW) {
      FLAGO13 = 1;
    }
    if (FLAGO13 == 1 && BO4 == HIGH) {
      FLAGO13 = 0;
      cont_personajesp1--;
      if (cont_personajesp1 < 1) {
        cont_personajesp1 = 3;
      }

    }

  }
  //PERSONAJE1 p2
  if (FLAG == 1) {
    if (BO9 == LOW) {
      FLAGO14 = 1;
    }
    if (FLAGO14 == 1 && BO9 == HIGH) {
      FLAGO14 = 0;
      cont_personajesp2++;
      if (cont_personajesp2 > 3) {
        cont_personajesp2 = 1;
      }

    }
  }
  //PERSONAJE2 p3
  if (FLAG == 1) {
    if (BO8 == LOW) {
      FLAGO15 = 1;
    }
    if (FLAGO15 == 1 && BO8 == HIGH) {
      FLAGO15 = 0;
      cont_personajesp2--;
      if (cont_personajesp2 < 1) {
        cont_personajesp2 = 3;
      }

    }

  }


  /*for (int x = 0; x<6; x++){
    if(x == 6){
      x = 0;
      delay(100);
    }
    LCD_Sprite(30, 30, 17, 17, explo, 6, x, 0, 0);*/

  //****************************************************************************************
  //*Detección HIT
  //***********************************************************************************
  if (hit1 == 1) {
    hit1 = 0;
    life2--;
    for (int i = 0; i < 6; i++) {
      LCD_Sprite(x_t2, y_t2, 17, 17, explo, 6, i, 0, 0);
      delay(50);
    }
    FillRect(x_t2, y_t2, 17, 17, 0);
    if (life2 == 0) {
      LCD_Bitmap(x_t2, y_t2, 18, 30, tumba);
      FillRect(197, 4, 83, 8, 0);
      LCD_Bitmap(264, 4, 16, 8, lifeb4);
      delay(100);
      end_game = 1;
      Serial.println("Gano Azul");
      record1++;
      if (record1 > 9) {
        record1 = 1;
        record2 = 0;
      }
      gano_p1=1;
      gano_p2=0;
      gameover();
    }
    else if (life2 == 2) {
      FillRect(197, 4, 83, 8, 0);
      LCD_Bitmap(220, 4, 60, 8, lifeb2);
    }
    else if (life2 == 1) {
      FillRect(197, 4, 83, 8, 0);
      LCD_Bitmap(244, 4, 36, 8, lifeb3);
    }
    if (end_game == 0) {
      if (flag_orient2 == 0) {
        LCD_Bitmap(x_t2, y_t2, 15, 17, tanque1_1);
      }
      else if (flag_orient2 == 1) {
        LCD_Bitmap(x_t2, y_t2, 17, 15, tanque1_2);
      }
      else if (flag_orient2 == 2) {
        LCD_Bitmap(x_t2, y_t2, 17, 15, tanque1_3);
      }
      else if (flag_orient2 == 3) {
        LCD_Bitmap(x_t2, y_t2, 15, 17, tanque1_4);
      }
    }
  }

  if (hit2 == 1) {
    hit2 = 0;
    life1--;
    for (int i = 0; i < 6; i++) {
      LCD_Sprite(x_t1, y_t1, 17, 17, explo, 6, i, 0, 0);
      delay(50);
    }
    FillRect(x_t1, y_t1, 17, 17, 0);
    if (life1 == 0) {
      LCD_Bitmap(x_t1, y_t1, 18, 30, tumba);
      FillRect(65, 4, 83, 8, 0);
      LCD_Bitmap(132, 4, 16, 8, lifeb4);
      end_game = 1;
      delay(100);
      Serial.println("Gano Amarillo");
      record2++;
      if (record2 > 9) {
        record1 = 0;
        record2 = 1;
      }
      gano_p2=1;
      gano_p1=0;
      gameover();
    }
    else if (life1 == 2) {
      FillRect(65, 4, 83, 8, 0);
      LCD_Bitmap(88, 4, 60, 8, lifeb2);

    }
    else if (life1 == 1) {
      FillRect(65, 4, 83, 8, 0);
      LCD_Bitmap(112, 4, 36, 8, lifeb3);
    }

    if (end_game == 0) {
      if (flag_orient == 0) {
        LCD_Bitmap(x_t1, y_t1, 15, 17, tanque2_1);
      }
      else if (flag_orient == 1) {
        LCD_Bitmap(x_t1, y_t1, 17, 15, tanque2_2);
      }
      else if (flag_orient == 2) {
        LCD_Bitmap(x_t1, y_t1, 17, 15, tanque2_3);
      }
      else if (flag_orient == 3) {
        LCD_Bitmap(x_t1, y_t1, 15, 17, tanque2_4);
      }
    }
  }
  //****************************************************************************************************************************************
  //*Movimiento
  //*****************************************************************************************************************************************

 //EN CADA BLOQUE SE UTILIZA UN ANTIRREBOTE PARA ASIGNARLE UN VALOR A UNA VARIABLE Y SEGUN ESE VALOR VA A SER LA ACCION DEL TANQUE

 //****************************************************************************************************************
  //BOTONES P1
  //****************************************************************************************************************


  if (FLAG2 == 1) {
    if (BO1 == LOW) {
      FLAGO = 1;
    }

    if (FLAGO == 1 && BO1 == HIGH) {
      FLAGO = 0;
      entrada = 48;         //AVANZAR

    }
  }

  if (FLAG2 == 1) {
    if (BO2 == LOW) {
      FLAGO1 = 1;
    }

    if (FLAGO1 == 1 && BO2 == HIGH) {
      FLAGO1 = 0;
      entrada = 49;      //ARRIBA

    }
  }

  if (FLAG2 == 1) {
    if (BO3 == LOW) {
      FLAGO2 = 1;
    }

    if (FLAGO2 == 1 && BO3 == HIGH) {
      FLAGO2 = 0;
      entrada = 50;    //DERECHA

    }
  }

  if (FLAG2 == 1) {
    if (BO4 == LOW) {
      FLAGO3 = 1;
    }

    if (FLAGO3 == 1 && BO4 == HIGH) {
      FLAGO3 = 0;
      entrada = 51;  //IZQUIERDA

    }
  }
  if (FLAG2 == 1) {
    if (BO5 == LOW) {
      FLAGO4 = 1;
    }
    if (FLAGO4 == 1 && BO5 == HIGH) {
      FLAGO4 = 0;
      entrada = 52;   //ABAJO

    }
  }

  if (FLAG2 == 1) {
    if (BO6 == LOW) {
      FLAGO5 = 1;
    }

    if (FLAGO5 == 1 && BO6 == HIGH) {
      FLAGO5 = 0;
      entrada = 53;   //DISPARO

    }
  }

  //****************************************************************************************************************
  //BOTONES P2
  //****************************************************************************************************************


  //AVANZAR P2
  if (FLAG2 == 1) {
    if (BO13 == LOW) {
      FLAGO6 = 1;
    }

    if (FLAGO6 == 1 && BO13 == HIGH) {
      FLAGO6 = 0;
      entrada2 = 58;  //AVANZAR
    }
  }

  //ARRIBA P2
  if (FLAG2 == 1) {
    if (BO10 == LOW) {
      FLAGO7 = 1;
    }

    if (FLAGO7 == 1 && BO10 == HIGH) {
      FLAGO7 = 0;
      entrada2 = 59; //ARRIBA

    }
  }


  //DERECHA P2
  if (FLAG2 == 1) {
    if (BO9 == LOW) {
      FLAGO8 = 1;
    }

    if (FLAGO8 == 1 && BO9 == HIGH) {
      FLAGO8 = 0;
      entrada2 = 60;  //DERECHA

    }
  }

  //IZQUIERDA P2
  if (FLAG2 == 1) {
    if (BO8 == LOW) {
      FLAGO9 = 1;
    }

    if (FLAGO9 == 1 && BO8 == HIGH) {
      FLAGO9 = 0;
      entrada2 = 61;  //IZQUIERDA

    }
  }

  //ABAJO P2
  if (FLAG2 == 1) {
    if (BO11 == LOW) {
      FLAGO10 = 1;
    }
    if (FLAGO10 == 1 && BO11 == HIGH) {
      FLAGO10 = 0;
      entrada2 = 62;  //ABAJO

    }
  }

  //DISPARO P2
  if (FLAG2 == 1) {
    if (BO12 == LOW) {
      FLAGO11 = 1;
    }

    if (FLAGO11 == 1 && BO12 == HIGH) {
      FLAGO11 = 0;
      entrada2 = 63;

    }
  }

  //Para saltar de Game Over a inicio
  if (FLAG3 == 1) {
    if (BO2 == LOW) {
      fon1 = 1;
    }
    if (fon1 == 1 && BO2 == HIGH) {
      fon1 = 0;
      ini();
    }

  }
  character_select(cont_personajesp1, cont_personajesp2);
  //*****************************Matrices de espacio ocupado*********************************
  ploteo(x_t1, y_t1, x_t2, y_t2, flag_orient);

  //***********************Movimiento***************************************
  //************************Bloque1Movimiento********************************
  if (entrada == 48 && flag_orient == 0 && con3 == 0) {
    entrada = 0;
    FillRect(x_t1, y_t1, 15, 17, 0);
    y_t1 -= 5;
    if (y_t1 <= y_limsup) {
      y_t1 += 5;
      LCD_Bitmap(x_t1, y_t1, 15, 17, tanque2_1);
    }
    else {
      LCD_Bitmap(x_t1, y_t1, 15, 17, tanque2_1);
    }
  }
  //************************Bloque2Movimiento********************************
  else if (entrada == 48 && flag_orient == 1 && con2 == 0) {
    entrada = 0;
    FillRect(x_t1, y_t1, 17, 15, 0);
    x_t1 += 5;
    if (x_t1 >= x_limsup) {
      x_t1 -= 5;
      LCD_Bitmap(x_t1, y_t1, 17, 15, tanque2_2);
    }
    else {
      LCD_Bitmap(x_t1, y_t1, 17, 15, tanque2_2);
    }
  }
  //************************Bloque3Movimiento*************************************
  else if (entrada == 48 && flag_orient == 2 && con1 == 0) {
    entrada = 0;
    FillRect(x_t1, y_t1, 17, 15, 0);
    x_t1 -= 5;
    if (x_t1 <= x_liminf) {
      x_t1 += 5;
      LCD_Bitmap(x_t1, y_t1, 17, 15, tanque2_3);
    }
    else {
      LCD_Bitmap(x_t1, y_t1, 17, 15, tanque2_3);
    }
  }
  //************************Bloque4Movimiento********************************
  else if (entrada == 48 && flag_orient == 3 && con4 == 0) {
    entrada = 0;
    FillRect(x_t1, y_t1, 15, 17, 0);
    y_t1 += 5;
    if (y_t1 >= (y_liminf - 17)) {
      y_t1 -= 5;
      LCD_Bitmap(x_t1, y_t1, 15, 17, tanque2_4);
    }
    else {
      LCD_Bitmap(x_t1, y_t1, 15, 17, tanque2_4);
    }
  }
  //***********************CambiosDeDireccion***************************************
  //************************Bloque1Direccion********************************
  else if (entrada == 49) {
    entrada = 0;
    FillRect(x_t1, y_t1, 17, 17, 0);
    LCD_Bitmap(x_t1, y_t1, 15, 17, tanque2_1);
    flag_orient = 0;
  }
  //************************Bloque2Direccion********************************
  else if (entrada == 50) {
    entrada = 0;
    FillRect(x_t1, y_t1, 17, 17, 0);
    LCD_Bitmap(x_t1, y_t1, 17, 15, tanque2_2);
    flag_orient = 1;
  }
  //************************Bloque3Direccion********************************
  else if (entrada == 51) {
    entrada = 0;
    FillRect(x_t1, y_t1, 17, 17, 0);
    LCD_Bitmap(x_t1, y_t1, 17, 15, tanque2_3);
    flag_orient = 2;
  }
  //************************Bloque4Direccion********************************
  else if (entrada == 52) {
    entrada = 0;
    FillRect(x_t1, y_t1, 17, 17, 0);
    LCD_Bitmap(x_t1, y_t1, 15, 17, tanque2_4);
    flag_orient = 3;
  }
  //************************Bloque1Disparo********************************
  if ((entrada == 53 ) && ((flag_orient == 0) && (disparo_activo == 0))) {
    entrada = 0;
    disparo_vertU = 1;
    disparo_vertA = 0;
    disparo_horI = 0;
    disparo_horD = 0;

    if ((y_t1 - 12) >= y_limsup ) {
      disparo_activo = 1;

      x_b1 = x_t1 + 4;
      y_b1 = y_t1 - 12;
    }
    else {
      disparo_activo = 0;
    }
  }
  //************************Bloque2Disparo********************************
  else if ((entrada == 53 ) && ((flag_orient == 1) && (disparo_activo == 0))) {
    entrada = 0;
    disparo_vertU = 0;
    disparo_vertA = 0;
    disparo_horI = 0;
    disparo_horD = 1;
    disparo_activo = 1;

    if ((x_t1 + 19) <= x_limsup) {
      disparo_activo = 1;

      x_b1 = x_t1 + 18;
      y_b1 = y_t1 + 4;
    }
    else {
      disparo_activo = 0;
    }

  }
  //************************Bloque3Disparo********************************
  else if ((entrada == 53 ) && ((flag_orient == 2) && (disparo_activo == 0))) {
    entrada = 0;
    disparo_vertU = 0;
    disparo_vertA = 0;
    disparo_horI = 1;
    disparo_horD = 0;
    disparo_activo = 1;

    if ((x_t1 - 12) <= x_limsup) {
      disparo_activo = 1;

      x_b1 = x_t1 - 12;
      y_b1 = y_t1 + 4;
    }
    else {
      disparo_activo = 0;
    }
    //************************Bloque4Disparo********************************
  }
  else if ((entrada == 53 ) && ((flag_orient == 3) && (disparo_activo == 0))) {
    entrada = 0;
    disparo_vertU = 0;
    disparo_vertA = 1;
    disparo_horI = 0;
    disparo_horD = 0;
    disparo_activo = 1;

    if ((y_t1 + 27) <= y_liminf) {
      disparo_activo = 1;
      x_b1 = x_t1 + 4;
      y_b1 = y_t1 + 18;
    }
    else {
      disparo_activo = 0;
    }
  }



  //**************************************************************************************
  //***************************PLAYER2****************************************************
  //**************************************************************************************
  //*****************************Matrices de espacio ocupado*********************************
  ploteo2(x_t2, y_t2, x_t1, y_t1, flag_orient2);

  //***********************Movimiento***************************************
  //************************Bloque1Movimiento********************************
  if (entrada2 == 58 && flag_orient2 == 0 && con3_ == 0) {
    entrada2 = 0;
    FillRect(x_t2, y_t2, 15, 17, 0);
    y_t2 -= 5;
    if (y_t2 <= y_limsup) {
      y_t2 += 5;
      LCD_Bitmap(x_t2, y_t2, 15, 17, tanque1_1);
    }
    else {
      LCD_Bitmap(x_t2, y_t2, 15, 17, tanque1_1);
    }
  }
  //************************Bloque2Movimiento********************************
  else if (entrada2 == 58 && flag_orient2 == 1 && con2_ == 0) {
    entrada2 = 0;
    FillRect(x_t2, y_t2, 17, 15, 0);
    x_t2 += 5;
    if (x_t2 >= x_limsup) {
      x_t2 -= 5;
      LCD_Bitmap(x_t2, y_t2, 17, 15, tanque1_2);
    }
    else {
      LCD_Bitmap(x_t2, y_t2, 17, 15, tanque1_2);
    }
  }
  //************************Bloque3Movimiento*************************************
  else if (entrada2 == 58 && flag_orient2 == 2 && con1_ == 0) {
    entrada2 = 0;
    FillRect(x_t2, y_t2, 17, 15, 0);
    x_t2 -= 5;
    if (x_t2 <= x_liminf) {
      x_t2 += 5;
      LCD_Bitmap(x_t2, y_t2, 17, 15, tanque1_3);
    }
    else {
      LCD_Bitmap(x_t2, y_t2, 17, 15, tanque1_3);
    }
  }
  //************************Bloque4Movimiento********************************
  else if (entrada2 == 58 && flag_orient2 == 3 && con4_ == 0) {
    entrada2 = 0;
    FillRect(x_t2, y_t2, 15, 17, 0);
    y_t2 += 5;
    if (y_t2 >= (y_liminf - 17)) {
      y_t2 -= 5;
      LCD_Bitmap(x_t2, y_t2, 15, 17, tanque1_4);
    }
    else {
      LCD_Bitmap(x_t2, y_t2, 15, 17, tanque1_4);
    }
  }
  //***********************CambiosDeDireccion***************************************
  //************************Bloque1Direccion********************************
  else if (entrada2 == 59) {
    entrada2 = 0;
    FillRect(x_t2, y_t2, 17, 17, 0);
    LCD_Bitmap(x_t2, y_t2, 15, 17, tanque1_1);
    flag_orient2 = 0;
  }
  //************************Bloque2Direccion********************************
  else if (entrada2 == 60) {
    entrada2 = 0;
    FillRect(x_t2, y_t2, 17, 17, 0);
    LCD_Bitmap(x_t2, y_t2, 17, 15, tanque1_2);
    flag_orient2 = 1;
  }
  //************************Bloque3Direccion********************************
  else if (entrada2 == 61) {
    entrada2 = 0;
    FillRect(x_t2, y_t2, 17, 17, 0);
    LCD_Bitmap(x_t2, y_t2, 17, 15, tanque1_3);
    flag_orient2 = 2;
  }
  //************************Bloque4Direccion********************************
  else if (entrada2 == 62) {
    entrada2 = 0;
    FillRect(x_t2, y_t2, 17, 17, 0);
    LCD_Bitmap(x_t2, y_t2, 15, 17, tanque1_4);
    flag_orient2 = 3;
  }
  //***********************Disparo***************************************
  //************************Bloque1Disparo********************************
  if ((entrada2 == 63 ) && ((flag_orient2 == 0) && (disparo_activo2 == 0))) {
    entrada2 = 0;
    disparo_vertU2 = 1;
    disparo_vertA2 = 0;
    disparo_horI2 = 0;
    disparo_horD2 = 0;

    if ((y_t2 - 12) >= y_limsup) {
      disparo_activo2 = 1;
      x_b2 = x_t2 + 4;
      y_b2 = y_t2 - 12;
    }
    else {
      disparo_activo2 = 0;
    }
  }
  //************************Bloque2Disparo********************************
  else if ((entrada2 == 63 ) && ((flag_orient2 == 1) && (disparo_activo2 == 0))) {
    entrada2 = 0;
    disparo_vertU2 = 0;
    disparo_vertA2 = 0;
    disparo_horI2 = 0;
    disparo_horD2 = 1;
    disparo_activo2 = 1;

    if ((x_t2 + 19) <= x_limsup) {
      disparo_activo2 = 1;

      x_b2 = x_t2 + 18;
      y_b2 = y_t2 + 4;
    }
    else {
      disparo_activo2 = 0;
    }

  }
  //************************Bloque3Disparo********************************
  else if ((entrada2 == 63 ) && ((flag_orient2 == 2) && (disparo_activo2 == 0))) {
    entrada2 = 0;
    disparo_vertU2 = 0;
    disparo_vertA2 = 0;
    disparo_horI2 = 1;
    disparo_horD2 = 0;
    disparo_activo2 = 1;

    if ((x_t2 - 12) <= x_limsup) {
      disparo_activo2 = 1;

      x_b2 = x_t2 - 12;
      y_b2 = y_t2 + 4;
    }
    else {
      disparo_activo2 = 0;
    }
    //************************Bloque4Disparo********************************
  }
  else if ((entrada2 == 63 ) && ((flag_orient2 == 3) && (disparo_activo2 == 0))) {
    entrada2 = 0;
    disparo_vertU2 = 0;
    disparo_vertA2 = 1;
    disparo_horI2 = 0;
    disparo_horD2 = 0;
    disparo_activo2 = 1;

    if ((y_t2 + 27) <= y_liminf) {
      disparo_activo2 = 1;

      x_b2 = x_t2 + 4;
      y_b2 = y_t2 + 18;
    }
    else {
      disparo_activo2 = 0;
    }

  }
  //************************Bloque3Disparo********************************
  else if ((entrada2 == 63 ) && ((flag_orient2 == 2) && (disparo_activo2 == 0))) {
    entrada2 = 0;
    disparo_vertU2 = 0;
    disparo_vertA2 = 0;
    disparo_horI2 = 1;
    disparo_horD2 = 0;
    disparo_activo2 = 1;

    if ((x_t2 - 12) <= x_limsup) {
      disparo_activo2 = 1;

      x_b2 = x_t2 - 12;
      y_b2 = y_t2 + 4;
    }
    else {
      disparo_activo2 = 0;
    }
    //************************Bloque4Disparo********************************
  }
  else if ((entrada2 == 63 ) && ((flag_orient2 == 3) && (disparo_activo2 == 0))) {
    entrada2 = 0;
    disparo_vertU2 = 0;
    disparo_vertA2 = 1;
    disparo_horI2 = 0;
    disparo_horD2 = 0;
    disparo_activo2 = 1;

    if ((y_t2 + 27) <= y_liminf) {
      disparo_activo2 = 1;

      x_b2 = x_t2 + 4;
      y_b2 = y_t2 + 18;
    }
    else {
      disparo_activo2 = 0;
    }
  }


  if (enable_sprites == 1) {

    enable_sprites = 0;
    //------->Avance Disparo
    //************************Bloque1AvanceDisparo********************************
    if ((disparo_activo == 1) && (disparo_vertU == 1)) {
      FillRect(x_b1, y_b1, 8, 10, 0);
      y_b1 -= 5;
      if ((((x_b1 + 8) > x_t2) && ((x_b1) < (x_t2 + 17))) && (((y_b1) > y_t2) && ((y_b1) < (y_t2 + 17)))) {
        disparo_activo = 0;
        hit1 = 1;
      }
      if ((y_b1 >= y_limsup) && hit1 == 0) {
        LCD_Bitmap((x_b1), (y_b1), 8, 10, balazo_v);

      }
      else {
        disparo_activo = 0;
      }
    }

    //************************Bloque2AvanceDisparo********************************
    else if ((disparo_activo == 1) && (disparo_horD == 1)) {
      FillRect(x_b1, y_b1, 10, 8, 0);
      x_b1 += 5;
      if ((((y_b1 + 17) > y_t2) && ((y_b1) < (y_t2 + 17))) && (((x_b1 + 10) > x_t2) && ((x_b1 + 10) < (x_t2 + 17)))) {
        disparo_activo = 0;
        hit1 = 1;
      }

      if ((x_b1 <= (x_limsup + 10)) && hit1 == 0) {
        LCD_Bitmap((x_b1), (y_b1), 10, 8, balazo);

      }
      else {
        disparo_activo = 0;
      }


    }
    //************************Bloque3AvanceDisparo********************************
    else if ((disparo_activo == 1) && (disparo_horI == 1)) {
      FillRect(x_b1, y_b1, 10, 8, 0);
      x_b1 -= 5;
      if ((((y_b1 + 17) > y_t2) && ((y_b1) < (y_t2 + 17))) && (((x_b1 - 5) < x_t2 + 17) && ((x_b1) > (x_t2)))) {
        disparo_activo = 0;
        hit1 = 1;
      }
      if ((x_b1 >= (x_liminf)) && hit1 == 0) {
        LCD_Bitmap((x_b1), (y_b1), 10, 8, balazo);

      }
      else {
        disparo_activo = 0;
      }

    }
    //************************Bloque4AvanceDisparo********************************
    else if ((disparo_activo == 1) && (disparo_vertA == 1)) {
      FillRect(x_b1, y_b1, 8, 10, 0);
      y_b1 += 5;
      if ((((x_b1 + 8) > x_t2) && ((x_b1) < (x_t2 + 17))) && (((y_b1 + 10) > y_t2) && ((y_b1 + 10) < (y_t2 + 17)))) {
        disparo_activo = 0;
        hit1 = 1;
      }
      if ((y_b1 <= (y_liminf - 10)) && hit1 == 0) {
        LCD_Bitmap((x_b1), (y_b1), 8, 10, balazo_v);

      }
      else {
        disparo_activo = 0;
      }

    }
    //------->Avance Disparo2
    //************************Bloque1AvanceDisparo********************************
    if ((disparo_activo2 == 1) && (disparo_vertU2 == 1)) {
      FillRect(x_b2, y_b2, 8, 10, 0);
      y_b2 -= 5;
      if ((((x_b2 + 8) > x_t1) && ((x_b2) < (x_t1 + 17))) && (((y_b2) > y_t1) && ((y_b2) < (y_t1 + 17)))) {
        disparo_activo2 = 0;
        hit2 = 1;
      }
      if ((y_b2 >= y_limsup) && hit2 == 0) {
        LCD_Bitmap((x_b2), (y_b2), 8, 10, balazo_v);

      }
      else {
        disparo_activo2 = 0;
      }
    }

    //************************Bloque2AvanceDisparo********************************
    else if ((disparo_activo2 == 1) && (disparo_horD2 == 1)) {
      FillRect(x_b2, y_b2, 10, 8, 0);
      x_b2 += 5;
      if ((((y_b2 + 17) > y_t1) && ((y_b2) < (y_t1 + 17))) && (((x_b2 + 10) > x_t1) && ((x_b2 + 10) < (x_t1 + 17)))) {
        disparo_activo2 = 0;
        hit2 = 1;
      }
      if ((x_b2 <= (x_limsup + 10)) && hit2 == 0) {
        LCD_Bitmap((x_b2), (y_b2), 10, 8, balazo);

      }
      else {
        disparo_activo2 = 0;
      }

    }
    //************************Bloque3AvanceDisparo********************************
    else if ((disparo_activo2 == 1) && (disparo_horI2 == 1)) {
      FillRect(x_b2, y_b2, 10, 8, 0);
      x_b2 -= 5;
      if ((((y_b2 + 17) > y_t1) && ((y_b2) < (y_t1 + 17))) && (((x_b2 - 5) < x_t1 + 17) && ((x_b2) > (x_t1)))) {
        disparo_activo2 = 0;
        hit2 = 1;
      }
      if ((x_b2 >= (x_liminf)) && hit2 == 0) {
        LCD_Bitmap((x_b2), (y_b2), 10, 8, balazo);

      }
      else {
        disparo_activo2 = 0;
      }

    }
    //************************Bloque4AvanceDisparo********************************
    else if ((disparo_activo2 == 1) && (disparo_vertA2 == 1)) {
      FillRect(x_b2, y_b2, 8, 10, 0);
      y_b2 += 5;
      if ((((x_b2 + 8) > x_t1) && ((x_b2) < (x_t1 + 17))) && (((y_b2 + 10) > y_t1) && ((y_b2 + 10) < (y_t1 + 17)))) {
        disparo_activo2 = 0;
        hit2 = 1;
      }
      if ((y_b2 <= (y_liminf - 10)) && hit2 == 0) {
        LCD_Bitmap((x_b2), (y_b2), 8, 10, balazo_v);

      }
      else {
        disparo_activo2 = 0;
      }

    }

  }



}



//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER)
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40 | 0x80 | 0x20 | 0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
  //  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c) {
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
    }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//***************************************************************************************************************************************
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
    LCD_DATA(c >> 8);
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//***************************************************************************************************************************************
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8);
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y + h, w, c);
  V_line(x  , y  , h, c);
  V_line(x + w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 = x + w;
  y2 = y + h;
  SetWindows(x, y, x2 - 1, y2 - 1);
  unsigned int k = w * h * 2 - 1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);

      //LCD_DATA(bitmap[k]);
      k = k - 2;
    }
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background)
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;

  if (fontSize == 1) {
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if (fontSize == 2) {
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }

  char charInput ;
  int cLength = text.length();
  Serial.println(cLength, DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength + 1];
  text.toCharArray(char_array, cLength + 1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1) {
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2) {
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 = x + width;
  y2 = y + height;
  SetWindows(x, y, x2 - 1, y2 - 1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k + 1]);
      //LCD_DATA(bitmap[k]);
      k = k + 2;
    }
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 =   x + width;
  y2 =    y + height;
  SetWindows(x, y, x2 - 1, y2 - 1);
  int k = 0;
  int ancho = ((width * columns));
  if (flip) {
    for (int j = 0; j < height; j++) {
      k = (j * (ancho) + index * width - 1 - offset) * 2;
      k = k + width * 2;
      for (int i = 0; i < width; i++) {
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k + 1]);
        k = k - 2;
      }
    }
  } else {
    for (int j = 0; j < height; j++) {
      k = (j * (ancho) + index * width + 1 + offset) * 2;
      for (int i = 0; i < width; i++) {
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k + 1]);
        k = k + 2;
      }
    }


  }
  digitalWrite(LCD_CS, HIGH);
}

//***************************************************************************************************************************************
// FUNCIONES
//***************************************************************************************************************************************

///FUNCION PARA EL STAGE

void pista(void) {
  int k = 0;
  myFile = SD.open("RECORD.txt");
  if (myFile) {
    Serial.println("test.txt:");
    while (myFile.available()) {
      k++;
      if (k == 1) {
        record1_1 = myFile.read();
      }
      else if (k == 2) {
        record1_2 = myFile.read();
      }
      else if (k == 3) {
        record1_3 = myFile.read();
      }
      else if (k == 4) {
        record2_1 = myFile.read();
      }
      else if (k == 5) {
        record2_2 = myFile.read();
      }
      else if (k == 6) {
        record2_3 = myFile.read();
      }
      else {
        Serial.write(myFile.read());
      }
    }
    myFile.close();
  }


  record1U = num_ascii(record1_1);
  record1 = record1U;
  record2U = num_ascii(record1_2);
  record2 = record2U;
  Serial.println("AQUI BIEN");
  Serial.println(record1);
  Serial.println(record2);


  //TRANSICION
  LCD_Clear(0x00);
  FillRect(0, 0, 320, 240, 0x00);

  //INDICADORES DE VIDA
  String text5 = "P1";
  String text6 = "P2";
  LCD_Bitmap(65, 4, 83, 8, lifeb1);
  LCD_Print(text5, 48, 4, 1, 0xffff, 0x00);
  LCD_Print(text6, 180, 4, 1, 0xffff, 0x00);
  LCD_Bitmap(197, 4, 83, 8, lifeb1);


//STAGE

  //fila de abajo
  LCD_Bitmap(0, 224, 16, 16, tile);
  LCD_Bitmap(16, 224, 16, 16, tile);
  LCD_Bitmap(32, 224, 16, 16, tile);
  LCD_Bitmap(48, 224, 16, 16, tile);
  LCD_Bitmap(64, 224, 16, 16, tile);
  LCD_Bitmap(80, 224, 16, 16, tile);
  LCD_Bitmap(96, 224, 16, 16, tile);
  LCD_Bitmap(112, 224, 16, 16, tile);
  LCD_Bitmap(128, 224, 16, 16, tile);
  LCD_Bitmap(144, 224, 16, 16, tile);
  LCD_Bitmap(160, 224, 16, 16, tile);
  LCD_Bitmap(176, 224, 16, 16, tile);
  LCD_Bitmap(192, 224, 16, 16, tile);
  LCD_Bitmap(208, 224, 16, 16, tile);
  LCD_Bitmap(224, 224, 16, 16, tile);
  LCD_Bitmap(240, 224, 16, 16, tile);
  LCD_Bitmap(256, 224, 16, 16, tile);
  LCD_Bitmap(272, 224, 16, 16, tile);
  LCD_Bitmap(288, 224, 16, 16, tile);
  LCD_Bitmap(304, 224, 16, 16, tile);
  //
  //fila arriba
  LCD_Bitmap(0, 16, 16, 16, tile);
  LCD_Bitmap(16, 16, 16, 16, tile);
  LCD_Bitmap(32, 16, 16, 16, tile);
  LCD_Bitmap(48, 16, 16, 16, tile);
  LCD_Bitmap(64, 16, 16, 16, tile);
  LCD_Bitmap(80, 16, 16, 16, tile);
  LCD_Bitmap(96, 16, 16, 16, tile);
  LCD_Bitmap(112, 16, 16, 16, tile);
  LCD_Bitmap(128, 16, 16, 16, tile);
  LCD_Bitmap(144, 16, 16, 16, tile);
  LCD_Bitmap(160, 16, 16, 16, tile);
  LCD_Bitmap(176, 16, 16, 16, tile);
  LCD_Bitmap(192, 16, 16, 16, tile);
  LCD_Bitmap(208, 16, 16, 16, tile);
  LCD_Bitmap(224, 16, 16, 16, tile);
  LCD_Bitmap(240, 16, 16, 16, tile);
  LCD_Bitmap(256, 16, 16, 16, tile);
  LCD_Bitmap(272, 16, 16, 16, tile);
  LCD_Bitmap(288, 16, 16, 16, tile);
  LCD_Bitmap(304, 16, 16, 16, tile);


  //columna izq
  LCD_Bitmap(0, 16, 16, 16, tile);
  LCD_Bitmap(0, 32, 16, 16, tile);
  LCD_Bitmap(0, 48, 16, 16, tile);
  LCD_Bitmap(0, 64, 16, 16, tile);
  LCD_Bitmap(0, 80, 16, 16, tile);
  LCD_Bitmap(0, 96, 16, 16, tile);
  LCD_Bitmap(0, 112, 16, 16, tile);
  LCD_Bitmap(0, 128, 16, 16, tile);
  LCD_Bitmap(0, 144, 16, 16, tile);
  LCD_Bitmap(0, 160, 16, 16, tile);
  LCD_Bitmap(0, 176, 16, 16, tile);
  LCD_Bitmap(0, 192, 16, 16, tile);
  LCD_Bitmap(0, 208, 16, 16, tile);

  //columna derecha
  LCD_Bitmap(304, 16, 16, 16, tile);
  LCD_Bitmap(304, 32, 16, 16, tile);
  LCD_Bitmap(304, 48, 16, 16, tile);
  LCD_Bitmap(304, 64, 16, 16, tile);
  LCD_Bitmap(304, 80, 16, 16, tile);
  LCD_Bitmap(304, 96, 16, 16, tile);
  LCD_Bitmap(304, 112, 16, 16, tile);
  LCD_Bitmap(304, 128, 16, 16, tile);
  LCD_Bitmap(304, 144, 16, 16, tile);
  LCD_Bitmap(304, 160, 16, 16, tile);
  LCD_Bitmap(304, 176, 16, 16, tile);
  LCD_Bitmap(304, 192, 16, 16, tile);
  LCD_Bitmap(304, 208, 16, 16, tile);


  //VARIABLES PARA LAS MECANICAS DE LOS OBJETOS
  con1 = 0;
  con2 = 0;
  con3 = 0;
  con4 = 0;

  con1_ = 0;
  con2_ = 0;
  con3_ = 0;
  con4_ = 0;
  gano_p1 = 0;
  gano_p2 = 0;
  enable_sprites = 0;

  x_t1 = 80;
  y_t1 = 100;
  x_t2 = 160;
  y_t2 = 100;
  disparo_vertU = 0;
  disparo_vertA = 0;
  disparo_horI = 0;
  disparo_horD = 0;
  disparo_activo = 0;
  flag_orient = 0;
  disparo_vertU2 = 0;
  disparo_vertA2 = 0;
  disparo_horI2 = 0;
  disparo_horD2 = 0;
  disparo_activo2 = 0;
  flag_orient2 = 0;
  hit1 = 0;
  hit2 = 0;
  life1 = 3;
  life2 = 3;
  end_game = 0;

//TANQUES
  LCD_Bitmap(x_t1, y_t1, 15, 17, tanque2_1);
  LCD_Bitmap(x_t2, y_t2, 15, 17, tanque1_1);

  //BANDERAS PARA EL TOGGLE DE LOS BOTONES, ACCIONES Y MUSICA
  FLAG = 0;
  FLAG2 = 1;


}

//FUNCION PARA MOSTRAR PANTALLA PRINCIPAL
void ini (void) {
  LCD_Clear(0x00);
  LCD_Bitmap(0, 0, 320, 240, fondo);
  FLAG = 0;
  FLAG2 = 0;
  FLAG3 = 0;
  FLAG4 = 1; //TOGGLE DE BANDERAS
}


//FUNCION PARA EL MOSTRAR EL CHARACTER SELECT
void chara (void) {
  LCD_Clear(0x00);
  FillRect(0, 0, 320, 240, 0x00);

  //ENABLLE DE LAS MECANICAS PARA SELECCIONAR PERSONAJE
  enable_personajes = 1;
  cont_personajesp1 = 1;
  cont_personajesp2 = 1;

  //DETALLES VISUALES
  String text4 = "ELIGE A UN CUQUE";
  String text5 = "P1";
  String text6 = "P2";

  //text, x, y ,tamaño de font, color, background
  LCD_Print(text4, 40, 40, 2, 0xffff, 0x00);
  LCD_Print(text5, 70, 80, 1, 0xffff, 0x00);
  LCD_Print(text6, 240, 80, 1, 0xffff, 0x00);

  LCD_Bitmap(20, 120, 20, 20, flechaizq);
  LCD_Bitmap(112, 120, 20, 20, flechader);

  LCD_Bitmap(190, 120, 20, 20, flechaizq);
  LCD_Bitmap(282, 120, 20, 20, flechader);

  FLAG = 1;
  FLAG2 = 0;
  FLAG3 = 0;
  FLAG4 = 0; //TOGGLE DE BANDERAS
}

//FUNCION PARA MOSTRAR LA PANTALLA DE GAME OVER
void gameover(void) {
  LCD_Clear(0x00);
  Serial.println("BUG");
  Serial.println(String(record1));
  Serial.println(String(record2));
  String text4 = "GAME OVER";
  //text, x, y ,tamaño de font, color, background
  LCD_Print(text4, 90, 10, 2, 0xffff, 0x00);
  LCD_Print("SCORE", 145, 27, 1, 0xffff, 0x00);
  LCD_Print("P1", 110, 40, 1, 0xffff, 0x00);
  LCD_Print(String(record1), 140, 40, 1, 0xffff, 0x00);
  LCD_Print("-", 160, 40, 1, 0xffff, 0x00);
  LCD_Print(String(record2), 180, 40, 1, 0xffff, 0x00);
  LCD_Print("P2", 200, 40, 1, 0xffff, 0x00);
  FLAG = 0;
  FLAG2 = 0;
  FLAG3 = 1;
  FLAG4 = 0;  //TOGGLE DE BANDERAS



//FUNCIONES PARA MOSTRAR AL GANADOR

  if (gano_p1 == 1) {
    if (cont_personajesp1 == 1) {
      LCD_Print("Solid Snake Wins", 30, 170, 2, 0xffff, 0x00);
      LCD_Bitmap(134, 70, 52, 89, snake);
    }
    else if (cont_personajesp1 == 2) {
      LCD_Print("Otacon Wins", 75, 170, 2, 0xffff, 0x00);
      LCD_Bitmap(134, 70, 52, 89, otacon);
    }
    else if (cont_personajesp1 == 3) {
      LCD_Print("Meryl Wins", 80, 170, 2, 0xffff, 0x00);
      LCD_Bitmap(134, 70, 52, 89, meryl);
    }
  }
  else if (gano_p2 == 1) {
    if (cont_personajesp2 == 1) {
      LCD_Print("Liquid Snake Wins", 25, 170, 2, 0xffff, 0x00);
      LCD_Bitmap(134, 70, 52, 89, liquid);
    }
    else if (cont_personajesp2 == 2) {
      LCD_Print("Sniper Wolf Wins", 30, 170, 2, 0xffff, 0x00);
      LCD_Bitmap(134, 70, 52, 89, sniper);
    }
    else if (cont_personajesp2 == 3) {
      LCD_Print("Naomi Hunter Wins", 30, 170, 2, 0xffff, 0x00);
      LCD_Bitmap(134, 70, 52, 89, naomi);
    }
  }
  //Borrar archivo y actualizar record
  SD.remove("RECORD.txt");
  myFile = SD.open("RECORD.txt", FILE_WRITE);

  if (myFile) {
    myFile.print(String(record1));
    myFile.print(String(record2));
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening reord.txt");
  }
}

//*****************************Matrices de espacio ocupado*********************************


void ploteo2(int origenx, int origeny, int origenx2, int origeny2, int dir) {

  int origenx_k = origenx + 17;
  int origeny_k = origeny + 17;
  int origenx_D = origenx_k + 5;
  int origeny_A = origeny_k + 5;
  int origenx_I = origenx - 5;
  int origeny_U = origeny - 5;
  int matx2[17];
  int maty2[17];
  int matx[17];
  int maty[17];



  if (dir == 1) {
    if ((((origeny + 17) > origeny2) && ((origeny) < (origeny2 + 17))) && (((origenx + 22) > origenx2) && ((origenx + 22) < (origenx2 + 17)))) {

      con2_ = 1;
    }
    else {
      con2_ = 0;
    }
  }
  else if (dir == 2) {
    if ((((origeny + 17) > origeny2) && ((origeny) < (origeny2 + 17))) && (((origenx - 5) < origenx2 + 17) && ((origenx - 5) > (origenx2)))) {
      con1_ = 1;
    }
    else {
      con1_ = 0;
    }
  }
  else if (dir == 3) {
    if ((((origenx + 17) > origenx2) && ((origenx) < (origenx2 + 17))) && (((origeny + 22) > origeny2) && ((origeny + 22) < (origeny2 + 17)))) {
      con4_ = 1;
    }
    else {
      con4_ = 0;
    }
  }
  else if (dir == 0) {
    if ((((origenx + 17) > origenx2) && ((origenx) < (origenx2 + 17))) && (((origeny - 5) > origeny2) && ((origeny - 5) < (origeny2 + 17)))) {
      con3_ = 1;
    }
    else {
      con3_ = 0;
    }
  }

}

//FUNCION PARA LIMITA MOVIMIENTO DE LOS TANQUES
void ploteo(int origenx, int origeny, int origenx2, int origeny2, int dir) {

  int origenx_k = origenx + 17;
  int origeny_k = origeny + 17;
  int origenx_D = origenx_k + 5;
  int origeny_A = origeny_k + 5;
  int origenx_I = origenx - 5;
  int origeny_U = origeny - 5;
  int matx2[17];
  int maty2[17];
  int matx[17];
  int maty[17];


//ESTO EVITA QUE LOS TANQUES SE TRASPASEN

  if (dir == 1) {
    if ((((origeny + 17) > origeny2) && ((origeny) < (origeny2 + 17))) && (((origenx + 22) > origenx2) && ((origenx + 22) < (origenx2 + 17)))) {

      con2 = 1;
    }
    else {
      con2 = 0;

    }
  }
  else if (dir == 2) {
    if ((((origeny + 17) > origeny2) && ((origeny) < (origeny2 + 17))) && (((origenx - 5) < origenx2 + 17) && ((origenx - 5) > (origenx2)))) {
      con1 = 1;
    }
    else {
      con1 = 0;

    }
  }
  else if (dir == 3) {
    if ((((origenx + 17) > origenx2) && ((origenx) < (origenx2 + 17))) && (((origeny + 22) > origeny2) && ((origeny + 22) < (origeny2 + 17)))) {
      con4 = 1;
    }
    else {
      con4 = 0;

    }
  }
  else if (dir == 0) {
    if ((((origenx + 17) > origenx2) && ((origenx) < (origenx2 + 17))) && (((origeny - 5) > origeny2) && ((origeny - 5) < (origeny2 + 17)))) {
      con3 = 1;
    }
    else {
      con3 = 0;

    }
  }

}

//TABLA ASCII PARA TRADUCIR LA INFORMACION DE LA SD

int num_ascii(char num) {

  switch (num) {

    case 48:
      return 0;
      break;

    case 49:
      return 1;
      break;

    case 50:
      return 2;
      break;

    case 51:
      return 3;
      break;

    case 52:
      return 4;
      break;

    case 53:
      return 5;
      break;

    case 54:
      return 6;
      break;

    case 55:
      return 7;
      break;

    case 56:
      return 8;
      break;

    case 57:
      return 9;
      break;
  }
}

//FUNCION PARA EL CHARACTER SELECT
void character_select(int p1, int p2) {
  if (enable_personajes == 1) {
    if (p1 == 1) {
      LCD_Bitmap(50, 100, 52, 89, snake);
    }
    else if (p1 == 2) {
      LCD_Bitmap(50, 100, 52, 89, otacon);
    }
    else if (p1 == 3) {
      LCD_Bitmap(50, 100, 52, 89, meryl);
    }
    if (p2 == 1) {
      LCD_Bitmap(220, 100, 52, 89, liquid);
    }
    else if (p2 == 2) {
      LCD_Bitmap(220, 100, 52, 89, sniper);
    }
    else if (p2 == 3) {
      LCD_Bitmap(220, 100, 52, 89, naomi);
    }
  }
}
