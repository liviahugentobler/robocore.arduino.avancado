#include "Wire.h"
#include "ESP8266_Lib.h"
#include "BlynkSimpleShieldEsp8266.h"

char auth[] = " Txy45-g6-9qt1CrN0B_jLrWtdF2rzN92";
char ssid[] = "Hugentobler";
char pass[] = "cascavel";

// Hardware Serial on Mega, Leonardo, Micro...
//#define EspSerial Serial1

// or Software Serial on Uno, Nano...
#include "SoftwareSerial.h"
SoftwareSerial EspSerial(10, 11); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

float giro_y; // aceleração angular eixo Y

void setup()
{
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);

  Wire.begin();
  Wire.beginTransmission(0x68); // T
  Wire.write(0x6B); //  registrador PWR_MGMT_1
  Wire.write(0); // seta para zero
  Wire.endTransmission(true); // encerra transmissão

}

void loop()
{
  Blynk.run();

  // Inicia transmissão de dados para o MPU-6050
  Wire.beginTransmission(0x68);
  Wire.write(0x45); // registrador ACCEL_XOUT_H
  Wire.endTransmission(false); // encerra transmissão
  Wire.requestFrom(0x68, 2, true);
  giro_y = Wire.read() << 8 | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  giro_y = giro_y / 131.0; // fator de conversão para escala de 250º/s

  if (giro_y > 2) {
    Blynk.notify("ALERTA: MOVIMENTO DETECTADO!");
  }
}
