#include "motor_control.h"

int pinENA = 13;
int pinIN1 = 5;
int pinIN2 = 4;

const int frecuencia = 500;
const int canal_pwm = 0;
const int resolucion = 8;
const int velocidad_maxima = 255;
const int zona_central = 20;

void inicializarMotor() {
  pinMode(pinENA, OUTPUT);
  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);

  ledcSetup(canal_pwm, frecuencia, resolucion);
  ledcAttachPin(pinENA, canal_pwm);
  ledcWrite(canal_pwm, barrita.toInt());
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, LOW);
}

void controlarMotor(int valor) {
  barrita = String(valor);

  int barrita_int = valor - 127;

  if (barrita_int > zona_central) {
    digitalWrite(pinIN1, HIGH);
    digitalWrite(pinIN2, LOW);
    ledcWrite(canal_pwm, map(barrita_int, zona_central + 1, 127, velocidad_maxima, 0));
  } else if (barrita_int < -zona_central) {
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, HIGH);
    ledcWrite(canal_pwm, map(barrita_int, -127, -zona_central - 1, velocidad_maxima, 0));
  } else {
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, LOW);
    ledcWrite(canal_pwm, 0);
  }
}
