// DRA_U8G_16VHF

#include <string.h>
#include "U8glib.h"

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI

#define PD      9  // Power Down pin. This need to start low, then be set high before programming.

int ch, sensorValue;
int ch_old = 1;

// set parameters for DRA818V
int bw = 1;                              // bandwith in KHz ( 0 = 12.5 or 1 = 25 )
double ftx;                              // tx frequency in MHz (134.0000 - 174.0000)
double frx;                              // rx frequency in MHz (134.0000 - 174.0000)
String tx_ctcss = "0000";                // ctcss frequency ( 0000 - 0038 ); 0000 = "no CTCSS"
String rx_ctcss = "0000";                // ctcss frequency ( 0000 - 0038 ); 0000 = "no CTCSS"
String stn;
int squ = 0;                             // squelch level  ( 0 - 8 ); 0 = "open"

//////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);

  pinMode(PD, OUTPUT);
  digitalWrite(PD, HIGH);
  delay(100);
}

/////////////////////////////////////////////////////////////////
void loop()
{
  sensorValue = analogRead(0);
  ch = sensorValue / 65;  // 16 channel version (0-15)
  if (ch != ch_old)

  {
    if (ch == 0) {
      ftx = 162.4750;
      frx = 162.4750;
      squ = 0;
      tx_ctcss = "0000";
      rx_ctcss = "0000";
      stn = "LouWX";
    }
    else if (ch == 1) {
      ftx = 144.3900;
      frx = 144.3900;
      squ = 1;
      tx_ctcss = "0000";
      rx_ctcss = "0000";
      stn = "APRS";
    }
    else if (ch == 2) {
      ftx = 146.2250;
      frx = 146.2250;
      squ = 1;
      tx_ctcss = "0000";
      rx_ctcss = "0000";
      stn = "Simplex";
    }
    else if (ch == 3) {
      ftx = 146.1000;
      frx = 146.7000;
      squ = 1;
      tx_ctcss = "0005";
      rx_ctcss = "0000";
      stn = "KY4KY";
    }
    else if (ch == 4) {
      ftx = 144.8100;
      frx = 145.4100;
      squ = 1;
      tx_ctcss = "0005";
      rx_ctcss = "0000";
      stn = "KK4CZ";
    }
    
    Serial.print("AT+DMOSETGROUP=");
    Serial.print(bw, 1);
    Serial.print(",");
    Serial.print(ftx, 4);
    Serial.print(",");
    Serial.print(frx, 4);
    Serial.print(",");
    Serial.print (tx_ctcss);
    Serial.print(",");
    Serial.print(squ);
    Serial.print(",");
    Serial.println (rx_ctcss);
    Serial.print("AT+DMOSETVOLUME=3");

    u8g.firstPage();
    do {
      draw();
    } while ( u8g.nextPage() );
    delay(100);
  }
  ch_old = ch;
}

/////////////////////////////////////////////////////////////////
void draw(void) {
  // graphic commands to redraw the complete screen
  u8g.setRot180();
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(2, 10, "VHF-TRANSCEIVER");
  u8g.setPrintPos(18, 30);
  u8g.print("Ch.");
  u8g.print(ch + 1);
  u8g.setPrintPos(18, 45);
  u8g.print(frx, 3);
  u8g.print(" MHz");
  u8g.setPrintPos(18, 60);
  u8g.print(stn);
}
