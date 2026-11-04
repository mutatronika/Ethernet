# Comparativa Técnica: Practica5-IO vs Practica5-web_server_Panel

## Resumen Ejecutivo
- **IO**: Funciona ✓ (HTML simple, sin dependencias complejas)
- **Panel**: Fue corregido ✓ (HTML complejo con gauges, requería validación)

---

## Diferencias en Estructura

### Arduino - Lado del Servidor

#### **Practica5-IO** (eth_websrv_SD_Ajax_in_out.ino)
```cpp
// Lectura simple de entradas análogas y digitales
for (count = 2; count <= 5; count++) {  // A2 a A5
    analog_val = analogRead(count);
    cl.print("<analog>");
    cl.print(analog_val);
    cl.println("</analog>");
}

// XML respuesta: <analog>, <switch>, <LED>
// Complejidad: BAJA
```

#### **Practica5-Panel** (PanelSolarsimple.ino)
```cpp
// Lectura con procesamiento avanzado
float leerVoltaje(byte pin) {
  long suma = 0;
  for (int i = 0; i < NUM_MUESTRAS; i++) {
    suma += analogRead(pin);
    delay(2);
  }
  float lecturaPromedio = suma / (float)NUM_MUESTRAS;
  float voltajePin = (lecturaPromedio * VREF) / 1024.0;
  float voltajeReal = voltajePin * FACTOR_DIVISION * CALIBRACION;
  return voltajeReal;
}

// XML respuesta: <panel_gauge>, <bateria_gauge>, <pwm_gauge>, 
//                <panel_volt>, <bateria_volt>, <pwm_volt>, <ldr>,
//                <LED>, <LED_RED>, <LED_YELLOW>, <LED_GREEN>
// Complejidad: ALTA
// Características: Promediado, calibración, múltiples lecturas
```

---

## Diferencias en HTML/JavaScript

### **Practica5-IO** (index.htm)
```html
<!-- Estructura: SIMPLE Y DIRECTA -->
<!DOCTYPE html>
<html>
<head>
    <title>Arduino Ajax I/O IotLabs</title>
    <script>
        // JavaScript simple, sin minificación
        function GetArduinoIO() {
            // Obtiene valores XML
            // Actualiza elementos HTML directamente
        }
    </script>
    <style>
        /* CSS básico */
    </style>
</head>
<body onload="GetArduinoIO()">  <!-- ✓ Funciona sin delay -->
    <h1>Arduino Ajax I/O. IotLabs</h1>
    <div class="IO_box">
        <!-- Divs simples para mostrar valores -->
    </div>
</body>
</html>
```

**Características**:
- ✓ Sin dependencias externas
- ✓ Sin librerías complicadas
- ✓ Sin canvas
- ✓ OnLoad directo funciona

---

### **Practica5-Panel** (index.htm - ANTES DE CORRECCIONES)
```html
<!-- Estructura: COMPLEJA CON GAUGES -->
<!DOCTYPE html>
<html>
<head>
    <title>Monitor Panel Solar 12V</title>
    <script>
        // 1. Código JavaScript minificado con eval()
        eval(function(p,a,c,k,e,r){
            // ... código obfuscado para librería Gauge
        }('...'));
        
        // 2. Variables globales para gauges
        var panel_gauge_val = 0;
        var bateria_gauge_val = 0;
        
        // 3. Función GetArduinoIO que usa Gauge.Collection
        function GetArduinoIO() {
            // Intenta usar: Gauge.Collection.get('panel_gauge')
            // ❌ ERROR: Gauge puede no estar listo
        }
    </script>
</head>
<body onload="GetArduinoIO()">  <!-- ❌ Falla: Gauge no está listo -->
    <!-- Canvas para gauges -->
    <canvas id="panel_gauge" data-type="canv-gauge" width="200" height="200"></canvas>
</body>
</html>
```

**Problemas**:
- ✗ Eval() minificado puede no ejecutarse completamente antes de onload
- ✗ Canvas requiere que Gauge esté listo
- ✗ Sin validación de Gauge.Collection

---

## Diferencias en XML Enviado

### **Practica5-IO** - XML Simple
```xml
<?xml version="1.0" ?>
<inputs>
    <analog>512</analog>   <!-- A2 -->
    <analog>600</analog>   <!-- A3 -->
    <analog>700</analog>   <!-- A4 -->
    <analog>800</analog>   <!-- A5 -->
    <switch>ON</switch>    <!-- Pin 2 -->
    <switch>OFF</switch>   <!-- Pin 3 -->
    <switch>ON</switch>    <!-- Pin 5 -->
    <LED>checked</LED>     <!-- LED1 -->
    <LED>unchecked</LED>   <!-- LED2 -->
    <LED>on</LED>          <!-- LED3 -->
    <LED>off</LED>         <!-- LED4 -->
</inputs>
```

