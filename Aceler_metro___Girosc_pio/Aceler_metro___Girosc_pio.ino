
// Inclui a biblioteca Wire que possui as funções da comunicação I2C:
#include <Wire.h>

const int endereco_MPU = 0x68; // endereço I2C do MPU-6050 (Padrão = 0x68)
// Variáveis de 16bits para armazenar os valores de aceleração:
float acel_x; // aceleração linear eixo X
float acel_y; // aceleração linear eixo Y
float acel_z; // aceleração linear eixo Z
float giro_x; // aceleração angular eixo X
float giro_y; // aceleração angular eixo Y
float giro_z; // aceleração angular eixo Z
float temp; // temperatura

void setup(){
  // Inicia e configura a Serial:
  Serial.begin(9600); // 9600bps

  // Inicia o barramento I2C:
  Wire.begin();

  // Inicializa o MPU-6050:
  Wire.beginTransmission(endereco_MPU); // T
  Wire.write(0x6B); //  registrador PWR_MGMT_1
  Wire.write(0); // seta para zero
  Wire.endTransmission(true); // encerra transmissão

  // Valores de sensibilidade padrão
  // Acelerômetro: 2g
  // Giroscópio: 250 graus/segundo
}

void loop(){
  // Inicia transmissão de dados para o MPU-6050
  Wire.beginTransmission(endereco_MPU);

  // Define o endereço inicial para leitura
  Wire.write(0x3B); // registrador ACCEL_XOUT_H
  Wire.endTransmission(false); // encerra transmissão

  // Solicita 14 registradores de 8bits = 7 registradores de 16bits
  Wire.requestFrom(endereco_MPU, 14, true);

  // Lê e converte 14 registradores de 8bits em 7 registradores de 16bits
  acel_x = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  acel_y = Wire.read() << 8  | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  acel_z = Wire.read() << 8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  temp = Wire.read() << 8 | Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)

  giro_x = Wire.read() << 8 | Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  giro_y = Wire.read() << 8  | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  giro_z = Wire.read() << 8 | Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  // Converte os valores lidos em aceleração (g)
  acel_x = acel_x / 16384.0; // fator de conversão para escala de 2g
  acel_y = acel_y / 16384.0; // fator de conversão para escala de 2g
  acel_z = acel_z / 16384.0; // fator de conversão para escala de 2g

  // Converte valor de temperatura para graus Celsius
  temp = temp / 340.00 + 36.53; // equação do datasheet do sensor

  // Converte os valores do giroscópio lidos em velocidade (º/s)
  giro_x = giro_x / 131.0; // fator de conversão para escala de 250º/s
  giro_y = giro_y / 131.0; // fator de conversão para escala de 250º/s
  giro_z = giro_z / 131.0; // fator de conversão para escala de 250º/s

  // Imprime os valores de aceleração no Monitor Serial
  Serial.print("AcX: ");
  Serial.print(acel_x, 2);
  Serial.print(" | AcY: ");
  Serial.print(acel_y, 2);
  Serial.print(" | AcZ: ");
  Serial.print(acel_z, 2);
  Serial.print("");

  // Imprime o valor da temperatura no Monitor Serial
  Serial.print(" | Tmp: ");
  Serial.print(temp);

  // Imprime os valores de aceleração no Monitor Serial
  Serial.print(" | GyX: ");
  Serial.print(giro_x, 2);
  Serial.print(" | GyY: ");
  Serial.print(giro_y, 2);
  Serial.print(" | GyZ: ");
  Serial.print(giro_z, 2);

  Serial.println(); // nova linha

  delay(500); // aguarda 0.5 segundo para uma nova leitura
}
