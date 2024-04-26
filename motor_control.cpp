#include "motor_control.h"

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
  barrita = valor;

  // Convertir el valor del deslizador al rango -127 a 127
  int barrita_int = barrita.toInt() - 127;

  // Ajustar la dirección y la velocidad del motor según el valor del deslizador
  if (barrita_int > zona_central) {
    // A la derecha, girar en una dirección y aumentar la velocidad
    digitalWrite(pinIN1, HIGH);
    digitalWrite(pinIN2, LOW);
    // Ajustar la velocidad
    ledcWrite(canal_pwm, map(barrita_int, zona_central + 1, 127, velocidad_maxima, 0));
  } else if (barrita_int < -zona_central) {
    // A la izquierda, girar en la dirección opuesta y aumentar la velocidad
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, HIGH);
    // Ajustar la velocidad
    ledcWrite(canal_pwm, map(barrita_int, -127, -zona_central - 1, velocidad_maxima, 0));
  } else {
    // En el rango central, detener el motor
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, LOW);
    // Detener el motor
    ledcWrite(canal_pwm, 0);
  }
}
