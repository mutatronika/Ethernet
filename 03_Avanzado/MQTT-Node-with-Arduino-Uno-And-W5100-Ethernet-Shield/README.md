# MQTT-Node-with-Arduino-Uno-And-W5100-Ethernet-Shield

## 1. Description
This tutorial shows how to build a basic IoT device using an Arduino Uno (R3) board equipped with an Ethernet Shield (W5100). The device is configured both as MQTT publisher (for publishing local sensor data) as well as subscriber  (for getting remote commands) to topics located on a local area networked MQTT server.

Difficulty level: intermediate.

## 2. MQTT basics
According to MQTT 3.1.1 specification: “MQTT is a Client Server publish/subscribe messaging transport protocol. It is light weight, open, simple, and designed so as to be easy to implement. These characteristics make it ideal for use in many situations, including constrained environments such as for communication in Machine to Machine (M2M) and Internet of Things (IoT) contexts where a small code footprint is required and/or network bandwidth is at a premium.”

The MQTT (MQ Telemetry Transmission) protocol runs over TCP/IP (and/or other connection-oriented network protocols) in order to transfer messages between interconnected devices. A such message consists of a topic and a payload. A device (node) can publish messages and/or subscribe to messages to/from a MTTQ broker (server). The MTTQ broker receives published messages , keep track of the subscribers and send the proper message to the right device. The principle is depicted in the image below.

![MQTT Pub Sub!](/res/images/mqtt_pub_sub.jpg "mqtt_pub_sub")

Note. Any device could assume multiple roles (i.e. publisher and/or subscriber to multiple topics).

## 3. Mosquitto server
There are many MQTT brokers available, both cloud-based (i.e. on AWS, Azure, ThingSpeak, etc.) or self hosted ones. For the purpose of this tutorial we’ll use a self hosted open source solution: Mosquitto server. This fully compliant MQTT server is available for both Linux and Windows platforms and comes as well with command-line utilities for publishing and subscribing.

### 3.1. Running Mosquitto server
For the purpose of this demo we will use a Mosquitto server installed on a Windows 10 machine (binaries and installation procedures are available at https://mosquitto.org/).

By default, Mosquitto server runs on port 1883 without security – that is not an issue for the purpose of this demo.

Note. More details on securing the transmission channel are available on the Mosquitto server documentation web page. The limited resources (CPU and memory) available on the Arduino Uno board cannot successfully sustain such implementations.

On Windows, the server could be launched automatically (as a service) or manually, from the command prompt. A manual launch in verbose mode offers more insights on what’s happening on the server side:

    cd "\Program Files (x86)\mosquitto"
    mosquitto.exe -v

![Server launch!](/res/images/srv_launch.jpg "srv_launch")

### 3.2. Testing Mosquitto server
With the server running in verbose mode, open two more command prompt windows – one for the publisher, one for the subscriber.

In the subscriber window, launch a subscriber node to “test” topic:

    cd "\Program Files (x86)\mosquitto"
    mosquitto_sub.exe -h 127.0.0.1 -i cmd_sub -t test

    -h (host flag) indicates the server running Mosquitto (in this example the local loopback);
    -i (identity flag) identifies the client; optional, if missing Mosquitto will auto generate one
    -t (topic flag) indicates the topic name.

![Sub launch!](/res/images/sub_launch.jpg "sub_launch")

The server window shows that a client named “cmd_sub” is listening to the “test” topic.

In the publisher window, launch a publisher node to “test” topic and publish a test message (“Test MQTT message”) to “test” topic:

    cd "\Program Files (x86)\mosquitto"
    mosquitto_pub.exe -h 127.0.0.1 -i cmd_pub -t test -m "Test MQTT message"

    -h (host flag) indicates the server running Mosquitto (in this example the local loopback);
    -i (identity flag) identifies the client; optional, if missing Mosquitto will auto generate one;
    -t (topic flag) indicates the topic name;
    -m (message flag) indicates the message to be published.

![Pub launch!](/res/images/pub_launch_msg.jpg "pub_launch_msg")

The server window shows that a message has been received from the publisher  “cmd_pub” for the “test” topic; the published message is visible in the subscriber window.

This is the basics of MQTT communication using Mosquitto server. Other useful options are available online in the official documentation (https://mosquitto.org/).

## 4. Setup details
The setup consists of the following components:

- a Mosquitto server acting as MQTT broker on the local area network;
- an Arduino Uno board equipped with an Ethernet shield (W5100) acting both as publisher and subscriber. As a publisher, it will send sensor readings (temperature and humidity) to two different topics (arduino_1/sensor/temperature_celsius and arduino_1/sensor/humidity); as a subscriber, it will receive commands to power on/off a LED via a separate topic (arduino_1/led);
- several command prompt Mosquitto clients acting as publishers/subscribers to MQTT topics on the Mosquitto browser.

The logical diagram of the demo setup is depicted in the image below.

![MQTT Pub Sub Arduino!](/res/images/mqtt_pub_sub_ard.jpg "mqtt_pub_sub_ard")

## 5. Parts – Arduino MQTT node
* 1 x Arduino Uno R3;
* 1 x Arduino Ethernet shield (W5100);
* 1 x DHT11;
* 1 x LED;
* 1 x 220 ohm resistor;
* Breadboard + wires.

## 6. Schematics – Arduino MQTT node

![MQTT Node Arduino Schematics!](/res/diagrams/mqtt_node_arduino_schem.jpg "mmqtt_node_arduino_schem")

## 7. Assembly (breadboard) – Arduino MQTT node

![MQTT Node Arduino Breadboard!](/res/diagrams/mqtt_node_arduino_bboard.jpg "mmqtt_node_arduino_bboard")

 ## 8. Code – Arduino MQTT node

 /mqtt_node_arduino/mqtt_node_arduino.ino

## 9. Demo

Youtube link:  [MQTT-Node-with-Arduino-Uno-And-W5100-Ethernet-Shield](https://www.youtube.com/watch?v=MQVbPG708Ik&lc=Ugzo6zfxNHqnAkhTXU54AaABAg.A2hoFgFLh2jA3QeNldDyGf)

## 10. Additional ressources

* MQTT:
    * http://www.steves-internet-guide.com/mqtt/
    * https://www.hivemq.com/mqtt-essentials/

* Mosquitto:
    * https://mosquitto.org

* Libraries:
    * Adafruit Unified Sensor Driver:
    https://github.com/adafruit/Adafruit_Sensor
    * DHT-sensor-library:
    https://github.com/adafruit/DHT-sensor-library
    * Arduino Client for MQTT:
    https://github.com/knolleary/pubsubclient