#include <Arduino.h>
#include <TFT_eSPI.h>

//#include <Wire.h>
//#include <FT6236.h>

#define I2C_SCL 15
#define I2C_SDA 16

#define TS_RESET 13

#define but_h 45
#define but_w 120
#define but_x 350
#define but_space 5

class CButton
{
public:
    CButton(const char c[]) : text(c), selected(false), x(0), y(0), h(0), w(0) { button = new TFT_eSPI_Button(); }
    CButton() : text("") {}

    TFT_eSPI_Button *button;
    char const * GetText() { return text;}
    void SetSelected(bool s) {selected = s;}

    int16_t x;
    int16_t y;
    int16_t h;
    int16_t w;

private: 
    bool selected;
    const char *text;

};

class Display //: FT6236
{
private:
//    FT6236 ts = FT6236();
#define NUM_BUTTON 6
public:
    CButton buttons[NUM_BUTTON];
    TFT_eSPI_Button SoundButton;
    //    const char* button_names[];
    void Select_button(CButton);
public:
    TFT_eSPI *_tft;
    Display();
    ~Display();
    void begin(TFT_eSPI *tft);
    void SetbuttonMark(int but);
    int32_t Check_buttons(int32_t *x, int32_t *y);
};
