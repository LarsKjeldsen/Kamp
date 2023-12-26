#include <touch.h>

#define SOUNDTEXT "Sound"

Touch::Touch()
{
}

Touch::~Touch()
{
}

void Touch::begin(TFT_eSPI tft)
{
    ts.begin(40, I2C_SDA, I2C_SCL);

    // FT6236 reset pin - drive high
    pinMode(TS_RESET, OUTPUT);
    digitalWrite(TS_RESET, HIGH);

    SoundButton.initButtonUL(&tft, 350, 280, 100, 50, TFT_DARKGREY, TFT_LIGHTGREY, TFT_BLACK, SOUNDTEXT, 1);
    SoundButton.drawButton();
    SoundButton.press(false);

    buttons[0] = CButton("Spotify");
    buttons[1] = CButton("Tape");
    buttons[2] = CButton("AUX1");
    buttons[3] = CButton("AUX2");
    //          "Spotify", "Tape", "AUX1", "AUX2"};

    for (int i = 0; i < NUM_BUTTON; i++)
    {
        Serial.println("Button setup ");
        Serial.println(i);

        buttons[i].button->initButtonUL(&tft, 350, (i * 55) + 10, 100, 50, TFT_DARKGREY, TFT_LIGHTGREY, TFT_BLACK, (char *)buttons[i].text, 1);
        buttons[i].button->drawButton();
        buttons[i].button->press(false);
    }
}

TS_Point Touch::Check_buttons()
{
    uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
                               //        uint16_t x, y;

    bool pressed = false; // Make sure we not use last loop's touch

    if (ts.touched())
    {
        // Retrieve a point
        TS_Point p = ts.getPoint();

        // Flip things around so it matches our screen rotation
        p.x = map(p.x, 0, 320, 320, 0);
        t_y = p.x;
        t_x = p.y;

        pressed = (p == TS_Point(0, 0, 0));

        for (int i = 0; i < NUM_BUTTON; i++)
        {
            buttons[i].selected = false;
            if (buttons[i].button->contains(t_x, t_y))
            {
                buttons[i].button->press(true); // tell the button it is pressed
                pressed = true;
                Serial.print("Button pressed ");
                Serial.println(buttons[i].text);
                //                buttons[i].button->drawButton(true, buttons[i].text);
                buttons[i].selected = true;
            }

            if (SoundButton.contains(t_x, t_y))
            {
                SoundButton.press(true); // tell the button it is pressed
                pressed = true;
                Serial.print("Sound pressed ");
            }
        }

        if (pressed)
        {
            for (int j = 0; j < NUM_BUTTON; j++)
                if (buttons[j].selected == false)
                {
                    // buttons[j].button->drawButton(false, buttons[j].text);
                    buttons[j].button->press(false); // tell the button it is NOT pressed
                    buttons[j].button->drawButton(false, buttons[j].text);
                }

            SoundButton.press(false); // tell the button it is NOT pressed
            Serial.print("Sound released ");
        }
    }
    return TS_Point(t_x, t_y, 0);
}
