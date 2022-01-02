#include "SoftwareSerial.h"

SoftwareSerial ESP_Serial(10, 11); // RX, TX

String rede = "WiFi-ESP8266";
String senha = "0123456789";
String resposta = "";

const int rele = 2;
int estadoRele = 0;
String botao = "";

void setup() {
  pinMode(rele, OUTPUT);

  Serial.begin(9600);
  ESP_Serial.begin(9600);

  Serial.println("Inicializando...");
  delay(1000);

  Serial.println("Chamando atencao do modulo com AT...");
  sendCommand("AT");
  readResponse(1000);

  if (resposta.indexOf("OK") == -1) { //procura na resposta se houve OK
    Serial.println("Atencao: Nao foi possivel se comunicar com o ESP8266.");
    Serial.println("Verifique se o módulo está alimentado e conectado corretamente à placa.");
  } else {
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

    Serial.println("Verificando o endereço de IP do módulo para posterior conexao...");
    sendCommand("AT+CIFSR");
    readResponse(1000);

    Serial.println("Configurando para multiplas conexoes...");
    sendCommand("AT+CIPMUX=1");
    readResponse(1000);

    Serial.println("Ligando o servidor...");
    sendCommand("AT+CIPSERVER=1,80");
    readResponse(1000);

    Serial.println("Pronto! Se conecte a rede do ESP8266 e acesse o IP.");
  }
}

void loop() {
  if (ESP_Serial.available()) {
    //.find le os dados vindos da serial ate o alvo, neste caso +IPD,
    if (ESP_Serial.find("+IPD,")) {

      delay(500);

      char id = ESP_Serial.peek();//pega ID da conexao

      //verifica se cliente esta requisitando a pagina /go:
      if (ESP_Serial.find("/g")) {
        if (estadoRele == 0) {
          digitalWrite(rele, HIGH);
          estadoRele = 1;
        } else {
          digitalWrite(rele, LOW);
          estadoRele = 0;
        }
      }

      if (estadoRele == 0) {
        botao = "L";
      } else {
        botao = "D";
      }

      //nossa pagina web em HTML
      String webpage = String("HTTP/1.1 200 OK\r\n") +
                       "Content-Type: text/html\r\n" +
                       "Connection: close\r\n" +
                       "\r\n" +
                       "<a href=\"g\">" +
                       botao +
                       "</a>";



      String cipSend = "AT+CIPSEND=";
      cipSend += (int(id) - 48);
      cipSend += ",";
      cipSend += webpage.length();
      Serial.println(webpage.length());
      sendCommand(cipSend);
      readResponse(750);

      sendCommand(webpage);
      readResponse(1000);

      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += (int(id) - 48);
      sendCommand(closeCommand);
      readResponse(750);

    }
  }
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
  //Serial.println(resposta);
}
