
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
int entrada;
int x_t1;
int y_t1;
int x_b1;
int y_b1;
int x_t2;
int y_t2;
int x_b2;
int y_b2;

int con1 = 0;
int con2 = 0;
int con3 = 0;
int con4 = 0;

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

int flag_orient2;
int disparo_vertU2;
int disparo_vertA2;
int disparo_horI2;
int disparo_horD2;
int disparo_activo2;

uint8_t BO1;
uint8_t BO2;
uint8_t BO3;
uint8_t BO4;
uint8_t BO5;
uint8_t BO6;
uint8_t BO7; //SOLO ES DE PRUEBA QUITAR DESPUES
uint8_t fon;
uint8_t fon1;
uint8_t FLAG;
uint8_t FLAG2;
uint8_t FLAG3;
uint8_t FLAGO;
uint8_t FLAGO1;
uint8_t FLAGO2;
uint8_t FLAGO3;
uint8_t FLAGO4;
uint8_t FLAGO5;
uint8_t FLAGO6;
uint8_t FINAL;

//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
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
//Indice de la imagen, 0, 0

extern uint8_t fondo[];
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  Serial3.begin(9600);
  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);

  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial2.begin(9600);
  pinMode(PF_4, INPUT_PULLUP);
  pinMode(PA_6, INPUT_PULLUP);
  pinMode(PA_7, INPUT_PULLUP);
  pinMode(PF_1, INPUT_PULLUP);
  pinMode(PE_3, INPUT_PULLUP);
  pinMode(PE_5, INPUT_PULLUP);
  pinMode(PF_3, INPUT_PULLUP);
  pinMode(PC_4, INPUT);
  pinMode(PC_5, OUTPUT);



  Serial.println("Inicio");
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
  LCD_Print(text3, 0, 140, 2, 0xffff, 0x00);
  delay (100);

  LCD_Clear(0x00);

  String text4 = "PRESENT";
  //text, x, y ,tamaño de font, color, background
  LCD_Print(text4, 100, 100, 2, 0xffff, 0x00);
  delay (3000);
  ini();





