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

    buttons[0] = CButton("Spotify", true);
    buttons[1] = CButton("Tape", true);
    buttons[2] = CButton("AUX1", true);
    buttons[3] = CButton("AUX2", true);
    buttons[4] = CButton("Sound", false);
    buttons[5] = CButton("Setup", false);

    for (int i = 0; i < NUM_BUTTON; i++)
    {
        Serial.printf("Button %d setup : %s\n", i, buttons[i].GetText());

        buttons[i].button->initButtonUL(tft, but_x, i * (but_h + but_space) + 10, but_w, but_h, TFT_BLUE, TFT_SKYBLUE, TFT_BLACK, (char *)buttons[i].GetText(), 1);
        buttons[i].x = but_x;
        buttons[i].y = i * (but_h + but_space) + 10;
        buttons[i].h = but_h;
        buttons[i].w = but_w;
        

        buttons[i].button->drawButton();
        buttons[i].button->press(false);
    }
}


void Display::SetbuttonMark(int but)    
{
    uint8_t r = min(buttons[but].w, buttons[but].h) / 4; // Corner radius

    if (buttons[but].menu)
    {
        for (int i=0; i<NUM_BUTTON; i++)
        {
            if (i==but)
                _tft->fillRoundRect(buttons[i].x - 10, buttons[i].y , 5, buttons[i].h, r, TFT_RED);
            else
                _tft->fillRoundRect(buttons[i].x - 10, buttons[i].y , 5, buttons[i].h, r, TFT_BLACK);
        }
    }
}



int32_t Display::Check_buttons(int32_t *ret_x, int32_t *ret_y)
{
    uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
                               //        uint16_t x, y;

    bool pressed = false; // Make sure we not use last loop's touch

    if (_tft->getTouch(&t_x, &t_y) != false)
    {
        // Retrieve a point

        // Flip things around so it matches our screen rotation
        int32_t y = map(t_y, 0, 320, 320, 0);
        t_y = y;

        *ret_x = t_x;
        *ret_y = t_y;
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
            SetbuttonMark(i);
            return i;
        }
        pressed = false;
    }
    return 0;
}
