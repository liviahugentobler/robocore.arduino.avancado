
#include "IRremote.h"

const int RECV_PIN = 11;
const int pino=2;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  pinMode(pino,OUTPUT);
  irrecv.enableIRIn(); //começa a receber
}

void loop() {
  if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);
  
  if (results.value == 0x20DF8877){
      Serial.println("liga"); 
      digitalWrite(pino, HIGH);
    }
  if (results.value == 0xFF18E7){
      Serial.println("desliga"); 
      digitalWrite(pino, LOW);
    }
   
    irrecv.resume(); // Recebe o próximo valor
  }
}
