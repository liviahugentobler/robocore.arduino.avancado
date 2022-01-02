const int pir_signal_pin = 9;
const int relay = 10;
const int LED = 13;
const int buzzer = 12;
const int button = 11;

boolean pir_status, ledState;
int button_status, flag = 0;
unsigned long previousMillis_liga = 0, previousMillis_desliga = 0;
const long interval = 100, alarme_liga = 500, alarme_desliga = 1000;

void setup() {
  pinMode(pir_signal_pin, INPUT);
  pinMode(button, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(LED, HIGH);
}

void loop() {

  button_status = digitalRead(button);
  if (button_status == HIGH) {
    while (button_status == HIGH) {
      button_status = digitalRead(button);
    }
    digitalWrite(relay, LOW);
    digitalWrite(LED, LOW);
    noTone(buzzer);
    flag++;
    if (flag == 0) {
      digitalWrite(LED, HIGH);
    }
    else {
      delay(5000);
      while (flag > 0) {
        pir_status = digitalRead(pir_signal_pin);
        if (pir_status == LOW) {
          unsigned long present = millis();
          if (present - previousMillis_liga >= interval) {
            previousMillis_liga = present;
            if (ledState == LOW) {
              ledState = HIGH;
            }
            else {
              ledState = LOW;
            }
            digitalWrite(LED, ledState);
          }
          digitalWrite(relay, LOW);
        }
        else {
          digitalWrite(relay, HIGH);
          digitalWrite(LED, LOW);
          while (flag > 0) {
            unsigned long present = millis();
            if (present -  previousMillis_liga >= alarme_liga) {
              previousMillis_liga = present;
              tone(buzzer, 2000);
            }
            if (present - previousMillis_desliga >= alarme_desliga) {
              previousMillis_desliga = present;
              noTone(buzzer);
            }
            button_status = digitalRead(button);
            if (button_status == HIGH) {
              flag = -1;
            }
          }
        }
      }
    }
  }
}
