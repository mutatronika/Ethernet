# Plataforma de Desarrollo y Prácticas: Arduino Ethernet (W5100 / W5500)

Repositorio estructurado para el aprendizaje, desarrollo e integración de proyectos de redes y sistemas embebidos utilizando microcontroladores Arduino (Uno, Mega 2560, Nano) y módulos Ethernet basados en los controladores Wiznet W5100 y W5500, con soporte para almacenamiento en tarjetas MicroSD.

---

## Índice

1. [Consideraciones de Hardware y Pines de Conexión](#consideraciones-de-hardware-y-pines-de-conexión)
2. [Estrategia de Memoria y Buenas Prácticas](#estrategia-de-memoria-y-buenas-prácticas)
3. [Roadmap de Aprendizaje](#roadmap-de-aprendizaje)
4. [Nivel 1: Fundamentos y Servidores Web Básicos (01_Basico)](#nivel-1-fundamentos-y-servidores-web-básicos-01_basico)
5. [Nivel 2: Interactividad, Almacenamiento SD y Sensores (02_Intermedio)](#nivel-2-interactividad-almacenamiento-sd-y-sensores-02_intermedio)
6. [Nivel 3: Sistemas Avanzados, Control Multicanal e IoT (03_Avanzado)](#nivel-3-sistemas-avanzados-control-multicanal-e-iot-03_avanzado)
7. [Módulo Base de Sensorización (Sensorica)](#módulo-base-de-sensorización-sensorica)
8. [Esquemas y Recursos de Documentación](#esquemas-y-recursos-de-documentación)
9. [Guía de Configuración Inicial y Diagnóstico](#guía-de-configuración-inicial-y-diagnóstico)

---

## Consideraciones de Hardware y Pines de Conexión

El Ethernet Shield estándar integra un controlador Ethernet Wiznet W5100 y una ranura para tarjeta MicroSD. Ambos dispositivos se comunican con el microcontrolador mediante el bus SPI.

### Distribución de Pines del Bus SPI

| Función | Arduino Uno / Nano | Arduino Mega 2560 | Observaciones |
|---|---|---|---|
| SPI MOSI | Pin 11 | Pin 51 | Línea de datos maestro a esclavo |
| SPI MISO | Pin 12 | Pin 50 | Línea de datos esclavo a maestro |
| SPI SCK | Pin 13 | Pin 52 | Reloj del bus SPI |
| Ethernet CS (SS) | Pin 10 | Pin 10 | Selección del chip Ethernet (activo en bajo) |
| MicroSD CS (SS) | Pin 4 | Pin 4 | Selección de la tarjeta MicroSD (activo en bajo) |
| SPI Hardware SS | Pin 10 | Pin 53 | Debe configurarse como salida para habilitar SPI |

### Convivencia entre Ethernet y MicroSD

Ambos dispositivos comparten las líneas MOSI, MISO y SCK. Para evitar contención en el bus:
- Al inicializar el hardware en `setup()`, desactive el chip secundario antes de inicializar el principal.
- Desactivación de Ethernet: poner Pin 10 en `HIGH`.
- Desactivación de MicroSD: poner Pin 4 en `HIGH`.
- Nunca use los pines 4 y 10 para entradas o salidas de propósito general en sus circuitos externos.

---

## Estrategia de Memoria y Buenas Prácticas

En microcontroladores con recursos limitados como el ATmega328P (2 KB de SRAM):

1. **Evitar la clase `String`**: La concatenación dinámica fragmenta el heap y produce reinicios esporádicos o fallos de conexión. Se deben emplear buffers fijos de caracteres (`char[]`), punteros y funciones estándar de C (`strcmp`, `strstr`, `snprintf`).
2. **Uso de la macro `F()`**: Las cadenas literales en llamadas `client.print()` deben almacenarse en memoria de programa Flash: `client.print(F("HTTP/1.1 200 OK\r\n"));`.
3. **Formato de la tarjeta MicroSD**:
   - Sistema de archivos: FAT16 o FAT32 (recomendado FAT16 para tarjetas de hasta 2 GB).
   - Nombres de archivos bajo la convención 8.3 (máximo 8 caracteres de nombre y 3 de extensión, sin caracteres especiales ni espacios).
4. **Dimensionamiento de buffers HTTP**: Limitar el buffer de lectura de la petición entrante al tamaño mínimo necesario para extraer la ruta o parámetros (generalmente entre 20 y 64 bytes).

---

## Roadmap de Aprendizaje

El repositorio se divide en tres niveles formativos principales más un módulo base y recursos de referencia. La progresión sigue una curva técnica ascendente:

| Fase | Directorio | Alcance Principal | Rango de Dificultad |
|---|---|---|---|
| Nivel 1 | `01_Basico/` | Clientes HTTP salientes, servidores locales con HTML embebido, control digital básico y primeros pasos con AJAX. | Baja a Media |
| Nivel 2 | `02_Intermedio/` | Despacho de páginas web desde MicroSD, peticiones asíncronas con XML, instrumentación analógica con calibración, modularización en C++ y reporte a bases de datos. | Media a Media-Alta |
| Nivel 3 | `03_Avanzado/` | Control multicanal industrial en Arduino Mega, monitoreo fotovoltaico integral, telemetría IoT en la nube, control por voz, pasarelas GSM/SMS y sockets TCP binarios. | Alta a Muy Alta |
| Base | `Sensorica/` | Plantilla estructurada para arranque rápido de adquisición de datos con almacenamiento en MicroSD. | Baja |

---

## Nivel 1: Fundamentos y Servidores Web Básicos (01_Basico)

En este nivel se asientan las bases de la comunicación TCP/IP, sockets de cliente, servidores HTTP con páginas generadas desde memoria de programa y el salto desde recargas completas hacia actualización asíncrona mediante AJAX.

### Práctica 1: Cliente Web (HTTP Client)
- **Directorio:** [01_Basico/Practica1/](01_Basico/Practica1/)
- **Archivo principal:** [Practica1.ino](01_Basico/Practica1/Practica1.ino)
- **Dificultad:** Baja
- **Alcance técnico:** Conexión de Arduino en modo cliente hacia un servidor externo (Google) mediante una petición HTTP GET. Implementación de resolución de red por DHCP y verificación por monitor serial.
- **Hardware requerido:** Arduino Uno + Ethernet Shield W5100.
- **Protocolos y librerías:** HTTP GET (puerto 80), `SPI.h`, `Ethernet.h`.
- **Consideraciones:** Requiere conexión a red con servidor DHCP y salida a Internet. El shield reserva los pines 10 a 13.

### Práctica 1: Control de LED vía Web
- **Directorio:** [01_Basico/Practica1-_Led/](01_Basico/Practica1-_Led/)
- **Archivo principal:** [Preactica_1_-_Led.ino](01_Basico/Practica1-_Led/Preactica_1_-_Led.ino)
- **Dificultad:** Baja
- **Alcance técnico:** Servidor web autónomo con IP fija que despacha una interfaz HTML con un formulario y checkbox para encender o apagar un LED mediante petición GET.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + LED y resistencia en pin 2.
- **Protocolos y librerías:** HTTP Server (puerto 80), `SPI.h`, `Ethernet.h`.
- **Consideraciones:** La página se recarga por completo tras cada interacción (`submit()`). La IP estática por defecto es `192.168.1.177`.

### Práctica 1: Monitor de Switch con Auto-Refresco
- **Directorio:** [01_Basico/Practica1-Swich/](01_Basico/Practica1-Swich/)
- **Archivo principal:** [Practica1-Swich.ino.ino](01_Basico/Practica1-Swich/Practica1-Swich.ino.ino)
- **Dificultad:** Baja
- **Alcance técnico:** Lectura de una entrada digital (interruptor) y presentación de su estado (ON/OFF) en una página web que se actualiza periódicamente mediante la etiqueta `<meta http-equiv="refresh" content="1">`.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + Interruptor o pulsador en pin 3.
- **Protocolos y librerías:** HTTP Server, `SPI.h`, `Ethernet.h`.
- **Consideraciones:** Método de refresco por recarga completa de página en el navegador. Demuestra las limitaciones de tráfico antes de introducir AJAX.

### Práctica 2: Servidor de Control Multicanal y Telemetría
- **Directorio:** [01_Basico/Practica2/](01_Basico/Practica2/)
- **Archivo principal:** [Practica2.ino](01_Basico/Practica2/Practica2.ino)
- **Dificultad:** Media
- **Alcance técnico:** Servidor HTTP con capacidad bidireccional: control de 2 salidas digitales (LEDs) y lectura de 2 entradas analógicas (A0, A1) y 2 entradas digitales (D4, D5) mediante parámetros en la URL (`?Data=1` a `?Data=4`).
- **Hardware requerido:** Arduino Uno + Ethernet Shield + 2 LEDs (pines 2 y 3) + 2 pulsadores (pines 4 y 5 con pull-up).
- **Protocolos y librerías:** HTTP Server con parseo de query strings, `SPI.h`, `Ethernet.h`.
- **Consideraciones:** El parser limita el almacenamiento de la línea HTTP a 50 caracteres para proteger la memoria SRAM.

### Práctica 2: Monitor de Switch con AJAX Periódico
- **Directorio:** [01_Basico/Practica2-AJAX_switch/](01_Basico/Practica2-AJAX_switch/)
- **Archivo principal:** [Practica2-AJAX_switch.ino](01_Basico/Practica2-AJAX_switch/Practica2-AJAX_switch.ino)
- **Dificultad:** Media
- **Alcance técnico:** Actualización del estado de un sensor en tiempo real sin recarga completa de la interfaz visual, implementando peticiones asíncronas continuas (`XMLHttpRequest`) cada 1000 ms desde JavaScript.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + Interruptor en pin 3.
- **Protocolos y librerías:** HTTP Server, JavaScript AJAX, `SPI.h`, `Ethernet.h`.
- **Consideraciones:** Empleo de parámetro de consulta anti-caché (`nocache`) para asegurar que el navegador consulte siempre el estado real del pin.

### Práctica 2: Monitor de Switch con AJAX Bajo Demanda
- **Directorio:** [01_Basico/Practica2-Ajax.ino/](01_Basico/Practica2-Ajax.ino/)
- **Archivo principal:** [Practica2-Ajax.ino.ino](01_Basico/Practica2-Ajax.ino/Practica2-Ajax.ino.ino)
- **Dificultad:** Media
- **Alcance técnico:** Consulta asíncrona disparada exclusivamente por evento del usuario al presionar un botón en la interfaz web, reduciendo drásticamente la carga de red frente al polling continuo.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + Interruptor en pin 3.
- **Protocolos y librerías:** HTTP Server, JavaScript AJAX manual, `SPI.h`, `Ethernet.h`.
- **Consideraciones:** Arquitectura de consulta cliente-servidor optimizada para aplicaciones donde los eventos no son de alta frecuencia.

### Práctica 2: Voltímetro con Estilos CSS
- **Directorio:** [01_Basico/Practica2-Voltimetro/](01_Basico/Practica2-Voltimetro/)
- **Archivo principal:** [Practica2-Voltimetro.ino](01_Basico/Practica2-Voltimetro/Practica2-Voltimetro.ino)
- **Dificultad:** Media
- **Alcance técnico:** Instrumento de medición de dos canales de tensión (A0 con referencia a 5.0 V y A1 con referencia a 3.3 V), control de actuadores y presentación web con CSS responsivo inline.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + Fuentes de tensión de prueba en A0 y A1 + 2 LEDs (pines 2 y 3).
- **Protocolos y librerías:** HTTP Server con detección de fin de cabecera (`\r\n\r\n`), CSS3, `SPI.h`, `Ethernet.h`.
- **Consideraciones:** Incorpora temporizador de timeout en la recepción de peticiones (2 segundos) y retardo de estabilización antes de cerrar la conexión TCP.

### Servidor Web Modular para Control de LED RGB
- **Directorio:** [01_Basico/ServidorWebArduino-controlar_LedRGB-master/](01_Basico/ServidorWebArduino-controlar_LedRGB-master/)
- **Archivo principal:** [RGB_web.ino](01_Basico/ServidorWebArduino-controlar_LedRGB-master/RGB_web.ino)
- **Dificultad:** Media
- **Alcance técnico:** Refactorización de código embebido bajo patrones orientados a objetos en C++. Desacopla la lógica de visualización web (clase `Tela`) de la lógica de control del actuador físico (clase `LedRGB`).
- **Hardware requerido:** Arduino Uno + Ethernet Shield + LED RGB de ánodo o cátodo común en pines PWM.
- **Protocolos y librerías:** HTTP Server modular, clases C++, `SPI.h`, `Ethernet.h`.
- **Consideraciones:** Ejemplo de buenas prácticas de ingeniería de software para proyectos embebidos escalables.

### Estación Meteorológica con Telnet y Telemetría
- **Directorio:** [01_Basico/simpleweatherstation-master/](01_Basico/simpleweatherstation-master/)
- **Archivo principal:** [simpleweatherstation-master.ino](01_Basico/simpleweatherstation-master/simpleweatherstation.ino)
- **Dificultad:** Media-Alta
- **Alcance técnico:** Adquisición de variables ambientales (temperatura y humedad) mediante sensor DHT11, envío periódico de telemetría a servicio externo y servidor concurrente de consola Telnet para administración local.
- **Hardware requerido:** Arduino Mega 2560 + Ethernet Shield + Sensor DHT11.
- **Protocolos y librerías:** HTTP Client (GET), Servidor Telnet (puerto 23), `DHT.h`, `Ethernet.h`, `SPI.h`.
- **Consideraciones:** Implementa dos servicios de red simultáneos en el mismo microcontrolador.

---

## Nivel 2: Interactividad, Almacenamiento SD y Sensores (02_Intermedio)

En este nivel se desacopla la interfaz web del código de Arduino almacenando los recursos en la tarjeta MicroSD, se estandarizan las transferencias asíncronas mediante XML y Canvas Gauges, y se integran sensores físicos con reporte a bases de datos relacionales externas.

### Práctica 2: Voltímetro para Sistema Fotovoltaico
- **Directorio:** [02_Intermedio/Practica2-VoltimetroPanelSolar/](02_Intermedio/Practica2-VoltimetroPanelSolar/)
- **Archivo principal:** [Practica2-VoltimetroPanelSolar.ino](02_Intermedio/Practica2-VoltimetroPanelSolar/Practica2-VoltimetroPanelSolar.ino)
- **Dificultad:** Media
- **Alcance técnico:** Adquisición de tensiones superiores a 5 V (panel solar y batería de 12 V) mediante divisores resistivos calibrados, algoritmo de filtrado por promedio de 10 muestras y reporte web.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + Divisores de tensión (R1=1.2 kΩ, R2=1.5 kΩ o relación 30k/10k) + LEDs indicadores.
- **Protocolos y librerías:** HTTP Server, `SPI.h`, `Ethernet.h`.
- **Consideraciones:** Se incluye variable de compensación de calibración (`CALIBRACION = 0.96`) para contrarrestar tolerancias de los resistores frente a mediciones de multímetro patrón.

### Práctica 2B: Arquitectura Modular y Desacoplamiento
- **Directorio:** [02_Intermedio/Practica2B — Arquitectura Modular/](02_Intermedio/Practica2B%20%E2%80%94%20Arquitectura%20Modular/)
- **Directorio complementario:** [02_Intermedio/Práctica 2B — Arquitectura Modular (complemento)/](02_Intermedio/Pr%C3%A1ctica%202B%20%E2%80%94%20Arquitectura%20Modular%20(complemento)/)
- **Dificultad:** Media
- **Alcance técnico:** Estructuración de aplicaciones web embebidas en múltiples módulos de compilación (`.h` y `.cpp`), encapsulando la capa de red, generadores de respuesta y drivers de actuadores.
- **Hardware requerido:** Arduino Uno o Mega + Ethernet Shield + Actuadores varios.
- **Protocolos y librerías:** Programación orientada a objetos en C++, `Ethernet.h`.
- **Consideraciones:** Facilita el mantenimiento y la adición de nuevas páginas o sensores sin modificar el bucle de ejecución central.

### Práctica 4: Servidor Web con MicroSD y AJAX
- **Directorios:** [02_Intermedio/Practica4-SDAjax/](02_Intermedio/Practica4-SDAjax/) y [02_Intermedio/Practica4-eth_websrv_SD_Ajax/](02_Intermedio/Practica4-eth_websrv_SD_Ajax/)
- **Archivos principales:** [SDAjax.ino](02_Intermedio/Practica4-SDAjax/SDAjax.ino), [Practica4-eth_websrv_SD_Ajax.ino](02_Intermedio/Practica4-eth_websrv_SD_Ajax/Practica4-eth_websrv_SD_Ajax.ino)
- **Dificultad:** Media
- **Alcance técnico:** Servicio del documento HTML raíz (`index.htm`) almacenado en tarjeta MicroSD (FAT16), combinado con una rutina asíncrona AJAX que consulta periódicamente el estado de un pin digital.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + Tarjeta MicroSD (FAT16) + Interruptor en pin 3.
- **Protocolos y librerías:** HTTP Server, `SPI.h`, `Ethernet.h`, `SD.h`.
- **Consideraciones:** El pin 4 queda reservado para la selección de la SD. Se eliminó el uso de la clase `String` para optimizar SRAM.

### Práctica 4: Telemetría con MicroSD y Respuestas en XML
- **Directorios:** [02_Intermedio/Practica4-button/](02_Intermedio/Practica4-button/) y [02_Intermedio/Practica4-eth_websrv_SD_Ajax_XML/](02_Intermedio/Practica4-eth_websrv_SD_Ajax_XML/)
- **Archivos principales:** [button.ino](02_Intermedio/Practica4-button/button.ino), [Practica4-eth_websrv_SD_Ajax_XML.ino](02_Intermedio/Practica4-eth_websrv_SD_Ajax_XML/Practica4-eth_websrv_SD_Ajax_XML.ino)
- **Dificultad:** Media-Alta
- **Alcance técnico:** Lectura simultánea de múltiples canales digitales (switches en pines 7 y 8) y analógicos (potenciómetro en A0/A2), serializando los datos en un documento XML estructurado para su análisis en JavaScript mediante el DOM.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + MicroSD + 2 interruptores + potenciómetro.
- **Protocolos y librerías:** HTTP Server, XML, AJAX (`responseXML`), `SPI.h`, `Ethernet.h`, `SD.h`.
- **Consideraciones:** La carga útil en XML reduce la sobrecarga de transmisión y separa completamente los datos del formato de presentación.

### Práctica 4: Visualización Analógica con Canvas Gauge
- **Directorio:** [02_Intermedio/Practica4-eth_websrv_SD_Ajax_gauge/](02_Intermedio/Practica4-eth_websrv_SD_Ajax_gauge/)
- **Archivo principal:** [Practica4-eth_websrv_SD_Ajax_gauge.ino](02_Intermedio/Practica4-eth_websrv_SD_Ajax_gauge/Practica4-eth_websrv_SD_Ajax_gauge.ino)
- **Dificultad:** Media-Alta
- **Alcance técnico:** Renderizado de un instrumento de aguja analógico en el navegador mediante la librería JavaScript Canvas Gauge (`canv-gauge`), alimentado por lecturas del ADC servidas desde la MicroSD.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + MicroSD con scripts de gauge + potenciómetro o sensor analógico en A0.
- **Protocolos y librerías:** HTTP Server, Canvas Gauge, AJAX, `SD.h`, `Ethernet.h`.
- **Consideraciones:** Los archivos `.js` y licencias deben ubicarse en la raíz de la tarjeta SD.

### Práctica 4: Servicio de Archivos Multimedia (Imágenes JPEG)
- **Directorio:** [02_Intermedio/Practica4-eth_websrv_SD_image/](02_Intermedio/Practica4-eth_websrv_SD_image/)
- **Archivo principal:** [Practica4-eth_websrv_SD_image.ino](02_Intermedio/Practica4-eth_websrv_SD_image/Practica4-eth_websrv_SD_image.ino)
- **Dificultad:** Media
- **Alcance técnico:** Servidor web con capacidad de interpretar peticiones para diferentes tipos MIME, despachando tanto texto HTML como imágenes binarias en formato JPEG desde la tarjeta de memoria.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + MicroSD con `index.htm` e imagen `pic.jpg`.
- **Protocolos y librerías:** HTTP (tipos `text/html` e `image/jpeg`), `SD.h`, `Ethernet.h`.
- **Consideraciones:** Las imágenes deben optimizarse en peso (menos de 50 KB recomendado) para no saturar el canal de transmisión de 10 Mbps del shield ni los buffers de Arduino.

### Práctica 4: Navegación Multipágina desde MicroSD
- **Directorio:** [02_Intermedio/Practica4-eth_websrv_SD_link/](02_Intermedio/Practica4-eth_websrv_SD_link/)
- **Archivo principal:** [Practica4-eth_websrv_SD_link.ino](02_Intermedio/Practica4-eth_websrv_SD_link/Practica4-eth_websrv_SD_link.ino)
- **Dificultad:** Media
- **Alcance técnico:** Implementación de navegación interna entre múltiples páginas HTML (`index.htm` y `page2.htm`) almacenadas en la tarjeta de memoria, con funciones personalizadas de búsqueda en arrays de caracteres (`StrClear`, `StrContains`).
- **Hardware requerido:** Arduino Uno + Ethernet Shield + MicroSD con múltiples páginas HTML enlazadas.
- **Protocolos y librerías:** HTTP Server, `SD.h`, `Ethernet.h`.
- **Consideraciones:** Desacopla la lógica de navegación sin incurrir en fugas de memoria por la clase `String`.

### Logger Ambiental con Envío HTTP POST hacia PHP/MySQL
- **Directorio:** [02_Intermedio/arduino-dht-ethernet-master/](02_Intermedio/arduino-dht-ethernet-master/)
- **Archivo principal:** [dht.ino](02_Intermedio/arduino-dht-ethernet-master/dht.ino)
- **Dificultad:** Media
- **Alcance técnico:** Arduino configurado como cliente que adquiere variables de temperatura y humedad con sensor DHT11 y las despacha periódicamente vía HTTP POST hacia un servidor web con backend PHP para su almacenamiento en base de datos relacional y posterior graficado.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + Sensor DHT11 en pin 2 + Servidor PHP/MySQL (XAMPP o servidor remoto).
- **Protocolos y librerías:** HTTP POST (`application/x-www-form-urlencoded`), `dht.h`, `Ethernet.h`, `SPI.h`.
- **Consideraciones:** Introducción al protocolo POST en contraposición a GET para la transmisión segura y estructurada de parámetros de instrumentación.

### Transmisión de Mediciones Eléctricas hacia MySQL
- **Directorio:** [02_Intermedio/Arduino-W5100-mysql-master/](02_Intermedio/Arduino-W5100-mysql-master/)
- **Archivo principal:** [Arduino-W5100-mysql.ino](02_Intermedio/Arduino-W5100-mysql-master/Arduino-W5100-mysql.ino)
- **Dificultad:** Media
- **Alcance técnico:** Emulación y envío de parámetros de red eléctrica (tensión, corriente, consumo acumulado en kWh) mediante peticiones POST hacia un script receptor en PHP que realiza la inserción en tablas MySQL.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + Servidor con motor de base de datos MySQL.
- **Protocolos y librerías:** HTTP POST, `Ethernet.h`, `SPI.h`.
- **Consideraciones:** Establece las bases para la creación de medidores de energía inteligentes y monitoreo industrial centralizado.

### Termostato Inteligente con Sensores Duales DHT
- **Directorio:** [02_Intermedio/DualDHTthermostat-master/](02_Intermedio/DualDHTthermostat-master/)
- **Archivo principal:** [DualDHTthermostat.ino](02_Intermedio/DualDHTthermostat-master/DualDHTthermostat.ino)
- **Dificultad:** Media-Alta
- **Alcance técnico:** Sistema de control de climatización con dos sensores de distinta precisión (DHT22 y DHT11), cuatro modos operativos (Calefacción, Refrigeración, Automático y Reposo), histéresis programable y servidor web para configuración de consignas.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + Sensor DHT22 (pin 2) + Sensor DHT11 (pin 5) + Módulo de relés de potencia.
- **Protocolos y librerías:** HTTP Server, `DHT.h`, `Ethernet.h`, `SPI.h`.
- **Consideraciones:** Implementa lógica de histéresis para evitar conmutaciones destructivas en compresores o relés de potencia.

### Documentación de Diseño Hardware W5100
- **Directorio:** [02_Intermedio/W5100-master/](02_Intermedio/W5100-master/)
- **Dificultad:** Media (Lectura de ingeniería)
- **Alcance técnico:** Colección de esquemáticos electrónicos, guías de ruteo de pista (layout), diseños de referencia y hojas de datos del circuito integrado Wiznet W5100 y conectores MAGJACK RJ45.
- **Hardware cubierto:** Circuito integrado Wiznet W5100.
- **Consideraciones:** Esencial para comprender el acoplamiento magnético, consideraciones de impedancia diferencial de 100 Ω y diseño de placas PCB personalizadas.

---

## Nivel 3: Sistemas Avanzados, Control Multicanal e IoT (03_Avanzado)

En esta fase se abordan sistemas con alta densidad de entradas/salidas, integración con plataformas de Internet de las Cosas (IoT), servidores web con interfaces modernas y proyectos híbridos que combinan Ethernet con otros medios físicos y protocolos (GSM, RFID, sockets TCP binarios).

### Práctica 5: Matriz de 24 Salidas Digitales en Arduino Mega
- **Directorio:** [03_Avanzado/Practica5-Mega/](03_Avanzado/Practica5-Mega/)
- **Archivo principal:** [eth_websrv_SD_Ajax_24_out.ino](03_Avanzado/Practica5-Mega/eth_websrv_SD_Ajax_24_out/eth_websrv_SD_Ajax_24_out.ino)
- **Dificultad:** Alta
- **Alcance técnico:** Control individual y simultáneo de 24 salidas digitales (pines 26 a 49) mediante checkboxes interactivos en una interfaz web servida desde MicroSD con comunicación asíncrona AJAX.
- **Hardware requerido:** Arduino Mega 2560 + Ethernet Shield + MicroSD + Matriz de 24 LEDs o actuadores con etapas de potencia.
- **Protocolos y librerías:** HTTP Server, AJAX, `SPI.h`, `Ethernet.h`, `SD.h`.
- **Consideraciones:** Exclusivo para Arduino Mega por requerimientos de pines. Los pines 50 a 53 no deben utilizarse ya que están asignados al bus SPI del shield.

### Práctica 5: Servidor Web de Entrada/Salida Integral
- **Directorio:** [03_Avanzado/Practica5-web_server_IO/](03_Avanzado/Practica5-web_server_IO/)
- **Archivo principal:** [eth_websrv_SD_Ajax_in_out.ino](03_Avanzado/Practica5-web_server_IO/eth_websrv_SD_Ajax_in_out/eth_websrv_SD_Ajax_in_out.ino)
- **Dificultad:** Alta
- **Alcance técnico:** Sistema completo de supervisión y control: lectura de 4 canales analógicos (A2 a A5), 3 interruptores de entrada (pines 2, 3 y 5) y control de 4 canales de salida con checkboxes y botones de pulsación momentánea.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + MicroSD + 4 LEDs + 3 switches + 4 sensores/potenciómetros.
- **Protocolos y librerías:** HTTP Server, AJAX, `SD.h`, `Ethernet.h`.
- **Consideraciones:** Emplea buffers de tamaño expandido (60 bytes) y gestión concurrente de múltiples parámetros por petición.

### Práctica 5: Monitor de Sistema Fotovoltaico Completo
- **Directorio:** [03_Avanzado/Practica5-web_server_Panel/](03_Avanzado/Practica5-web_server_Panel/)
- **Archivos principales:** [PanelSolarsimple.ino](03_Avanzado/Practica5-web_server_Panel/PanelSolarsimple/PanelSolarsimple.ino) (con SD y Gauges) y [PanelSolarsimple2.ino](03_Avanzado/Practica5-web_server_Panel/PanelSolarsimple2/PanelSolarsimple2.ino) (sin SD)
- **Dificultad:** Alta
- **Alcance técnico:** Estación de supervisión solar con adquisición de tensión de paneles (A2), banco de baterías 12 V (A3), ciclo de carga PWM (A4) y radiación mediante LDR (A5). Máquina de estados para diagnóstico de operación (Descarga, Equilibrio, Generación) con señalización por LEDs físicos y panel gráfico con Canvas Gauges.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + MicroSD + Divisores resistivos de rango 0-20 V (30kΩ/10kΩ) + LDR + LEDs de estado (D2, D3, D5) + LEDs de control (D6-D9).
- **Protocolos y librerías:** HTTP Server, Canvas Gauges, XML, AJAX, `SD.h`, `Ethernet.h`.
- **Consideraciones:** Incorpora calibración analógica por software (`CALIBRACION = 0.94`) y algoritmo de promediado de muestras.

### Práctica 6: Telemetría en la Nube con ThingSpeak
- **Directorio:** [03_Avanzado/Practica6-ThingSpeak/](03_Avanzado/Practica6-ThingSpeak/)
- **Archivos principales:** [Practica6-ThingSpeak.ino](03_Avanzado/Practica6-ThingSpeak/Practica6-ThingSpeak.ino), [index.htm](03_Avanzado/Practica6-ThingSpeak/index.htm)
- **Dificultad:** Media-Alta
- **Alcance técnico:** Integración de un sistema de medición local con plataformas de nube para IoT. Envío periódico de datos de tensión hacia la API REST de ThingSpeak y visualización paralela local mediante Canvas Gauge.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + MicroSD + Potenciómetro o sensor de tensión en A2 + Conexión a Internet.
- **Protocolos y librerías:** HTTP REST API (ThingSpeak `api.thingspeak.com`), Canvas Gauge, `SD.h`, `Ethernet.h`.
- **Consideraciones:** Requiere cuenta y canal en ThingSpeak con una clave de escritura (`Write API Key`). La cadencia de envío está ajustada a un intervalo de 20 segundos para respetar las restricciones de la API.

### Sistema de Alarmas con Interfaz Web Moderna
- **Directorio:** [03_Avanzado/AlarmasUT-main/](03_Avanzado/AlarmasUT-main/)
- **Dificultad:** Alta
- **Alcance técnico:** Panel de monitoreo de alarmas con diseño web responsivo de última generación utilizando Bootstrap 5, animaciones CSS (Animate.css, WOW.js) y manipulación mediante jQuery, incluyendo un entorno de emulación local en Node.js y Express.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + Sensores de intrusión/contacto seco.
- **Protocolos y librerías:** HTTP Server, Bootstrap 5, jQuery, Node.js para desarrollo.
- **Consideraciones:** Demuestra cómo servir interfaces de alto nivel estético vinculadas a dispositivos embebidos mediante recursos cargados desde CDN o servidor local.

### Monitoreo de Eventos en Tiempo Real con Sensor de Lluvia
- **Directorio:** [03_Avanzado/W5100-Eventos-master/](03_Avanzado/W5100-Eventos-master/)
- **Archivo principal:** [W5100-Eventos.ino](03_Avanzado/W5100-Eventos-master/W5100-Eventos.ino)
- **Dificultad:** Alta
- **Alcance técnico:** Detección de condiciones meteorológicas críticas mediante sensor de lluvia resistivo. Disparo inmediato de notificaciones HTTP POST hacia una base de datos MySQL en un servidor web externo sólo ante la ocurrencia del evento.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + Sensor de lluvia resistivo con placa comparadora + Servidor PHP/MySQL.
- **Protocolos y librerías:** HTTP POST, PHP (`add.php`, `connect.php`), MySQL, `Ethernet.h`.
- **Consideraciones:** Implementa arquitectura orientada a eventos, optimizando el ancho de banda y los ciclos de trabajo al no realizar envíos redundantes cuando no hay cambios de estado.

### Pasarela Web a SMS con Módulo GSM SIM900
- **Directorio:** [03_Avanzado/arduino-sms-web-master/](03_Avanzado/arduino-sms-web-master/)
- **Archivo principal:** [sms.ino](03_Avanzado/arduino-sms-web-master/sms.ino)
- **Dificultad:** Alta
- **Alcance técnico:** Pasarela de comunicaciones híbrida: servidor web Ethernet que recibe número de destino y mensaje de texto a través de un formulario HTML y los transmite a la red celular mediante un módem GSM SIM900 conectado por puerto serie emulado.
- **Hardware requerido:** Arduino Nano o Uno + Ethernet Shield W5100 + Módulo GSM/GPRS SIM900 (EFCOM v1.2) + Tarjeta SIM con plan SMS activo.
- **Protocolos y librerías:** HTTP Server, Comandos AT, `SoftwareSerial.h`, `SIM900.h`, `sms.h`, `Ethernet.h`.
- **Consideraciones:** Gestión cuidadosa de pines compartidos: el módem GSM utiliza pines D2/D3 (SoftwareSerial) y D5/D6 (líneas de control y reset), requiriendo un mapa estricto para evitar colisiones con el shield Ethernet.

### Control Domótico por Reconocimiento de Voz
- **Directorio:** [03_Avanzado/Arduino-Voice-control-master/](03_Avanzado/Arduino-Voice-control-master/)
- **Archivo principal:** [Voice.ino](03_Avanzado/Arduino-Voice-control-master/Voice.ino)
- **Dificultad:** Alta
- **Alcance técnico:** Integración de la API de reconocimiento por voz del navegador (Web Speech API) con hardware embebido. El usuario dicta comandos de voz en un cliente web, un backend PHP almacena las instrucciones y Arduino consulta periódicamente como cliente HTTP para accionar cargas.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + Cargas o LEDs en pines de salida + Servidor web con soporte PHP y conexión HTTPS (requerida por navegadores para la Web Speech API).
- **Protocolos y librerías:** HTTP GET, Web Speech API (JavaScript), PHP, `Ethernet.h`.
- **Consideraciones:** Latencia de respuesta basada en el intervalo de sondeo (5 segundos en la implementación base).

### Servidor TCP para Sincronización con Stellarium
- **Directorio:** [03_Avanzado/conectrar_stellarium-main/](03_Avanzado/conectrar_stellarium-main/)
- **Archivo principal:** [conectrar_stellarium.ino](03_Avanzado/conectrar_stellarium-main/conectrar_stellarium.ino)
- **Dificultad:** Alta
- **Alcance técnico:** Comunicación de bajo nivel mediante sockets TCP en puerto dedicado (10000). Implementación del protocolo de control de telescopios virtuales de Stellarium para recibir coordenadas ecuatoriales celestes y calcular ángulos de guiado altacimutales.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + PC con Stellarium configurado en modo telescopio remoto.
- **Protocolos y librerías:** Sockets TCP puros en puerto 10000, protocolo binario/texto Stellarium, `Ethernet.h`.
- **Consideraciones:** Único proyecto enfocado en sockets de transporte directos sin sobrecarga de capas de aplicación HTTP.

### Control de Acceso e Identificación RFID con W5500
- **Directorio:** [03_Avanzado/rfid-main/](03_Avanzado/rfid-main/)
- **Archivo principal:** [rfid.ino](03_Avanzado/rfid-main/rfid.ino)
- **Dificultad:** Alta
- **Alcance técnico:** Lector de tarjetas y llaveros inteligentes de radiofrecuencia (MFRC522) con registro temporal de eventos mediante reloj de tiempo real (RTC DS1307 en bus I2C), buzzer de señalización y despacho de registros de acceso hacia una base de datos web mediante controlador Ethernet W5500.
- **Hardware requerido:** Arduino Uno + Módulo Ethernet W5500 + Lector RFID MFRC522 (pin 9 como SS, pin 8 como RST) + RTC DS1307 (I2C: pines A4/A5) + Buzzer en pin 3.
- **Protocolos y librerías:** `MFRC522.h`, `DS1307.h`, `Wire.h`, `Ethernet_W5500.h`, `SPI.h`.
- **Consideraciones:** El bus SPI es compartido entre el lector RFID y el controlador Ethernet. Es obligatorio alternar las líneas de chip select (D9 para RFID y D10 para Ethernet) deshabilitando el dispositivo secundario antes de inicializar la interfaz de red.

### Sistema Integral de Automatización para Invernaderos (Capstone)
- **Directorio:** [03_Avanzado/WebestufaServer_ArduinoMega2560_EthernetW5100-master/](03_Avanzado/WebestufaServer_ArduinoMega2560_EthernetW5100-master/)
- **Archivo principal:** [WebestufaServer_ArduinoMega2560_EthernetW5100.ino](03_Avanzado/WebestufaServer_ArduinoMega2560_EthernetW5100-master/WebestufaServer_ArduinoMega2560_EthernetW5100.ino)
- **Dificultad:** Muy Alta
- **Alcance técnico:** Proyecto integral de automatización climática con Arduino Mega 2560. Combina lectura ambiental (DHT11), reloj en tiempo real (RTC DS1307), pantalla de visualización física LCD 16x2 por bus I2C, control de potencia mediante módulo de 8 relés para actuadores de 110/220 VAC y panel web de administración remota.
- **Hardware requerido:** Arduino Mega 2560 + Ethernet Shield + MicroSD + Sensor DHT11 en A0 + Módulo de 8 relés + LCD I2C (dirección 0x27) + RTC DS1307 en pines I2C (20 y 21 del Mega).
- **Protocolos y librerías:** HTTP Server, I2C (`Wire.h`), `LiquidCrystal_I2C.h`, `RTClib.h`, `DHT.h`, `Ethernet.h`, `SD.h`.
- **Consideraciones:** Máxima integración de periféricos en buses SPI e I2C simultáneos. En el Arduino Mega, el pin 53 debe configurarse explícitamente como salida para garantizar la operación del controlador SPI por hardware.

---

## Módulo Base de Sensorización (Sensorica)

Plantilla de referencia simplificada para proyectos de adquisición y despacho de datos.

- **Directorio:** [Sensorica/](Sensorica/)
- **Archivos principales:** [Sensorica.ino](Sensorica/Sensorica.ino), [index.html](Sensorica/index.html)
- **Dificultad:** Baja
- **Alcance técnico:** Estructura modular estándar para inicializar el controlador W5100 con IP estática, montar el sistema de archivos FAT16 de la tarjeta MicroSD y responder con la página principal `index.html`.
- **Hardware requerido:** Arduino Uno + Ethernet Shield + Tarjeta MicroSD.
- **Protocolos y librerías:** HTTP Server, `SD.h`, `Ethernet.h`, `SPI.h`.
- **Consideraciones:** Punto de partida recomendado antes de desarrollar proyectos personalizados con sensores analógicos o I2C.

---

## Esquemas y Recursos de Documentación

### Diagramas de Conexión
- **Esquema de conexionado para Arduino Mega:** [Schematics/MegaW5100.fzz](Schematics/MegaW5100.fzz). Archivo editable en Fritzing que documenta la integración entre Arduino Mega 2560, Ethernet Shield W5100 y periféricos asociados.

### Documentación y Guías de Referencia
Ubicados en la carpeta [00_Recursos/](00_Recursos/):
- **Guías técnicas detalladas:** [00_Recursos/Guias/](00_Recursos/Guias/)
  - [Resumen_Practicas.md](00_Recursos/Guias/Resumen_Practicas.md): Fichas técnicas individuales con consideraciones operativas y de conexión.
  - [Propuesta_Integracion_Proyectos.md](00_Recursos/Guias/Propuesta_Integracion_Proyectos.md): Análisis arquitectónico para la inclusión de proyectos de terceros.
  - Guías interactivas en HTML sobre servidores SD y sistemas de monitoreo solar.
- **Datasheets y Bootloaders:** [00_Recursos/Documentacion/](00_Recursos/Documentacion/)
  - Hoja de datos completa del chip Wiznet W5100 (`W5100_Datasheet_v1_1_6.pdf`).
  - Esquemáticos oficiales de la placa Arduino Ethernet (`Arduino-Ethernet-v112pg.pdf`).
  - Bootloader con soporte de red para W5100 (`optiboot-w5100-latest.tgz`).

---

## Guía de Configuración Inicial y Diagnóstico

### 1. Parámetros de Red
Antes de cargar el firmware en su microcontrolador:
- Configure una dirección MAC única por dispositivo:
  ```cpp
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  ```
- Si emplea IP estática, verifique que se encuentre dentro del rango de subred de su enrutador y fuera del rango de asignación DHCP:
  ```cpp
  IPAddress ip(192, 168, 1, 177);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  ```

### 2. Preparación de la Tarjeta MicroSD
- Tarjetas de capacidad recomendada: 2 GB a 8 GB.
- Formato: FAT16 o FAT32 (tamaño de asignación estándar).
- Los archivos web (`index.htm`, scripts, imágenes) deben situarse en la raíz de la tarjeta, nunca dentro de subdirectorios, a menos que el código implemente navegación de carpetas.

### 3. Procedimiento de Diagnóstico por Monitor Serial
- Conecte el cable USB y abra el monitor serie de Arduino IDE configurado a **9600 baudios**.
- Si el monitor no muestra la IP local (`Ethernet.localIP()`):
  - Compruebe que el cable RJ45 esté conectado y los LEDs de LINK/ACT del conector MAGJACK estén encendidos.
  - Compruebe que no haya pines en cortocircuito entre el shield y la placa base.
- Si falla la inicialización de la MicroSD:
  - Verifique que la línea Pin 10 (Ethernet CS) se haya puesto en `HIGH` antes de ejecutar `SD.begin(4)`.