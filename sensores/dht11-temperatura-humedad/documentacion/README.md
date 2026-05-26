# Sensor DHT11 - Temperatura y Humedad

## Descripción

Sensor de temperatura y humedad ambiente. Utilizado para monitorear
las condiciones climáticas en el campo y enviar los datos a Firebase.

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
- Librería **Firebase ESP Client** by Mobizt

## Funcionamiento

1. El ESP32 se conecta a la red WiFi
2. Se autentica en Firebase
3. Lee temperatura y humedad del DHT11 cada 5 minutos
4. Envía los datos a Firebase Realtime Database
5. En caso de error de lectura envía una alerta

## Archivos

| Archivo | Descripción |
|---------|-------------|
| `codigo/dht11_firebase.ino` | Código principal |
| `esquema/conexion.png` | Diagrama de conexión |

## Estado

🔧 En desarrollo
