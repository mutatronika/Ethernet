#include <SPI.h>
#include <Ethernet.h>

// ── Configuración de red ──────────────────────────────────
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer webServer(80);
EthernetClient conexion;

// ── Pines del LED RGB ─────────────────────────────────────
const int PIN_R = 11;
const int PIN_G = 9;
const int PIN_B = 10;

// ── Variables de estado ───────────────────────────────────
bool modoManual = false;   // false = arcoíris automático | true = control web
int  val        = 0;       // contador del efecto arcoíris
bool subiendo   = true;    // dirección del ciclo arcoíris

// ── Función: controlar LED (0-255) ────────────────────────
void setLed(int r, int g, int b) {
  analogWrite(PIN_R, r);
  analogWrite(PIN_G, g);
  analogWrite(PIN_B, b);
}

// ── Función: un paso del efecto arcoíris ──────────────────
void pasoArcoiris() {
  setLed(val, 255 - val, 128 - val);

  if (subiendo) {
    val++;
    if (val >= 255) subiendo = false;
  } else {
    val--;
    if (val <= 0) subiendo = true;
  }

  delay(10);
}

// ── Función: enviar página HTML ───────────────────────────
void enviarPagina() {
  conexion.println("HTTP/1.1 200 OK");
  conexion.println("Content-Type: text/html");
  conexion.println("Connection: close");
  conexion.println();
  conexion.println("<!DOCTYPE html>");
  conexion.println("<html lang='es'><head><title>Control LED</title></head>");
  conexion.println("<body style='background:#B5B5B5; text-align:center'>");
  conexion.println("<h2 style='font-family:Arial'>Control LED RGB</h2>");
  conexion.println("<form style='background:#828282; padding:20px; display:inline-block'>");

  // Botones de color
  conexion.println("<button type='submit' formaction='btRojo'     style='width:250px;height:80px;font-size:25px;color:red;margin:8px'><b>ROJO</b></button><br>");
  conexion.println("<button type='submit' formaction='btVerde'    style='width:250px;height:80px;font-size:25px;color:green;margin:8px'><b>VERDE</b></button><br>");
  conexion.println("<button type='submit' formaction='btAzul'     style='width:250px;height:80px;font-size:25px;color:blue;margin:8px'><b>AZUL</b></button><br>");
  conexion.println("<button type='submit' formaction='btAmarillo' style='width:250px;height:80px;font-size:25px;color:orange;margin:8px'><b>AMARILLO</b></button><br>");
  conexion.println("<button type='submit' formaction='btBlanco'   style='width:250px;height:80px;font-size:25px;margin:8px'><b>BLANCO</b></button><br>");

  // Separador
  conexion.println("<hr style='border-color:#555;margin:15px 0'>");

  // Botones especiales
  conexion.println("<button type='submit' formaction='btArcoiris' style='width:250px;height:60px;font-size:20px;margin:8px;background:linear-gradient(to right,red,orange,yellow,green,blue,violet)'><b>🌈 ARCOÍRIS</b></button><br>");
  conexion.println("<button type='submit' formaction='btApagar'   style='width:250px;height:60px;font-size:20px;margin:8px'><b>⬛ APAGAR</b></button><br>");
  conexion.println("<button type='submit' formaction='btSalir'    style='width:130px;height:40px;font-size:16px;margin:8px'><b>SALIR</b></button>");
  conexion.println("</form></body></html>");
}

// ── Setup ─────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);

  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  setLed(0, 0, 0);

  Ethernet.begin(mac, ip);
  webServer.begin();

  Serial.print("Servidor en: ");
  Serial.println(Ethernet.localIP());
}

// ── Loop ──────────────────────────────────────────────────
void loop() {

  // ── Verificar si hay conexión entrante ────────────────
  conexion = webServer.available();

  if (conexion) {
    Serial.println("Nueva conexión");
    String recibido  = "";
    bool   lineaBlanca = true;
    int    cod       = 0;

    while (conexion.connected()) {
      if (conexion.available()) {
        char c = conexion.read();
        recibido += c;

        // Detectar botón presionado
        if      (recibido.endsWith("/btRojo"))      cod = 1;
        else if (recibido.endsWith("/btVerde"))      cod = 2;
        else if (recibido.endsWith("/btAzul"))       cod = 3;
        else if (recibido.endsWith("/btAmarillo"))   cod = 4;
        else if (recibido.endsWith("/btBlanco"))     cod = 5;
        else if (recibido.endsWith("/btArcoiris"))   cod = 6;
        else if (recibido.endsWith("/btApagar"))     cod = 7;
        else if (recibido.endsWith("/btSalir"))      cod = 8;

        // Fin de cabecera HTTP
        if (c == '\n' && lineaBlanca) {

          // Aplicar acción
          switch (cod) {
            case 1: setLed(255,   0,   0); modoManual = true;  break; // Rojo
            case 2: setLed(  0, 255,   0); modoManual = true;  break; // Verde
            case 3: setLed(  0,   0, 255); modoManual = true;  break; // Azul
            case 4: setLed(255, 255,   0); modoManual = true;  break; // Amarillo
            case 5: setLed(255, 255, 255); modoManual = true;  break; // Blanco
            case 6: modoManual = false; val = 0; subiendo = true; break; // Arcoíris
            case 7: setLed(0, 0, 0);       modoManual = true;  break; // Apagar
            case 8:                                                     // Salir
              setLed(0, 0, 0);
              modoManual = false;
              conexion.stop();
              return;
          }

          enviarPagina();
          break;
        }

        lineaBlanca = (c == '\n') ? true : (c != '\r' ? false : lineaBlanca);
      }
    }

    delay(1);
    conexion.stop();
    Serial.println("Conexión cerrada");

  } else {
    // ── Sin conexión: modo arcoíris automático ──────────
    if (!modoManual) {
      pasoArcoiris();
    }
  }
}
