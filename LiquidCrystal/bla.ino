
// Inclui a biblioteca Wire que possui as funções da comunicação I2C:
#include <Wire.h>

const int endereco_MPU = 0x68; // endereço I2C do MPU-6050 (Padrão = 0x68)
// Variável para armazenar o valor de aceleração
float acel_y; // eixo Y

#include <LiquidCrystal.h> // inclui a biblioteca para uso do Display LCD
// inicializa um objeto nos pinos para acesso as funções do LCD
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

// Define os caraceteres especiais para formar um ponteiro no LCD
byte ponteiro_esquerda[8] = { // metade esquerda do ponteiro
  B00000,
  B00000,
  B00000,
  B11111,
  B01111,
  B00111,
  B00001
};

byte ponteiro_direita[8] = { // metade direita do ponteiro
  B00000,
  B00000,
  B00000,
  B11111,
  B11110,
  B11000,
  B10000
};

// variável que armazena o fator de calibração do MPU6050
// esta variável deve ser alterada caso o seu sensor esteja desregulado
// para isso é necessário usar as leituras "cruas" do sensor
const int fator_calibracao = 0;

// variável para armazenar os valores atual e anterior do ângulo
int angulo; 
int angulo_anterior = -100; // inicia com -100 para forçar atualização do LCD
int coluna; // variável para armazenar a posição da coluna do LCD

void setup(){
  // Configura o LCD com os número de colunas e linhas
  lcd.begin(16, 2); // 16 colunas e 2 linhas

  // Cria os caracteres especiais na memória do LCD
  lcd.createChar(0, ponteiro_esquerda); // Posição 0
  lcd.createChar(1, ponteiro_direita); // Posição 1

  // Inicia o barramento I2C:
  Wire.begin();

  Serial.begin(9600);
  
  // Inicializa o MPU-6050:
  Wire.beginTransmission(endereco_MPU); // T
  Wire.write(0x6B); //  registrador PWR_MGMT_1
  Wire.write(0); // seta para zero
  Wire.endTransmission(true); // encerra transmissão
  
  lcd.setCursor(2, 0); // posiciona o cursor do LCD (coluna, linha)
  lcd.print("Inclinometro"); // Imprime mensagem
  lcd.setCursor(5, 1); 
  lcd.print("Digital");
  
  delay(2000); // aguarda 2 segundo para iniciar
  
  // Limpa o LCD
  lcd.clear();
}
  
void loop(){
  // Inicia transmissão de dados para o MPU-6050
  Wire.beginTransmission(endereco_MPU);
  
  // Define o endereço inicial para leitura
  Wire.write(0x3D); // registrador ACCEL_YOUT_H
  Wire.endTransmission(false); // encerra transmissão

  // Solicita os 2 registradores correspondentes ao eixo y do acelerômetro
  Wire.requestFrom(endereco_MPU, 2, true);
  acel_y = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  Serial.print("Valor cru = ");
  Serial.print(acel_y);

  acel_y = acel_y - fator_calibracao; // subtrai a leitura do sensor pelo fator de calibração
  // Os valores do acelerômetro vão de -16384 até 16384
  acel_y = constrain(acel_y, -16384.0, 16384.0); // limitamos sua leitura entre estes valores
  
  angulo = asin(acel_y / 16384.0) * 180.0 / PI; // converte os valores de aceleração em ângulo
  
  Serial.print("\t|\t");
  Serial.print("Angulo calculado = ");
  Serial.println(angulo);

  int nivel = angulo / 12; // = 15 Níveis ( -7...0...+7)
  
  coluna = nivel + 7; // As colunas do LCD vão de 0 a 15.

  // Só atualiza o LCD se o ângulo mudar (evita que fique o LCD "piscando")
  if (angulo != angulo_anterior) {
    lcd.clear(); // Limpa o LCD
   
    // imprime o caracteres especiais da memória do LCD
    lcd.setCursor(coluna, 0);
    lcd.write(byte(0)); // metade esquerda do ponteiro
    lcd.write(byte(1)); // metade direita do ponteiro
    
    // Imprime a escala
    lcd.setCursor(0,1);
    lcd.print("90  45 00 45  90");
  }

  angulo_anterior = angulo; // atualiza o ângulo anterior
  
  delay(500); // aguarda 0.5 segundo para uma nova leitura
}
