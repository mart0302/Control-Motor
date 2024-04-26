#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

extern int pinENA;
extern int pinIN1;
extern int pinIN2;
extern String barrita;

extern const int frecuencia;
extern const int canal_pwm;
extern const int resolucion;
extern const int velocidad_maxima;
extern const int zona_central;

void inicializarMotor();
void controlarMotor(int valor);

#endif
