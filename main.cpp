#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Mega_2.4G_062F";
const char* password = "qSJ6rfnX";

int pinENA = 13;
int pinIN1 = 5;
int pinIN2 = 4;
String barrita = "127";

const int frecuencia = 500;
const int canal_pwm = 0;
const int resolucion = 8;
const int velocidad_maxima = 255; // Velocidad máxima del motor
const int zona_central = 20; // Rango alrededor de la mitad donde el motor se detiene

const char* parametro_entrada = "valor";

AsyncWebServer servidor(80);

const char index_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>PAGINA PARA MOTOR :)</title>
  <style>
    html {font-family: Times New Roman; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 2.0rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #4000ff;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background:#01070a; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #01070a; cursor: pointer; }
  </style>
</head>
<body>
  <h2>PAGINA PARA MOTOR :)</h2>
  <p></p>
  <p><input type="range" onchange="actualizarPWM(this)" id="pwmSlider" min="0" max="255" value="127" step="1" class="slider"></p>
<script>
function actualizarPWM(elemento) {
  var valor_deslizador = document.getElementById("pwmSlider").value;
  console.log(valor_deslizador);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/deslizador?valor="+valor_deslizador, true);
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

String procesador(const String& var){
  if (var == "VALORBARRITA"){
    return barrita;
  }
  return String(); 
}

void setup(){
  Serial.begin(115200);
  delay(1000);
  pinMode(pinENA, OUTPUT);
  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);

  ledcSetup(canal_pwm, frecuencia, resolucion);
  ledcAttachPin(pinENA, canal_pwm);
  ledcWrite(canal_pwm, barrita.toInt());
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando...");
  }

  Serial.println(WiFi.localIP());

  servidor.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, procesador);
  });

  servidor.on("/deslizador", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String mensaje;
    if (request->hasParam(parametro_entrada)) {
      mensaje = request->getParam(parametro_entrada)->value();
      barrita = mensaje;
      ledcWrite(canal_pwm, barrita.toInt());
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
    request->send(200, "text/plain", "OK");
  });
  
  servidor.begin();
}
  
void loop() {
  
}
