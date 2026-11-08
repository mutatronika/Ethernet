/*------------------------------------------------------------------------------ 
Program:      mqtt_node_arduino 

Description:  Basic MQTT node using Arduino Uno development board equipped with 
              an Ethernet shield (W500). The MQTT node actes acts both a MQTT
              publisher and subscriber - publish local sensor 
              readings (temperature and humidity) to MQTT topics and gets commands
              through a subscription to another MQTT topic.

Hardware:     Arduino Uno R3, Ethernet Shield (W5100), DHT11.
              Should work with other Arduinos 

Software:     Developed using Arduino 1.8.5 IDE

Libraries:    
              - Adafruit Unified Sensor Driver: 
                https://github.com/adafruit/Adafruit_Sensor
              - DHT-sensor-library: 
                https://github.com/adafruit/DHT-sensor-library
              - Arduino Client for MQTT:
                https://github.com/knolleary/pubsubclient
              
References: 
              - DHT11 datasheet: http://www.micropik.com/PDF/dht11.pdf
              - Arduino Client for MQTT:https://pubsubclient.knolleary.net/
              
Date:         Febryary 18, 2018

Author:       Gheorghe G., http://www.arduinolab.net
------------------------------------------------------------------------------*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <DHT.h>

#define ARDUINO_CLIENT_ID "arduino_1"                     // Client ID for Arduino pub/sub
#define PUB_TEMP "arduino_1/sensor/temperature_celsius"   // MTTQ topic for temperature [C]
#define PUB_HUMID "arduino_1/sensor/humidity"             // MTTQ topic for humidity
#define SUB_LED "arduino_1/led"                           // MTTQ topic for LED
#define PUBLISH_DELAY 3000                                // Publishing delay [ms]

// Hardware setup details
const int ledPin = 3;
const int sensorPin = 5;
const int sensorType = DHT11;

// Networking details
byte mac[]    = {  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };  // Ethernet shield (W5100) MAC address
IPAddress ip(192, 168, 2, 105);                           // Ethernet shield (W5100) IP address
IPAddress server(192, 168, 2, 114);                       // MTTQ server IP address

DHT dht(sensorPin, sensorType);
EthernetClient ethClient;
PubSubClient client(ethClient);

long previousMillis;

void setup()
{
  Serial.begin(9600);

  // LED off
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // MTTQ parameters
  client.setServer(server, 1883);
  client.setCallback(callback);

  // Ethernet shield configuration
  Ethernet.begin(mac, ip);

  delay(1500); // Allow hardware to stabilize

  previousMillis = millis();
}

void loop()
{
  if (!client.connected())
    reconnect();

  if (millis() - previousMillis > PUBLISH_DELAY)
  {
    previousMillis = millis();
    float humidity = dht.readHumidity(); // humidity
    float tempC = dht.readTemperature(); // temperature [C]
    char tmpBuffer[20];

    // check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(tempC))
    {
      Serial.println("error reading sensor data");
      return;
    }
    else
    {
      Serial.print("[sensor data] temperature[C]: ");
      Serial.print(tempC);
      Serial.print(", humidity: ");
      Serial.println(humidity);

      client.publish(PUB_TEMP, dtostrf(tempC, 6, 2, tmpBuffer));
      client.publish(PUB_HUMID, dtostrf(humidity, 6, 2, tmpBuffer));
    }
  }

  client.loop();
}

void reconnect()
{
  // Loop until reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection ... ");
    // Attempt to connect
    if (client.connect(ARDUINO_CLIENT_ID)) {
      Serial.println("connected");
      // (re)subscribe
      client.subscribe(SUB_LED);
    } else {
      Serial.print("Connection failed, state: ");
      Serial.print(client.state());
      Serial.println(", retrying in 5 seconds");
      delay(5000); // Wait 5 seconds before retrying
    }
  }
}

// sub callback function
void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("[sub: ");
  Serial.print(topic);
  Serial.print("] ");
  char message[length + 1] = "";
  for (int i = 0; i < length; i++)
    message[i] = (char)payload[i];
  message[length] = '\0';
  Serial.println(message);

  // SUB_LED topic section
  if (strcmp(topic, SUB_LED) == 0)
  {
    if (strcmp(message, "on") == 0)
      digitalWrite(ledPin, HIGH);
    if (strcmp(message, "off") == 0)
      digitalWrite(ledPin, LOW);
  }
}
