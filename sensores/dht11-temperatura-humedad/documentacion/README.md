# Sensor DHT11 - Temperatura y Humedad

## Descripción

Sensor de temperatura y humedad ambiente. Utilizado para monitorear
las condiciones climáticas en el campo y enviar los datos a Firebase
Realtime Database.

## Hardware necesario

| Componente | Descripción |
|------------|-------------|
| ESP32 WROOM-32 | Microcontrolador con WiFi integrado |
| DHT11 (3 pines) | Sensor de temperatura y humedad |
| Cables Dupont | Conexión entre componentes |

## Conexión

| DHT11 | ESP32 |
|-------|-------|
| +     | 3.3V  |
| -     | GND   |
| S     | GPIO4 |

## Especificaciones del DHT11

| Parámetro | Valor |
|-----------|-------|
| Temperatura | 0°C a 50°C |
| Precisión temperatura | ±2°C |
| Humedad | 20% a 80% |
| Precisión humedad | ±5% |
| Intervalo de lectura | mínimo 2 segundos |

## Dependencias

- Librería DHT sensor library by Adafruit
- Librería Firebase ESP32 Client by Mobizt (FirebaseESP32.h)

## Ciclo de funcionamiento

Con deep sleep implementado el ESP32 opera en ciclos de 5 minutos:

- 19 segundos activo: Arranque, WiFi, NTP, Firebase, Lectura, Envío
- 287 segundos durmiendo: Deep sleep

Ventajas:
- 96% del tiempo en sleep, mayor duración de batería
- Reinicio completo en cada ciclo, evita procesos colgados
- Timeouts configurables en config.h

## Timeouts

| Proceso | Tiempo máximo |
|---------|--------------|
| Conexión WiFi | 20 segundos |
| Sincronización NTP | 15 segundos |

Si se supera algún timeout el LED parpadea 5 veces rápido y el ESP32 entra en sleep.

## LED onboard (GPIO2)

El LED azul onboard indica el estado del sistema mediante señales visuales.
No requiere hardware adicional.

| Estado | Señal | Descripción |
|--------|-------|-------------|
| Todo OK | 3 destellos cortos (150ms) | Lectura y envío exitosos |
| Error sensor | 2 destellos largos (800ms) | Lectura inválida del DHT11 |
| Error Firebase | 5 destellos rápidos (100ms) | Fallo al enviar datos |
| Error WiFi o NTP | 5 destellos rápidos (100ms) | Timeout de conexión |
| Deep sleep | LED apagado | ESP32 en modo de bajo consumo |

### Diagnóstico rápido en el campo

- 3 destellos cortos y apagado: todo funcionando correctamente
- 2 destellos largos: revisar conexión del sensor DHT11
- 5 destellos rápidos: revisar WiFi o Firebase
- LED apagado: en sleep o sin energía

## Archivos

| Archivo | Descripción |
|---------|-------------|
| codigo/dht11_firebase.ino | Código principal |
| codigo/config.h.ejemplo | Plantilla de configuración |
| esquema/conexion.svg | Diagrama de conexión |

## Configuración

Copiás el archivo de ejemplo y completás con tus datos:

    cp codigo/config.h.ejemplo codigo/dht11_firebase/config.h

Editás config.h con tus credenciales:
- WIFI_SSID
- WIFI_PASSWORD
- API_KEY (Firebase)
- PROJECT_ID (Firebase)
- DATABASE_URL (Firebase Realtime Database)
- TIEMPO_SLEEP (segundos de deep sleep, default 287)
- TIMEOUT_WIFI (ms, default 20000)
- TIMEOUT_NTP (ms, default 15000)

## Estado

Funcionando
