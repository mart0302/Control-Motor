#include <Arduino.h>
#include <WiFi.h>
#include "motor_control.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "ControlMotorAP";
const char* password = "password123";

AsyncWebServer servidor(80);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  inicializarMotor();

  WiFi.softAP(ssid, password);

  servidor.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  servidor.on("/deslizador", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String mensaje;
    if (request->hasParam(parametro_entrada)) {
      mensaje = request->getParam(parametro_entrada)->value();
      controlarMotor(mensaje.toInt());
    }
    request->send(200, "text/plain", "OK");
  });
  
  servidor.begin();
}

void loop() {

}
