#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#include <Wire.h>
#include <FT6236.h>

#define I2C_SCL 15
#define I2C_SDA 16

#define TS_RESET 13


class CButton  {
public:
    CButton(const char c[]) :  text(c), selected(false) { button = new TFT_eSPI_Button(); } 
    CButton() : text("") {}

    TFT_eSPI_Button *button;
    const char *text;
    bool selected;
};


class Touch : FT6236
{
private:
    FT6236 ts = FT6236();
    #define NUM_BUTTON 4
    public: CButton buttons[NUM_BUTTON];
    TFT_eSPI_Button SoundButton;
//    const char* button_names[];

public:
    Touch();
    ~Touch();
    void begin(TFT_eSPI tft);
    TS_Point Check_buttons();
};