//



  //LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);

  //LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  x_t1 = 80;
  y_t1 = 100;
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



}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {

//***************************************************************************************************************************************
// BOTONES
//***************************************************************************************************************************************


  BO1 = digitalRead(PA_7);
  BO2 = digitalRead(PA_6);
  BO3 = digitalRead(PF_4);
  BO4 = digitalRead(PF_1);
  BO5 = digitalRead(PE_5);
  BO6 = digitalRead(PE_3);
  BO7 = digitalRead(PF_3);
  if (FLAG == 1) {
    if (BO1 == LOW) {
      fon = 1;
    }
    if (fon == 1 && BO1 == HIGH) {
      fon = 0;
      pista();
    }

  }

  /*for (int x = 0; x<6; x++){
    if(x == 6){
      x = 0;
      delay(100);
    }
    LCD_Sprite(30, 30, 17, 17, explo, 6, x, 0, 0);*/


//****************************************************************************************
//*Movimiento
//***********************************************************************************
  if (FLAG2 == 1) {
    if (BO1 == LOW) {
      FLAGO = 1;
    }

    if (FLAGO == 1 && BO1 == HIGH) {
      FLAGO = 0;
      entrada = 48;

    }
  }

  if (FLAG2 == 1) {
    if (BO2 == LOW) {
      FLAGO1 = 1;
    }

    if (FLAGO1 == 1 && BO2 == HIGH) {
      FLAGO1 = 0;
      entrada = 49;

    }
  }

  if (FLAG2 == 1) {
    if (BO3 == LOW) {
      FLAGO2 = 1;
    }

    if (FLAGO2 == 1 && BO3 == HIGH) {
      FLAGO2 = 0;
      entrada = 50;

    }
  }

  if (FLAG2 == 1) {
    if (BO4 == LOW) {
      FLAGO3 = 1;
    }

    if (FLAGO3 == 1 && BO4 == HIGH) {
      FLAGO3 = 0;
      entrada = 51;

    }
  }
  if (FLAG2 == 1) {
    if (BO5 == LOW) {
      FLAGO4 = 1;
    }
    if (FLAGO4 == 1 && BO5 == HIGH) {
      FLAGO4 = 0;
      entrada = 52;

    }
  }

  if (FLAG2 == 1) {
    if (BO6 == LOW) {
      FLAGO5 = 1;
    }

    if (FLAGO5 == 1 && BO6 == HIGH) {
      FLAGO5 = 0;
      entrada = 53;

    }
  }

   if (FLAG2 == 1) {
    if (BO7 == LOW) {
      FLAGO6 = 1;
    }

    if (FLAGO6 == 1 && BO7 == HIGH) { //SOLO ES DE PRUEBA QUITAR DESPUES
      FLAGO6 = 0;
      gameover();

    }
  }

  if (FLAG3 == 1) {
    if (BO2 == LOW) {
      fon1 = 1;
    }
    if (fon1 == 1 && BO2 == HIGH) {
      fon1 = 0;
      ini();
    }

  }
    //*****************************Matrices de espacio ocupado*********************************
    ploteo(x_t1, y_t1, x_t2, y_t2, flag_orient);

    //***********************Movimiento***************************************
   //************************Bloque1Movimiento********************************
    if (entrada == 48 && flag_orient == 0 && con3 == 0) {
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
      FillRect(x_t1, y_t1, 17, 17, 0);
      LCD_Bitmap(x_t1, y_t1, 15, 17, tanque2_1);
      flag_orient = 0;
    }
    //************************Bloque2Direccion********************************
    else if (entrada == 50) {
      FillRect(x_t1, y_t1, 17, 17, 0);
      LCD_Bitmap(x_t1, y_t1, 17, 15, tanque2_2);
      flag_orient = 1;
    }
    //************************Bloque3Direccion********************************
    else if (entrada == 51) {
      FillRect(x_t1, y_t1, 17, 17, 0);
      LCD_Bitmap(x_t1, y_t1, 17, 15, tanque2_3);
      flag_orient = 2;
    }
    //************************Bloque4Direccion********************************
    else if (entrada == 52) {
      FillRect(x_t1, y_t1, 17, 17, 0);
      LCD_Bitmap(x_t1, y_t1, 15, 17, tanque2_4);
      flag_orient = 3;
    }
    //***********************Disparo***************************************
    //************************Bloque1Disparo********************************
    if ((entrada == 53 ) && ((flag_orient == 0) && (disparo_activo == 0))) {
      disparo_vertU = 1;
      disparo_vertA = 0;
      disparo_horI = 0;
      disparo_horD = 0;

      if ((y_t1 - 12) >= y_limsup) {
        disparo_activo = 1;
        LCD_Bitmap((x_t1 + 4), (y_t1 - 12), 8, 10, balazo_v);
        x_b1 = x_t1 + 4;
        y_b1 = y_t1 - 12;
      }
      else {
        disparo_activo = 0;
      }
    }
    //************************Bloque2Disparo********************************
    else if ((entrada == 53 ) && ((flag_orient == 1) && (disparo_activo == 0))) {
      disparo_vertU = 0;
      disparo_vertA = 0;
      disparo_horI = 0;
      disparo_horD = 1;
      disparo_activo = 1;

      if ((x_t1 + 19) <= x_limsup) {
        disparo_activo = 1;
        LCD_Bitmap((x_t1 + 18), (y_t1 + 4), 10, 8, balazo);
        x_b1 = x_t1 + 18;
        y_b1 = y_t1 + 4;
      }
      else {
        disparo_activo = 0;
      }

    }
    //************************Bloque3Disparo********************************
    else if ((entrada == 53 ) && ((flag_orient == 2) && (disparo_activo == 0))) {
      disparo_vertU = 0;
      disparo_vertA = 0;
      disparo_horI = 1;
      disparo_horD = 0;
      disparo_activo = 1;

      if ((x_t1 - 12) <= x_limsup) {
        disparo_activo = 1;
        LCD_Bitmap((x_t1 - 12), (y_t1 + 4), 10, 8, balazo);
        x_b1 = x_t1 - 12;
        y_b1 = y_t1 + 4;
      }
      else {
        disparo_activo = 0;
      }
//************************Bloque4Disparo********************************
    }
    else if ((entrada == 53 ) && ((flag_orient == 3) && (disparo_activo == 0))) {
      disparo_vertU = 0;
      disparo_vertA = 1;
      disparo_horI = 0;
      disparo_horD = 0;
      disparo_activo = 1;

      if ((y_t1 + 27) <= y_liminf) {
        disparo_activo = 1;
        LCD_Bitmap((x_t1 + 4), (y_t1 + 18), 8, 10, balazo_v);
        x_b1 = x_t1 + 4;
        y_b1 = y_t1 + 18;
      }
      else {
        disparo_activo = 0;
      }
    }
    //------->Avance Disparo
    //************************Bloque1AvanceDisparo********************************
    if ((disparo_activo == 1) && (disparo_vertU == 1)) {
      FillRect(x_b1, y_b1, 8, 10, 0);
      y_b1 -= 5;
      if (y_b1 >= y_limsup) {
        LCD_Bitmap((x_b1), (y_b1), 8, 10, balazo_v);
        delay(100);
      }
      else {
        disparo_activo = 0;
      }
    }

    //************************Bloque2AvanceDisparo********************************
    else if ((disparo_activo == 1) && (disparo_horD == 1)) {
      FillRect(x_b1, y_b1, 10, 8, 0);
      x_b1 += 5;
      if (x_b1 <= (x_limsup + 10)) {
        LCD_Bitmap((x_b1), (y_b1), 10, 8, balazo);
        delay(100);
      }
      else {
        disparo_activo = 0;
      }

    }
    //************************Bloque3AvanceDisparo********************************
    else if ((disparo_activo == 1) && (disparo_horI == 1)) {
      FillRect(x_b1, y_b1, 10, 8, 0);
      x_b1 -= 5;
      if (x_b1 >= (x_liminf)) {
        LCD_Bitmap((x_b1), (y_b1), 10, 8, balazo);
        delay(100);
      }
      else {
        disparo_activo = 0;
      }

    }
    //************************Bloque4AvanceDisparo********************************
    else if ((disparo_activo == 1) && (disparo_vertA == 1)) {
      FillRect(x_b1, y_b1, 8, 10, 0);
      y_b1 += 5;
      if (y_b1 <= (y_liminf - 10)) {
        LCD_Bitmap((x_b1), (y_b1), 8, 10, balazo_v);
        delay(100);
      }
      else {
        disparo_activo = 0;
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
// SUBRUTINAS
//***************************************************************************************************************************************


void pista(void) {


  LCD_Clear(0x00);
  FillRect(0, 0, 320, 240, 0x00);

  String text5 = "P1";
  String text6 = "P2";
  //LCD_Bitmap(50, 4, 83, 8, life1);
  //LCD_Bitmap(150, 4, 60, 8, life2);
  //LCD_Bitmap(215, 4, 36, 8, life3);
  //LCD_Bitmap(255, 4, 16, 8, life4);
  LCD_Print(text5, 48, 4, 1, 0xffff, 0x00);




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

  LCD_Bitmap(x_t1, y_t1, 15, 17, tanque2_1);
  LCD_Bitmap(x_t2, y_t2, 15, 17, tanque1_1);

  FLAG = 0;
  FLAG2 = 1;
  Serial2.println(6, DEC);
  //delay(10000);
 // gameover();


  /* LCD_Bitmap(30, 10, 17, 15, tanque2_2);
    LCD_Bitmap(60, 10, 17, 15, tanque2_3);
    LCD_Bitmap(80, 10, 15, 17, tanque2_4);
    LCD_Bitmap(97, 10, 10, 8, misil1);*/

  /*
    for(int x = 0; x <319; x++){
      LCD_Bitmap(x, 52, 16, 16, tile2);
      LCD_Bitmap(x, 68, 16, 16, tile);

      LCD_Bitmap(x, 207, 16, 16, tile);
      LCD_Bitmap(x, 223, 16, 16, tile);
      x += 15;

    }*/
}

void ini (void) {
  LCD_Clear(0x00);
  LCD_Bitmap(0, 0, 320, 240, fondo);
  FLAG = 1;
  FLAG2 = 0;
  FLAG3 = 0;
}


void gameover(void) {
  LCD_Clear(0x00);

  String text4 = "GAME OVER";
  //text, x, y ,tamaño de font, color, background
  LCD_Print(text4, 100, 100, 2, 0xffff, 0x00);
  FLAG2 = 0;
  FLAG3 = 1;
}

//*****************************Matrices de espacio ocupado*********************************
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



    if (dir == 1) {
      if ((((origeny+17)>origeny2)&&((origeny)<(origeny2+17)))&&(((origenx+22)>origenx2)&&((origenx+22)<(origenx2+17)))) {

        con2 = 1;
      }
      else {
        con2=0;
      }
    }
    else if(dir==2){
      if((((origeny+17)>origeny2)&&((origeny)<(origeny2+17)))&&(((origenx-5)<origenx2+17)&&((origenx-5)>(origenx2)))){
        con1=1;
      }
      else{
        con1=0;
      }
    }
    else if(dir==3){
      if((((origenx+17)>origenx2)&&((origenx)<(origenx2+17)))&&(((origeny+22)>origeny2)&&((origeny+22)<(origeny2+17)))){
        con4=1;
      }
      else{
        con4=0;
      }
    }
    else if(dir==0){
      if((((origenx+17)>origenx2)&&((origenx)<(origenx2+17)))&&(((origeny-5)>origeny2)&&((origeny-5)<(origeny2+17)))){
        con3=1;
      }
      else{
        con3=0;
      }
    }

}
