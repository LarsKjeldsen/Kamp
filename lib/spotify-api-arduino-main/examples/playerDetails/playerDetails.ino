/*******************************************************************
    Prints info about your currently active spotify device
    on the serial monitor using an ES32 or ESP8266

    NOTE: You need to get a Refresh token to use this example
    Use the getRefreshToken example to get it.

    Compatible Boards:
	  - Any ESP8266 or ESP32 board

    Parts:
    ESP32 D1 Mini style Dev board* - http://s.click.aliexpress.com/e/C6ds4my

 *  * = Affiliate

    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/


    Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

// ----------------------------
// Standard Libraries
// ----------------------------

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

#include <WiFiClientSecure.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <SpotifyArduino.h>
// Library for connecting to the Spotify API

// Install from Github
// https://github.com/witnessmenow/spotify-api-arduino

// including a "spotify_server_cert" variable
// header is included as part of the SpotifyArduino libary
#include <SpotifyArduinoCert.h>

#include <ArduinoJson.h>
// Library used for parsing Json from the API responses

// Search for "Arduino Json" in the Arduino Library manager
// https://github.com/bblanchon/ArduinoJson

//------- Replace the following! ------

char ssid[] = "SSID";         // your network SSID (name)
char password[] = "password"; // your network password

char clientId[] = "56t4373258u3405u43u543";     // Your client ID of your spotify APP
char clientSecret[] = "56t4373258u3405u43u543"; // Your client Secret of your spotify APP (Do Not share this!)

// Country code, including this is advisable
#define SPOTIFY_MARKET "IE"

#define SPOTIFY_REFRESH_TOKEN "AAAAAAAAAABBBBBBBBBBBCCCCCCCCCCCDDDDDDDDDDD"

//------- ---------------------- ------

WiFiClientSecure client;
SpotifyArduino spotify(client, clientId, clientSecret, SPOTIFY_REFRESH_TOKEN);

unsigned long delayBetweenRequests = 60000; // Time between requests (1 minute)
unsigned long requestDueTime;               //time when request due

void setup()
{

    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
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

    // Handle HTTPS Verification
#if defined(ESP8266)
    client.setFingerprint(SPOTIFY_FINGERPRINT); // These expire every few months
#elif defined(ESP32)
    client.setCACert(spotify_server_cert);
#endif
    // ... or don't!
    //client.setInsecure();

    // If you want to enable some extra debugging
    // uncomment the "#define SPOTIFY_DEBUG" in SpotifyArduino.h

    Serial.println("Refreshing Access Tokens");
    if (!spotify.refreshAccessToken())
    {
        Serial.println("Failed to get access tokens");
    }
}

void printPlayerDetailsToSerial(PlayerDetails playerDetails)
{
    Serial.println("--------- Player Details ---------");

    Serial.print("Device ID: ");
    Serial.println(playerDetails.device.id);

    Serial.print("Device Name: ");
    Serial.println(playerDetails.device.name);

    Serial.print("Device Type: ");
    Serial.println(playerDetails.device.type);

    Serial.print("Is Active: ");
    if (playerDetails.device.isActive)
    {
        Serial.println("Yes");
    }
    else
    {
        Serial.println("No");
    }

    Serial.print("Is Resticted: ");
    if (playerDetails.device.isRestricted)
    {
        Serial.println("Yes, from API docs \"no Web API commands will be accepted by this device\"");
    }
    else
    {
        Serial.println("No");
    }

    Serial.print("Is Private Session: ");
    if (playerDetails.device.isPrivateSession)
    {
        Serial.println("Yes");
    }
    else
    {
        Serial.println("No");
    }

    Serial.print("Volume Percent: ");
    Serial.println(playerDetails.device.volumePercent);

    Serial.print("Progress (Ms): ");
    Serial.println(playerDetails.progressMs);

    Serial.print("Is Playing: ");
    if (playerDetails.isPlaying)
    {
        Serial.println("Yes");
    }
    else
    {
        Serial.println("No");
    }

    Serial.print("Shuffle State: ");
    if (playerDetails.shuffleState)
    {
        Serial.println("On");
    }
    else
    {
        Serial.println("Off");
    }

    Serial.print("Repeat State: ");
    switch (playerDetails.repeateState)
    {
    case repeat_track:
        Serial.println("track");
        break;
    case repeat_context:
        Serial.println("context");
        break;
    case repeat_off:
        Serial.println("off");
        break;
    }

    Serial.println("------------------------");
}

void loop()
{
    if (millis() > requestDueTime)
    {
        Serial.print("Free Heap: ");
        Serial.println(ESP.getFreeHeap());

        Serial.println("Getting player Details:");
        // Market can be excluded if you want e.g. spotify.getPlayerDetails()
        int status = spotify.getPlayerDetails(printPlayerDetailsToSerial, SPOTIFY_MARKET);
        if (status == 200)
        {
            Serial.println("Successfully got player details");
        }
        else if (status == 204)
        {
            Serial.println("No active player?");
        }
        else
        {
            Serial.print("Error: ");
            Serial.println(status);
        }

        requestDueTime = millis() + delayBetweenRequests;
    }
}
