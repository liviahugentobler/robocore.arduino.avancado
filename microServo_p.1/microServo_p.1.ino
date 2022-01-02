#include <Servo.h> // Inclui a biblioteca para servos motores.

Servo servo_motor;  // Cria um objeto para o controle do servo.

void setup() {
  servo_motor.attach(9);  // Informa qual pino será usado para o servo declarado anteriormente.
  servo_motor.write(10); //Informa ao servo que deverá se posicionar a 10 graus
}

void loop() {
}
