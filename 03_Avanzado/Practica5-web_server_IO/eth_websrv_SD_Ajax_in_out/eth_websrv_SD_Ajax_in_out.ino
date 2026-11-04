/*--------------------------------------------------------------
  Program:      eth_websrv_SD_Ajax_in_out

  Description:  Arduino web server that displays 4 analog inputs,
                the state of 3 switches and controls 4 outputs,
                2 using checkboxes and 2 using buttons.
                The web page is stored on the micro SD card.
  
  Hardware:     Arduino Uno and official Arduino Ethernet
                shield. Should work with other Arduinos and
                compatible Ethernet shields.
                2Gb micro SD card formatted FAT16.
                A2 to A4 analog inputs, pins 2, 3 and 5 for
                the switches, pins 6 to 9 as outputs (LEDs).
                
  Software:     Developed using Arduino 1.0.5 software
                Should be compatible with Arduino 1.0 +
                SD card contains web page called index.htm
  
  References:   - WebServer example by David A. Mellis and 
                  modified by Tom Igoe
                - SD card examples by David A. Mellis and
                  Tom Igoe
                - Ethernet library documentation:
                  http://arduino.cc/en/Reference/Ethernet
                - SD Card library documentation:
                  http://arduino.cc/en/Reference/SD

  Date:         4 April 2013
  Modified:     19 June 2013
                - removed use of the String class
 
  Author:       W.A. Smith, http://startingelectronics.com
--------------------------------------------------------------*/

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
// tamaño del buffer usado para capturar solicitudes HTTP
#define REQ_BUF_SZ   60

// Dirección MAC de la etiqueta del Ethernet shield bajo la placa
byte mac[] = { 0xD3, 0xAD, 0xB5, 0xAF, 0x05, 0xED };
IPAddress ip(192, 168, 1, 177); // Dirección IP, puede necesitar cambiar según la red
EthernetServer server(80);  // crear un servidor en el puerto 80
File webFile;               // el archivo de página web en la tarjeta SD
char HTTP_req[REQ_BUF_SZ] = {0}; // solicitud HTTP almacenada como cadena terminada en nulo
char req_index = 0;              // índice into HTTP_req buffer
boolean LED_state[4] = {0}; // almacena los estados de los LEDs

void setup()
{
    // deshabilitar chip Ethernet
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    
    Serial.begin(9600);       // para depuración
    
    // inicializar tarjeta SD
    Serial.println("Inicializando tarjeta SD...");
    if (!SD.begin(4)) {
        Serial.println("ERROR - ¡Falló la inicialización de la tarjeta SD!");
        return;    // inicialización fallida
    }
    Serial.println("ÉXITO - Tarjeta SD inicializada.");
    // verificar archivo index.htm
    if (!SD.exists("index.htm")) {
        Serial.println("ERROR - ¡No se puede encontrar el archivo index.htm!");
        return;  // no se encuentra archivo índice
    }
    Serial.println("ÉXITO - Archivo index.htm encontrado.");
    // interruptores en pines 2, 3 y 5
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(5, INPUT);
    // LEDs
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    
    Ethernet.begin(mac, ip);  // inicializar dispositivo Ethernet
    server.begin();           // empezar a escuchar clientes
}

void loop()
{
    EthernetClient client = server.available();  // try to get client

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                // limit the size of the stored received HTTP request
                // buffer first part of HTTP request in HTTP_req array (string)
                // leave last element in array as 0 to null terminate string (REQ_BUF_SZ - 1)
                if (req_index < (REQ_BUF_SZ - 1)) {
                    HTTP_req[req_index] = c;          // save HTTP request character
                    req_index++;
                }
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    // remainder of header follows below, depending on if
                    // web page or XML page is requested
                    // Ajax request - send XML file
                    if (StrContains(HTTP_req, "ajax_inputs")) {
                        // send rest of HTTP header
                        client.println("Content-Type: text/xml");
                        client.println("Connection: keep-alive");
                        client.println();
                        SetLEDs();
                        // send XML file containing input states
                        XML_response(client);
                    }
                    else {  // web page request
                        // send rest of HTTP header
                        client.println("Content-Type: text/html");
                        client.println("Connection: keep-alive");
                        client.println();
                        // send web page
                        webFile = SD.open("index.htm");        // open web page file
                        if (webFile) {
                            while(webFile.available()) {
                                client.write(webFile.read()); // send web page to client
                            }
                            webFile.close();
                        }
                    }
                    // display received HTTP request on serial port
                    Serial.print(HTTP_req);
                    // reset buffer index and all buffer elements to 0
                    req_index = 0;
                    StrClear(HTTP_req, REQ_BUF_SZ);
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
}

