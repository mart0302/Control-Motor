#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

extern String barrita;
extern const char* parametro_entrada;
extern const char index_html[];

void inicializarMotor();
void controlarMotor(int valor);

#endif
