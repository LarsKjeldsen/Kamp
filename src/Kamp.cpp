#include <touch.h>
#include "NotoSansBold15.h"

TFT_eSPI tft = TFT_eSPI();
Touch touch = Touch();


//====================================================================
void setup(void) {
  Serial.begin(115200);
  Serial.println("\n\nStarting...");

  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
 // tft.setFreeFont(&FreeSansBold18pt7b);
//  tft.loadFont(NotoSansBold15);
  tft.setTextFont(4);

  touch.begin(tft);
}

//====================================================================

void loop() 
{
  TS_Point p = touch.Check_buttons();

  tft.fillCircle(p.x, p.y, 5, TFT_RED);

  tft.setCursor(30,30);
  tft.print(touch.buttons[0].selected?"1":"0"); tft.print(":");
  tft.print(touch.buttons[1].selected?"1":"0"); tft.print(":");
  tft.print(touch.buttons[2].selected?"1":"0"); tft.print(":");
  tft.print(touch.buttons[3].selected?"1":"0");
  
  delay(100);
}

