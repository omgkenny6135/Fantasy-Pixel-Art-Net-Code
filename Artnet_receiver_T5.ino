#include <Adafruit_NeoPixel.h>
//#include <SPI.h>
const int numLeds = 360;
const byte dataPin = D2;
Adafruit_NeoPixel leds = Adafruit_NeoPixel(numLeds, dataPin, NEO_GRB + NEO_KHZ800);

#include <Artnet.h>

// WiFi stuff
const char* ssid = "KennyWifi";
const char* pwd = "kenny6135";
const IPAddress ip(192, 168, 0, 121);
const IPAddress gateway(192, 168, 0, 1);
const IPAddress subnet(255, 255, 255, 0);

ArtnetWiFiReceiver artnet;
uint8_t art_universe = 0;
uint8_t art_net = 10;
uint8_t art_subnet = 0;

void setup() {
    Serial.begin(115200);
    delay(2000);

    leds.begin();

    // WiFi stuff
    WiFi.begin(ssid, pwd);
    WiFi.config(ip, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.print("WiFi connected, IP = ");
    Serial.println(WiFi.localIP());

    artnet.begin(art_net, art_subnet);

     artnet.subscribe(art_universe, [&](const uint8_t* data, const uint16_t size) {
      
         for (int i = 0; i < 60; ++i) {

            leds.setPixelColor(i, data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);

            leds.setPixelColor(i + 60, data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);

            leds.setPixelColor(i + 120, data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);

            leds.setPixelColor(i + 180, data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);

            leds.setPixelColor(i + 240, data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);

            leds.setPixelColor(i + 300, data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);

         }
         leds.show();
     });
}

void loop() {
    artnet.parse();  // check if artnet packet has come and execute callback
}
