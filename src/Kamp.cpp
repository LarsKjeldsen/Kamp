#include "Display.h"
//#include "NotoSansBold15.h"

TFT_eSPI tft = TFT_eSPI();
Display display = Display();


//====================================================================
void setup(void) {
  Serial.begin(115200);
  Serial.println("\n\nStarting...");
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  tft.setFreeFont(&FreeSansBold12pt7b);
//  tft.loadFont(NotoSansBold15);
//  tft.setTextFont(4);
  display.begin(&tft);
}

//====================================================================

void loop() 
{
  int x, y;
  display.Check_buttons(&x, &y);

  tft.fillCircle(x, y, 5, TFT_RED);
  tft.fillCircle(10, 50, 5, TFT_BLUE);
  tft.fillCircle(50, 10, 5, TFT_WHITE);
  

  tft.setCursor(30,30);
//  tft.print(touch.buttons[0].selected?"1":"0"); tft.print(":");
//  tft.print(touch.buttons[1].selected?"1":"0"); tft.print(":");
//  tft.print(touch.buttons[2].selected?"1":"0"); tft.print(":");
//  tft.print(touch.buttons[3].selected?"1":"0");
//  Serial.print('.');
  delay(100);
}

