#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// CAMBIO: Pon aquí la IP de tu servidor (donde tengas tu PHP, Python o Node.js)
IPAddress server(192, 168, 1, 50); 

EthernetClient client;

void setup() {
  Serial.begin(9600);
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Falla DHCP");
    for(;;);
  }
  delay(1000);
}

void loop() {
  // Leemos un sensor (por ejemplo, un potenciómetro en A0)
  int valorSensor = analogRead(A0);
  
  Serial.println("Conectando para enviar datos...");

  if (client.connect(server, 80)) {
    Serial.println("Conectado al servidor");

    // CAMBIO: Enviamos los datos mediante la URL
    // Imagina que tu servidor tiene un archivo "recibir.php"
    client.print("GET /recibir.php?sensor1=");
    client.print(valorSensor);
    client.println(" HTTP/1.1");
    
    // CAMBIO: Host es obligatorio en HTTP/1.1
    client.println("Host: 192.168.1.50"); 
    client.println("Connection: close");
    client.println(); // Línea en blanco final obligatoria

    Serial.println("Datos enviados correctamente");
  } else {
    Serial.println("Fallo al conectar al servidor");
  }

  // Esperamos 10 segundos antes de enviar el siguiente dato
  // Para no saturar el servidor
  delay(10000); 

  // Leemos la respuesta del servidor (opcional)
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  if (!client.connected()) {
    client.stop();
  }
}
