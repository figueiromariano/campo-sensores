# Campo Sensores

Repositorio de sensores para el proyecto **Agro** de monitoreo agrícola-ganadero.

## Descripción

Este repositorio contiene el código y la documentación de los sensores
utilizados en el campo para medir condiciones climáticas y otros parámetros
relevantes para la actividad agrícola-ganadera.

Los datos relevados por los sensores son enviados a Firebase Realtime Database
y consultados por el agente de toma de decisiones
[agente-campo](https://github.com/figueiromariano/agente-campo).

## Tecnologías

- **Hardware:** ESP32 WROOM-32
- **Conectividad:** WiFi
- **Base de datos:** Firebase Realtime Database
- **Lenguaje:** C++ (Arduino IDE 1.8.19)
- **Sistema operativo desarrollo:** Linux Mint

## Sensores disponibles

| Sensor | Función | Estado |
|--------|---------|--------|
| DHT11  | Temperatura y humedad ambiente | Funcionando |

## Estructura del repositorio

    campo-sensores/
    ├── sensores/
    │   └── dht11-temperatura-humedad/
    │       ├── codigo/
    │       │   ├── dht11_firebase.ino
    │       │   └── config.h.ejemplo
    │       ├── documentacion/
    │       │   └── README.md
    │       └── esquema/
    └── docs/
        └── arquitectura-general.md

## Configuración inicial

1. Clonar el repositorio
2. Instalar Arduino IDE
3. Instalar librerías: DHT sensor library (Adafruit) y Firebase ESP32 Client (Mobizt)
4. Copiar config.h.ejemplo a config.h dentro de la carpeta del sensor
5. Completar credenciales WiFi y Firebase en config.h
6. Cargar el sketch al ESP32

## Proyecto relacionado

- [agente-campo](https://github.com/figueiromariano/agente-campo)
