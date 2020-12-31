#include <Adafruit_NeoPixel.h>
#include <Artnet.h>

const int numLeds = 100;
const byte dataPin = 9;
Adafruit_NeoPixel leds = Adafruit_NeoPixel(numLeds, dataPin, NEO_GRBW + NEO_KHZ800);

// Ethernet stuff
const IPAddress ip(192, 168, 0, 164);
uint8_t mac[] = {0x67, 0x89, 0xAB, 0x01, 0x23, 0x45};
IPAddress myDns(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

ArtnetReceiver artnet;
byte NetID = {0};
byte SubnetID = {0};
byte UniverseID = {0};
short universe = (NetID * 256 + SubnetID*16 + UniverseID);

void setup() {
  Ethernet.init(53);
  leds.begin();
  delay(2000);

  Ethernet.begin(mac, ip, myDns, gateway, subnet);
  IPAddress newSubnet(255, 255, 255, 0);
  Ethernet.setSubnetMask(newSubnet);
  //Ethernet.begin(mac, ip);
  artnet.begin();

  // if Artnet packet comes to this universe, this function is called
  artnet.subscribe(universe, [&](const uint8_t* data, const uint16_t size) {
    for (int i = 0; i < numLeds; ++i) {

      int RDC = data[i * 3];  //RedDataChannel
      int GDC = data[i * 3 + 1];  //GreenDataChannel
      int BDC = data[i * 3 + 2];  //BlueDataChannel
      
      int MIN;
        MIN = RDC;
        if(GDC < MIN){
          MIN = GDC;
        }
        if(BDC < MIN){
          MIN = BDC;
        }

        leds.setPixelColor(i, RDC - MIN, GDC - MIN, BDC - MIN, MIN);
    }
    leds.show();
  });

}

void loop() {
  artnet.parse();  // check if artnet packet has come and execute callback
}
