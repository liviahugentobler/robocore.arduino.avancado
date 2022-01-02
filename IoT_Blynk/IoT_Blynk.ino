#define BLYNK_PRINT Serial

#include "ESP8266_Lib.h"
#include "BlynkSimpleShieldEsp8266.h"

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = " Txy45-g6-9qt1CrN0B_jLrWtdF2rzN92";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Hugentobler ";
char pass[] = "cascavel";

// Hardware Serial on Mega, Leonardo, Micro...
//#define EspSerial Serial1

// or Software Serial on Uno, Nano...
#include "SoftwareSerial.h"
SoftwareSerial EspSerial(10, 11); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);


void setup()
{
  // Debug console
  Serial.begin(9600);

  delay(10);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);

}

void loop()
{
  Blynk.run();

  int valorPot = analogRead(A0);
  if (valorPot >= 1020) {
    Blynk.notify("Valor do potenciometro maior ou igual a 1020");
  }
}
