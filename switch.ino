#include "U8glib.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#define ONE_WIRE_BUS 2

U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float temperature = 0; 
int powerState = 0;
int respondState = 0;
int respondPin = 3;
int powerPin = 4;
int mbpowerPin = 5;
int responTime = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(respondPin,INPUT_PULLUP);
  pinMode(powerPin,INPUT_PULLUP);
  pinMode(mbpowerPin,OUTPUT);
  digitalWrite(mbpowerPin, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:
  respondState = digitalRead(respondPin);
  if (respondState == LOW)
  {
    //触发感应
    if (responTime < 64)
    {
      u8g.firstPage();
      do
      {
        u8g.drawBox(0,0,responTime,15);
      }
      while(u8g.nextPage());
      responTime++;
    }
    if (responTime == 64)
    {
      //触发ps_on信号
      digitalWrite(mbpowerPin, LOW);
    }

    if (responTime > 63)
    {
      u8g.firstPage();
      do
      {
        u8g.drawBox(0,0,responTime,16);
        u8g.setFont(u8g_font_unifont);
        u8g.setPrintPos(0,28);
        u8g.print("PS_ON active");
      }
      while(u8g.nextPage());
      responTime++;
    }
  }

  if (respondState == HIGH)
  {
    //非触发感应
    responTime = 0;
    digitalWrite(mbpowerPin, HIGH);
    powerState = digitalRead(powerPin);
    if(powerState == LOW)
    {
      //关机状态
      u8g.firstPage();
      do
      {
        u8g.setFont(u8g_font_unifont);
        u8g.setPrintPos(0,12);
        u8g.print("SYS POWER OFF");
      }
      while(u8g.nextPage());
      delay(100);
    }

    if (powerState == HIGH)
    {
      //开机状态
      sensors.requestTemperatures(); 
      temperature = sensors.getTempCByIndex(0);
      u8g.firstPage();
      do
      {
        u8g.setFont(u8g_font_unifont);
        u8g.setPrintPos(0,12);
        u8g.print("SYS POWER ON");
        u8g.setPrintPos(0,28);
        u8g.print("TEMP: ");
        u8g.setPrintPos(48,28);
        u8g.print(temperature);
      }
      while(u8g.nextPage());
      delay(100);
    }
  }
} 