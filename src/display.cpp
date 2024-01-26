#include <Display.h>


Display::Display()
{
}

Display::~Display()
{
}

void Display::begin(TFT_eSPI *tft)
{
    _tft = tft;
//    ts.begin(40, I2C_SDA, I2C_SCL);

    // FT6236 reset pin - drive high
//    pinMode(TS_RESET, OUTPUT);
//    digitalWrite(TS_RESET, HIGH);

    buttons[0] = CButton("Spotify");
    buttons[1] = CButton("Tape");
    buttons[2] = CButton("AUX1");
    buttons[3] = CButton("AUX2");
    buttons[4] = CButton("Sound");
    buttons[5] = CButton("Setup");

    for (int i = 0; i < NUM_BUTTON; i++)
    {
        Serial.printf("Button %d setup : %s\n", i, buttons[i].GetText());

        buttons[i].button->initButtonUL(tft, but_x, i * (but_h + but_space) + 10, but_w, but_h, TFT_BLUE, TFT_SKYBLUE, TFT_BLACK, (char *)buttons[i].GetText(), 1);

        buttons[i].button->drawButton();
        buttons[i].button->press(false);
    }
}


void Display::SetbuttonMark(CButton but)
{
    uint8_t r = min(but.get_w(), but.get_h()) / 4; // Corner radius

    _tft->fillRoundRect(but_x - 10, but.get_y() , 5, but.get_h(), r, TFT_RED);
}


int32_t Display::Check_buttons(int32_t *x, int32_t *y)
{
    uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
                               //        uint16_t x, y;

    bool pressed = false; // Make sure we not use last loop's touch

    if (_tft->getTouch(&t_x, &t_y) != false)
    {
        // Retrieve a point

        // Flip things around so it matches our screen rotation
        int32_t x = map(t_x, 0, 320, 320, 0);
        t_x = t_y;
        t_y = x;
    }

    for (int i = 0; i < NUM_BUTTON; i++)
    {
        if (buttons[i].button->contains(t_x, t_y))
        {
            pressed = true;
            buttons[i].button->press(true); // tell the button it is pressed
            buttons[i].SetSelected(true);
        }
        else
        {
            buttons[i].button->press(false);
            buttons[i].SetSelected(false);
        }

        if (buttons[i].button->justPressed())
        {
            Serial.print("Button Justpressed  ");
            Serial.println(buttons[i].GetText());
            buttons[i].button->drawButton(true, buttons[i].GetText());
        }

        if (buttons[i].button->justReleased())
        {
            Serial.print("Button Justreleased  ");
            Serial.println(buttons[i].GetText());
            buttons[i].button->drawButton(false, buttons[i].GetText());
            buttons[i].button->press(false);
            SetbuttonMark(buttons[i]);
            return i;
        }
        pressed = false;
    }
    return 0;
}
