#include <SPI.h>
#include <Ethernet.h>
#include <dht.h>
#include <stdlib.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "mysite.com";
// Establecer la dirección IP estática a usar si el DHCP falla en asignar
IPAddress ip(192,168,1,11);
EthernetClient client;
dht DHT;
#define DHT11_PIN 2

void setup() {
  Serial.begin(9600);
}

void loop()
{

  // iniciar la conexión Ethernet:
  if (Ethernet.begin(mac) == 0) {
    // intentar configurar usando dirección IP en lugar de DHCP:
    Ethernet.begin(mac, ip);
  }
  // dar un segundo al Ethernet shield para inicializarse:
  delay(1000);
  
  if (client.connect(server, 80)) {
    
    DHT.read11(DHT11_PIN);
    char buffr[20];
    
    String PostData="macaddr=testaddr";
    PostData=PostData+"&temperature=";
    PostData=PostData+dtostrf(DHT.temperature,5,1,buffr);
    PostData=PostData+"&humidity=";
    PostData=PostData+dtostrf(DHT.humidity,5,1,buffr);

    client.println("POST /arduino/index.php HTTP/1.1");
    client.println("Host:  mysite.com");
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(PostData.length());
    client.println();
    client.println(PostData);
    
    // esperar
    Serial.println("Esperando");
    

  } 
  else {
    Serial.println("Cliente no conectado - no se pudieron enviar datos.");
  }
  
  // si hay bytes entrantes disponibles 
  // del servidor, leerlos e imprimirlos:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  delay(5000);
  client.flush();
  client.stop();
  delay(1800000); // 30 min = 1800000 ms
    
  // si el servidor se ha desconectado, detener el cliente:
  //if (!client.connected()) {
  //  client.stop();
  //  // do nothing forevermore:
  //  while(true);
  //}
}