// verifica si la solicitud HTTP recibida está encendiendo/apagando LEDs
// también guarda el estado de los LEDs
void SetLEDs(void)
{
    // LED 1 (pin 6)
    if (StrContains(HTTP_req, "LED1=1")) {
        LED_state[0] = 1;  // guardar estado LED
        digitalWrite(6, HIGH);
    }
    else if (StrContains(HTTP_req, "LED1=0")) {
        LED_state[0] = 0;  // guardar estado LED
        digitalWrite(6, LOW);
    }
    // LED 2 (pin 7)
    if (StrContains(HTTP_req, "LED2=1")) {
        LED_state[1] = 1;  // guardar estado LED
        digitalWrite(7, HIGH);
    }
    else if (StrContains(HTTP_req, "LED2=0")) {
        LED_state[1] = 0;  // guardar estado LED
        digitalWrite(7, LOW);
    }
    // LED 3 (pin 8)
    if (StrContains(HTTP_req, "LED3=1")) {
        LED_state[2] = 1;  // guardar estado LED
        digitalWrite(8, HIGH);
    }
    else if (StrContains(HTTP_req, "LED3=0")) {
        LED_state[2] = 0;  // guardar estado LED
        digitalWrite(8, LOW);
    }
    // LED 4 (pin 9)
    if (StrContains(HTTP_req, "LED4=1")) {
        LED_state[3] = 1;  // guardar estado LED
        digitalWrite(9, HIGH);
    }
    else if (StrContains(HTTP_req, "LED4=0")) {
        LED_state[3] = 0;  // guardar estado LED
        digitalWrite(9, LOW);
    }
}

// enviar el archivo XML con valores analógicos, estado de interruptores
// y estado de LEDs
void XML_response(EthernetClient cl)
{
    int analog_val;            // almacena valor leído de entradas analógicas
    int count;                 // usado por ciclos 'for'
    int sw_arr[] = {2, 3, 5};  // pines conectados a interruptores
    
    cl.print("<?xml version = \"1.0\" ?>");
    cl.print("<inputs>");
    // read analog inputs
    for (count = 2; count <= 5; count++) { // A2 to A5
        analog_val = analogRead(count);
        cl.print("<analog>");
        cl.print(analog_val);
        cl.println("</analog>");
    }
    // read switches
    for (count = 0; count < 3; count++) {
        cl.print("<switch>");
        if (digitalRead(sw_arr[count])) {
            cl.print("ON");
        }
        else {
            cl.print("OFF");
        }
        cl.println("</switch>");
    }
    // checkbox LED states
    // LED1
    cl.print("<LED>");
    if (LED_state[0]) {
        cl.print("checked");
    }
    else {
        cl.print("unchecked");
    }
    cl.println("</LED>");
    // LED2
    cl.print("<LED>");
    if (LED_state[1]) {
        cl.print("checked");
    }
    else {
        cl.print("unchecked");
    }
     cl.println("</LED>");
    // button LED states
    // LED3
    cl.print("<LED>");
    if (LED_state[2]) {
        cl.print("on");
    }
    else {
        cl.print("off");
    }
    cl.println("</LED>");
    // LED4
    cl.print("<LED>");
    if (LED_state[3]) {
        cl.print("on");
    }
    else {
        cl.print("off");
    }
    cl.println("</LED>");
    
    cl.print("</inputs>");
}

// establece cada elemento de str a 0 (limpia el arreglo)
void StrClear(char *str, char length)
{
    for (int i = 0; i < length; i++) {
        str[i] = 0;
    }
}

// busca la cadena sfind en la cadena str
// devuelve 1 si se encuentra la cadena
// devuelve 0 si no se encuentra la cadena
char StrContains(char *str, char *sfind)
{
    char found = 0;
    char index = 0;
    char len;

    len = strlen(str);
    
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }

    return 0;
}
