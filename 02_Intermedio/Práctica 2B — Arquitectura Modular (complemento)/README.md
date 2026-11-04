# 🌈 Control LED RGB vía Web con Arduino + Ethernet Shield

Control de un LED RGB desde un navegador web utilizando un servidor HTTP embebido en Arduino, con efecto arcoíris automático cuando no hay conexión activa.

---

## 📋 Descripción del Proyecto

Este proyecto implementa un **servidor web embebido** en Arduino que permite controlar los colores de un LED RGB (módulo KY-016) desde cualquier dispositivo conectado a la misma red local, simplemente abriendo un navegador.

### Funcionalidades
- Control de colores desde el navegador: **Rojo, Verde, Azul, Amarillo, Blanco**
- Botón para **apagar** el LED
- Modo **arcoíris automático** cuando no hay conexión activa
- Botón **Salir** que regresa al modo arcoíris
- Diagnóstico por **Serial Monitor**

---

## 🔧 Lista de Componentes

| Componente | Cantidad | Descripción |
|---|---|---|
| Arduino Uno **o** Arduino Mega | 1 | Microcontrolador principal |
| Ethernet Shield W5100 | 1 | Módulo de comunicación Ethernet |
| Módulo LED RGB KY-016 | 1 | LED de tres colores (cátodo común) |
| Cable de red RJ45 | 1 | Conexión al router/switch |
| Cables jumper | c/n | Conexiones entre módulo y shield |
| Cable USB | 1 | Programación y alimentación |

> ⚠️ El Ethernet Shield W5100 **ocupa los pines 10, 11, 12 y 13** para SPI. Asegúrate de no usarlos para otros propósitos.

---

## 🔌 Esquema de Conexiones

### LED RGB KY-016 → Arduino

| Pin KY-016 | Pin Arduino | Color de cable sugerido |
|---|---|---|
| R (Rojo) | Pin **11** | Rojo |
| G (Verde) | Pin **9** | Verde |
| B (Azul) | Pin **10** | Azul |
| GND (-) | GND | Negro |

> ✅ Los pines 9, 10 y 11 son PWM, necesarios para `analogWrite()`.

### Ethernet Shield

El shield se monta **directamente sobre el Arduino** (apilado). Solo conecta el cable RJ45 del shield al router o switch.

```
[Router/Switch] ──── RJ45 ──── [Ethernet Shield W5100]
                                        │
                               [Arduino Uno / Mega]
                                        │
                               [Módulo KY-016 RGB]
```

---

## 💻 Instrucciones de Instalación

### 1. Requisitos de software

- [Arduino IDE](https://www.arduino.cc/en/software) versión 1.8 o superior
- Librería **Ethernet** (incluida en el IDE)
- Librería **SPI** (incluida en el IDE)

### 2. Configurar la IP

Verifica el rango de tu red local antes de cargar el código.  
En Windows abre `cmd` y ejecuta:

```
ipconfig
```

Busca la línea **Puerta de enlace predeterminada**, por ejemplo `192.168.1.1`.  
Luego asigna una IP libre en ese mismo rango al Arduino:

```cpp
// Si tu red es 192.168.1.x
IPAddress ip(192, 168, 1, 177);

// Si tu red es 10.0.0.x
IPAddress ip(10, 0, 0, 20);
```

### 3. Cargar el código

1. Abre el archivo `.ino` en Arduino IDE
2. Selecciona la placa correcta: `Herramientas → Placa → Arduino Uno` (o Mega)
3. Selecciona el puerto COM correcto
4. Carga el sketch con el botón **→ Subir**

### 4. Verificar en Serial Monitor

Abre el Serial Monitor (`Ctrl + Shift + M`) a **9600 baudios**.  
Deberías ver:

```
=== INICIANDO ===
Cable: OK
IP: 192.168.1.177
Gateway: 192.168.1.1
Máscara: 255.255.255.0
Servidor iniciado
=================
```

### 5. Acceder desde el navegador

En cualquier dispositivo de la misma red, abre el navegador y escribe la IP del Arduino:

```
http://192.168.1.177
```

---

## 🛠️ Solución de Problemas

### ❌ La IP aparece como `0.0.0.0`

| Causa | Solución |
|---|---|
| Shield mal encajado | Retirar y reinsertar firmemente el Ethernet Shield |
| Cable RJ45 no conectado | Verificar que el cable esté conectado al router/switch |
| IP fuera del rango de red | Ajustar la IP al rango de tu red (ver sección de instalación) |
| MAC duplicada | Cambiar la dirección MAC en el código por una diferente |

**Prueba con DHCP** para que el router asigne la IP automáticamente:
```cpp
if (Ethernet.begin(mac) == 0) {
  Ethernet.begin(mac, ip); // Fallback a IP estática
}
Serial.println(Ethernet.localIP());
```

---

### ❌ La página se queda cargando

Esto ocurre cuando el navegador no sabe cuándo termina la respuesta HTTP.

**Solución:** Asegúrate de incluir `Content-Length` en los headers:
```cpp
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.print("Content-Length: ");
client.println(html.length());  // longitud exacta del HTML
client.println("Connection: close");
client.println();
client.println(html);
```

---

### ❌ ERR_CONNECTION_RESET en el navegador

El Arduino rechaza la conexión antes de responder.

| Causa | Solución |
|---|---|
| Shield W5100 no inicializado | Agregar `delay(2000)` al inicio del `setup()` |
| IP en conflicto con otro dispositivo | Cambiar la IP a una que no esté en uso |
| Cable de red defectuoso | Probar con otro cable RJ45 |
| Usar `Connection: keep-alive` con W5100 | Cambiar a `Connection: close` |

---

### ❌ El LED no enciende

| Causa | Solución |
|---|---|
| Pines invertidos | Verificar la tabla de conexiones KY-016 |
| Pin PWM ocupado por el shield | El pin 10 lo usa el shield; reasignar LED a pines libres |
| GND no conectado | Verificar conexión a tierra del módulo KY-016 |

---

### ❌ Los botones no hacen nada

Verifica en el **Serial Monitor** que las peticiones HTTP estén llegando.  
Si no aparece nada al presionar un botón, el problema es de red.  
Si aparece la petición pero el LED no responde, revisar la detección de rutas:

```cpp
// Usar indexOf() es más robusto que endsWith()
if (HTTP_req.indexOf("btRojo") > -1) { ... }
```

---

## 📁 Estructura del Código

```
proyecto/
│
├── sketch.ino        ← Código principal (servidor + control LED)
└── README.md         ← Este archivo
```

---

## 📌 Notas Importantes

- El Ethernet Shield W5100 **no soporta múltiples conexiones simultáneas**
- El efecto arcoíris usa `analogWrite()`, que requiere **pines PWM**
- Si se usa **Arduino Uno**, la memoria RAM es limitada; evitar Strings muy largas
- El **Arduino Mega** es más recomendable por su mayor memoria disponible

---

## 👨‍💻 Referencias

- [Documentación librería Ethernet - Arduino](https://www.arduino.cc/reference/en/libraries/ethernet/)
- [Starting Electronics - Arduino Web Server](http://startingelectronics.org)
- [Módulo KY-016 RGB LED - Datasheet](https://arduinomodules.info/ky-016-rgb-full-color-led-module/)
