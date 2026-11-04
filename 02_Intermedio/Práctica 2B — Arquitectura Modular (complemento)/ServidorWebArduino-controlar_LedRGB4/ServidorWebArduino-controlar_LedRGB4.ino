#include <SPI.h>
#include <Ethernet.h>

// ── Red ───────────────────────────────────────────────────
byte mac[] = { 0xDE, 0xAD, 0xAE, 0xEF, 0xF0, 0xED };
IPAddress ip(192, 168, 1, 177);  // <-- Mismo rango que el documento
EthernetServer webServer(80);

// ── Pines LED RGB ─────────────────────────────────────────
const int PIN_R = 3;
const int PIN_G = 5;
const int PIN_B = 8;

// ── Estado ────────────────────────────────────────────────
String    HTTP_req  = "";
bool      modoManual = false;
int       val        = 0;
bool      subiendo   = true;

// ── LED ───────────────────────────────────────────────────
void setLed(int r, int g, int b) {
  analogWrite(PIN_R, r);
  analogWrite(PIN_G, g);
  analogWrite(PIN_B, b);
}

void pasoArcoiris() {
  setLed(val, 255 - val, abs(128 - val));
  subiendo ? val++ : val--;
  if (val >= 255) subiendo = false;
  if (val <= 0)   subiendo = true;
  delay(10);
}

// ── HTML ──────────────────────────────────────────────────
void enviarPagina(EthernetClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: keep-alive"); // igual que el documento
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html><head><title>Control LED RGB</title></head>");
  client.println("<body style='background:#B5B5B5;text-align:center'>");
  client.println("<h2>Control LED RGB</h2>");
  client.println("<form style='background:#828282;padding:20px;display:inline-block'>");
  client.println("<button type='submit' formaction='btRojo'     style='width:250px;height:80px;font-size:25px;color:red;margin:8px'><b>ROJO</b></button><br>");
  client.println("<button type='submit' formaction='btVerde'    style='width:250px;height:80px;font-size:25px;color:green;margin:8px'><b>VERDE</b></button><br>");
  client.println("<button type='submit' formaction='btAzul'     style='width:250px;height:80px;font-size:25px;color:blue;margin:8px'><b>AZUL</b></button><br>");
  client.println("<button type='submit' formaction='btAmarillo' style='width:250px;height:80px;font-size:25px;color:orange;margin:8px'><b>AMARILLO</b></button><br>");
  client.println("<button type='submit' formaction='btBlanco'   style='width:250px;height:80px;font-size:25px;margin:8px'><b>BLANCO</b></button><br>");
  client.println("<button type='submit' formaction='btCian'     style='width:250px;height:80px;font-size:25px;color:#00FFFF;margin:8px'><b>CIAN</b></button><br>");
  client.println("<button type='submit' formaction='btMagenta'  style='width:250px;height:80px;font-size:25px;color:#FF00FF;margin:8px'><b>MAGENTA</b></button><br>");
  client.println("<button type='submit' formaction='btNaranja'  style='width:250px;height:80px;font-size:25px;color:#FF5500;margin:8px'><b>NARANJA</b></button><br>");
  client.println("<button type='submit' formaction='btMorado'   style='width:250px;height:80px;font-size:25px;color:#8000FF;margin:8px'><b>MORADO</b></button><br>");
  client.println("<button type='submit' formaction='btRosa'     style='width:250px;height:80px;font-size:25px;color:#FF1493;margin:8px'><b>ROSA</b></button><br>");
  client.println("<button type='submit' formaction='btTurquesa' style='width:250px;height:80px;font-size:25px;color:#40E0D0;margin:8px'><b>TURQUESA</b></button><br>");
  client.println("<button type='submit' formaction='btLima'     style='width:250px;height:80px;font-size:25px;color:#32CD32;margin:8px'><b>LIMA</b></button><br>");
  client.println("<hr style='border-color:#555;margin:15px 0'>");
  client.println("<button type='submit' formaction='btArcoiris' style='width:250px;height:60px;font-size:20px;margin:8px'><b>ARCOIRIS</b></button><br>");
  client.println("<button type='submit' formaction='btApagar'   style='width:250px;height:60px;font-size:20px;margin:8px'><b>APAGAR</b></button><br>");
  client.println("<button type='submit' formaction='btSalir'    style='width:130px;height:40px;font-size:16px;margin:8px'><b>SALIR</b></button>");
  client.println("</form></body></html>");
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

   delay(2000); // Esperar que el shield arranque completamente

  delay(2000); // Esperar que el shield arranque

  Serial.println("=== INICIANDO ===");

  // Intentar con DHCP primero
  Serial.println("Intentando DHCP...");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP falló, usando IP estática...");
    Ethernet.begin(mac, ip);
  } else {
    Serial.println("DHCP exitoso");
  }

  delay(1000); // Esperar estabilización

  // Verificar hardware
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("ERROR: Shield no encontrado. Deteniendo.");
    while (true) { delay(1); }
  }

  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("ADVERTENCIA: Cable desconectado");
  } else {
    Serial.println("Cable: OK");
  }

  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());
  Serial.print("Gateway: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("Máscara: ");
  Serial.println(Ethernet.subnetMask());

  webServer.begin();
  Serial.println("Servidor iniciado");
  Serial.println("=================");
  Serial.print("Servidor en: ");
  Serial.println(Ethernet.localIP());
}

