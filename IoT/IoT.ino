#include "SoftwareSerial.h"

SoftwareSerial ESP_Serial(10, 11); // RX, TX

String rede = "WiFi-ESP8266"; //nome da rede a ser criada
String senha = "0123456789"; //senha da rede
String resposta = "";
  
void setup() {
  Serial.begin(9600);
  ESP_Serial.begin(9600);

  Serial.println("Inicializando...");
  delay(1000);

  Serial.println("Chamando atencao do modulo com AT...");
  sendCommand("AT");
  readResponse(1000);

  Serial.println("Mudando o modo com CWMODE=2...");
  sendCommand("AT+CWMODE=2");
  readResponse(1000);

  Serial.println("Criando a rede com CWSAP...");
  String CWSAP = "AT+CWSAP=\"" + rede + "\",\"" + senha + "\",6,3";
  sendCommand(CWSAP);
  readResponse(5000);

  Serial.println("Testando a criacao da rede...");
  sendCommand("AT+CWSAP?");
  readResponse(1000);
}

void loop() {
  // por enquanto nao precisamos de nada aqui.
}

void sendCommand(String cmd) {
  ESP_Serial.println(cmd);
}

void readResponse(unsigned int timeout) {
  unsigned long timeIn = millis();
  resposta = "";
  while (timeIn + timeout > millis()) {
    if (ESP_Serial.available()) {
      char c = ESP_Serial.read();
      resposta += c;
    }
  }
  Serial.println(resposta);
}