### **Practica5-Panel** - XML Complejo
```xml
<?xml version="1.0" ?>
<inputs>
    <!-- Valores ADC para gauges -->
    <panel_gauge>512</panel_gauge>
    <bateria_gauge>600</bateria_gauge>
    <pwm_gauge>700</pwm_gauge>
    
    <!-- Voltajes calculados y calibrados -->
    <panel_volt>12.45</panel_volt>
    <bateria_volt>13.20</bateria_volt>
    <pwm_volt>8.90</pwm_volt>
    
    <!-- Sensores -->
    <ldr>450</ldr>
    
    <!-- Controles -->
    <LED>on</LED>
    <LED>off</LED>
    <LED>on</LED>
    <LED>off</LED>
    
    <!-- Estado del sistema -->
    <LED_RED>on</LED_RED>
    <LED_YELLOW>off</LED_YELLOW>
    <LED_GREEN>off</LED_GREEN>
</inputs>
```

---

## Diferencias en Presentación

| Aspecto | **IO** | **Panel** |
|---------|--------|----------|
| **Gauge/Medidores** | ✗ No | ✓ Sí (3) |
| **Estilo Visual** | Básico | Moderno (gradientes) |
| **Información Mostrada** | 4 analog + 3 switches | Voltajes, LDR, LEDs, gauges |
| **Responsividad** | Fija | Media query para móvil |
| **Interactividad** | Checkboxes + Botones | Checkboxes + Botones |
| **Complejidad CSS** | 5 KB | 15 KB |
| **Complejidad HTML** | 8 KB | 20 KB |
| **Dependencias JS** | 0 | 1 (Gauge obfuscada) |

---

## Correcciones Aplicadas a Panel

### 1. Timing de Inicialización
```javascript
// ANTES: ❌ Carga inmediata
<body onload="GetArduinoIO()">

// DESPUÉS: ✓ Con delay
<body onload="setTimeout(function() { GetArduinoIO(); }, 1500)">
```

### 2. Validación de Gauge
```javascript
// ANTES: ❌ Sin validación
Gauge.Collection.get('panel_gauge').setValue(panel_gauge_val);

// DESPUÉS: ✓ Con validación
if (typeof Gauge !== 'undefined' && Gauge.Collection) {
    var g = Gauge.Collection.get('panel_gauge');
    if (g) g.setValue(panel_gauge_val);
}
```

### 3. Manejo de Errores
```javascript
// ANTES: ❌ Sin try-catch
var panels = this.responseXML.getElementsByTagName('panel_gauge');
// ... procesamiento directo

// DESPUÉS: ✓ Con try-catch
try {
    var panels = this.responseXML.getElementsByTagName('panel_gauge');
    // ... procesamiento
} catch(e) {
    console.error('Error procesando respuesta XML: ' + e.message);
}
```

### 4. Debugging
```javascript
// AGREGADO: Función para diagnosticar problemas
window.onload = function() {
    console.log('Página cargada - Waiting for gauges and Arduino data...');
    setTimeout(function() {
        if (typeof Gauge !== 'undefined') {
            console.log('✓ Gauge library loaded');
        } else {
            console.warn('✗ Gauge library NOT loaded');
        }
    }, 500);
};
```

---

## Checklist de Verificación

### Arduino Configuración
- [x] Código compila sin errores ✓
- [x] Serial muestra "OK - SD inicializada" ✓
- [x] Serial muestra "OK - Archivo index.htm encontrado" ✓
- [x] Envía XML con todos los valores ✓

### HTML/JavaScript
- [x] OnLoad tiene delay de 1500ms ✓
- [x] GetArduinoIO valida Gauge.Collection ✓
- [x] Tiene try-catch en procesamiento XML ✓
- [x] Tiene función debug en window.onload ✓

### Navegador
- [ ] Consola (F12) muestra "✓ Gauge library loaded"
- [ ] Gauges aparecer y se actualizan
- [ ] Voltajes se muestran correctamente
- [ ] LEDs responden al hacer clic
- [ ] No hay errores rojos en consola

---

## Conclusiones

| Criterio | IO | Panel (Corregido) |
|----------|-------|-------|
| **Complejidad Arduino** | Baja | Alta |
| **Complejidad Frontend** | Baja | Alta |
| **Robusted Código** | Buena | Mejorada ✓ |
| **Funcionalidad** | Entradas/Salidas | Monitoreo completo |
| **Curva de Aprendizaje** | Fácil | Media |
| **Debugging Facilitado** | Sí | Sí (mejorado) ✓ |

**Recomendación**: 
- Comienza con **IO** para aprender conceptos básicos
- Usa **Panel** para proyectos más complejos de monitoreo
- Las correcciones aplicadas ahora hacen Panel más confiable

---

**Documentación creada**: 9 de febrero de 2026