// ── Loop ──────────────────────────────────────────────────
void loop() {
  EthernetClient client = webServer.available(); // igual que el documento

  if (client) {
    boolean currentLineIsBlank = true; // mismo nombre que el documento
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        HTTP_req += c;

        if (c == '\n' && currentLineIsBlank) {
          // ── Detectar botón usando indexOf() como el documento ──
          if      (HTTP_req.indexOf("btRojo")     > -1) { setLed(255,   0,   0); modoManual = true; }
          else if (HTTP_req.indexOf("btVerde")    > -1) { setLed(  0, 255,   0); modoManual = true; }
          else if (HTTP_req.indexOf("btAzul")     > -1) { setLed(  0,   0, 255); modoManual = true; }
          else if (HTTP_req.indexOf("btAmarillo") > -1) { setLed(255, 255,   0); modoManual = true; }
          else if (HTTP_req.indexOf("btBlanco")   > -1) { setLed(255, 255, 255); modoManual = true; }
          else if (HTTP_req.indexOf("btCian")     > -1) { setLed(  0, 255, 255); modoManual = true; }
          else if (HTTP_req.indexOf("btMagenta")  > -1) { setLed(255,   0, 255); modoManual = true; }
          else if (HTTP_req.indexOf("btNaranja")  > -1) { setLed(255,  80,   0); modoManual = true; }
          else if (HTTP_req.indexOf("btMorado")   > -1) { setLed(128,   0, 255); modoManual = true; }
          else if (HTTP_req.indexOf("btRosa")     > -1) { setLed(255,  20, 147); modoManual = true; }
          else if (HTTP_req.indexOf("btTurquesa") > -1) { setLed( 64, 224, 208); modoManual = true; }
          else if (HTTP_req.indexOf("btLima")     > -1) { setLed( 50, 205,  50); modoManual = true; }
          else if (HTTP_req.indexOf("btArcoiris") > -1) { modoManual = false; val = 0; subiendo = true; }
          else if (HTTP_req.indexOf("btApagar")   > -1) { setLed(  0,   0,   0); modoManual = true; }
          else if (HTTP_req.indexOf("btSalir")    > -1) { setLed(  0,   0,   0); modoManual = false; }

          enviarPagina(client);

          Serial.print(HTTP_req); // diagnóstico igual que el documento
          HTTP_req = "";          // vaciar igual que el documento
          break;
        }

        // ✅ Lógica correcta copiada del documento
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
  }

  // Arcoíris automático si no hay conexión
  if (!modoManual) {
    pasoArcoiris();
  }
}
