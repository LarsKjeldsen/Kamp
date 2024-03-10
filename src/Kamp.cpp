#include <WiFi.h>
#include "Display.h"
#include <SpotifyArduino.h>
#include <SpotifyArduinoCert.h>
#include <ArduinoJson.h>
#include <Secure.h>
#include <WiFiClientSecure.h>


TFT_eSPI tft = TFT_eSPI();
Display display = Display();

char ssid[] = SSID;         // your network SSID (name)
char password[] = WIFIPASSWORD; // your network password

char clientId[] = "2239f038fb8948c288d1204d974fb429";     // Your client ID of your spotify APP
char clientSecret[] = "cc8a8c7378ee4ab69809d60ab29f7b6b"; // Your client Secret of your spotify APP (Do Not share this!)

// Country code, including this is advisable
#define SPOTIFY_MARKET "IE"

#define SPOTIFY_REFRESH_TOKEN "AQCxLgQtJykM71wRxr3KDQ5v0cChyFKGJLUJbLZJhmY2woYktT_Lg3ySLv0NDbb4C-gomyPgFGqYysB5iQzC8i7azX8XN6--vYSofQpbw7QnLu_DSkqTopsbC8xK0pilGU0"

WiFiClientSecure client;
SpotifyArduino spotify(client, clientId, clientSecret, SPOTIFY_REFRESH_TOKEN);

unsigned long delayBetweenRequests = 60000; // Time between requests (1 minute)
unsigned long requestDueTime;               //time when request due

bool getDeviceCallback(SpotifyDevice device, int index, int numDevices);

struct SimpleDevice
{
  char name[SPOTIFY_DEVICE_NAME_CHAR_LENGTH];
  char id[SPOTIFY_DEVICE_ID_CHAR_LENGTH];
};

#define MAX_DEVICES 6

SimpleDevice deviceList[MAX_DEVICES];
int numberOfDevices = -1;



//====================================================================
void setup(void) {
  Serial.begin(115200);
  Serial.println("\n\nStarting...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //client.setCACert(spotify_server_cert);
  client.setInsecure();

 Serial.println("Refreshing Access Tokens");
  if (!spotify.refreshAccessToken())
  {
    Serial.println("Failed to get access tokens");
  }

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

//  tft.fillCircle(x, y, 5, TFT_RED);
//  tft.fillCircle(10, 50, 5, TFT_BLUE);
//  tft.fillCircle(50, 10, 5, TFT_WHITE);
  

//  tft.setCursor(30,30);
//  tft.print(touch.buttons[0].selected?"1":"0"); tft.print(":");
//  tft.print(touch.buttons[1].selected?"1":"0"); tft.print(":");
//  tft.print(touch.buttons[2].selected?"1":"0"); tft.print(":");
//  tft.print(touch.buttons[3].selected?"1":"0");
//  Serial.print('.');
 if (millis() > requestDueTime)
  {

    Serial.println("Getting devices:");
    int status = spotify.getDevices(getDeviceCallback);
    if (status == 200)
    {
      Serial.print("Successfully got devices, tranfering playback between them  : ");
      Serial.println(numberOfDevices);
      for (int i = 0; i < numberOfDevices; i++)
      {
        // You could do this transfer play back in the callback
        // But this example is more to simulate grabbing the devices
        // and having a UI to change between them
        spotify.transferPlayback(deviceList[i].id, true); //true means to play after transfer
        delay(5000);
      }
    }

    requestDueTime = millis() + delayBetweenRequests;
  }
}




void printDeviceToSerial(SpotifyDevice device)
{

  Serial.println("--------- Device Details ---------");

  Serial.print("Device ID: ");
  Serial.println(device.id);

  Serial.print("Device Name: ");
  Serial.println(device.name);

  Serial.print("Device Type: ");
  Serial.println(device.type);

  Serial.print("Is Active: ");
  if (device.isActive)
  {
    Serial.println("Yes");
  }
  else
  {
    Serial.println("No");
  }

  Serial.print("Is Resticted: ");
  if (device.isRestricted)
  {
    Serial.println("Yes, from API docs \"no Web API commands will be accepted by this device\"");
  }
  else
  {
    Serial.println("No");
  }

  Serial.print("Is Private Session: ");
  if (device.isPrivateSession)
  {
    Serial.println("Yes");
  }
  else
  {
    Serial.println("No");
  }

  Serial.print("Volume Percent: ");
  Serial.println(device.volumePercent);

  Serial.println("------------------------");
}

bool getDeviceCallback(SpotifyDevice device, int index, int numDevices)
{
  if (index == 0)
  {
    // This is a first device from this batch
    // lets set the number of devices we got back
    if (numDevices < MAX_DEVICES)
    {
      numberOfDevices = numDevices;
    }
    else
    {
      numberOfDevices = MAX_DEVICES;
    }
  }

  // We can't handle anymore than we can fit in our array
  if (index < MAX_DEVICES)
  {
    printDeviceToSerial(device);

    strncpy(deviceList[index].name, device.name, sizeof(deviceList[index].name)); //DO NOT use deviceList[index].name = device.name, it won't work as you expect!
    deviceList[index].name[sizeof(deviceList[index].name) - 1] = '\0';            //ensures its null terminated

    strncpy(deviceList[index].id, device.id, sizeof(deviceList[index].id));
    deviceList[index].id[sizeof(deviceList[index].id) - 1] = '\0';

    if (index == MAX_DEVICES - 1)
    {
      return false; //returning false stops it processing any more
    }
    else
    {
      return true;
    }
  }

  // We should never get here
  return false; //returning false stops it processing any more
}
