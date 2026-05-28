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

- Librería **DHT sensor library** by Adafruit
- Librería **Firebase ESP32 Client** by Mobizt (FirebaseESP32.h)

## Funcionamiento

1. El ESP32 se conecta a la red WiFi
2. Sincroniza el tiempo via NTP (pool.ntp.org, UTC-3)
3. Se autentica en Firebase de forma anónima
4. Lee temperatura y humedad del DHT11 cada 5 minutos
5. Envía los datos a Firebase Realtime Database
6. En caso de error de lectura informa por puerto serie

## Estructura de datos en Firebase
sensores/
dht11/
ultima_lectura/
temperatura: 26.10
humedad: 32.00
unidad_temp: "C"
timestamp: 12345678

## Archivos

| Archivo | Descripción |
|---------|-------------|
| `codigo/dht11_firebase.ino` | Código principal |
| `codigo/config.h.ejemplo` | Plantilla de configuración |
| `esquema/conexion.png` | Diagrama de conexión (pendiente) |

## Configuración

Copiás el archivo de ejemplo y completás con tus datos:

```bash
cp codigo/config.h.ejemplo codigo/dht11_firebase/config.h
```

Editás config.h con tus credenciales:
- WIFI_SSID
- WIFI_PASSWORD
- API_KEY (Firebase)
- PROJECT_ID (Firebase)
- DATABASE_URL (Firebase Realtime Database)

## Estado

✅ Funcionando
