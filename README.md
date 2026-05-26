# Campo Sensores

Repositorio de sensores para el proyecto **Agro** de monitoreo agrícola-ganadero.

## Descripción

Este repositorio contiene el código y la documentación de los sensores
utilizados en el campo para medir condiciones climáticas y otros parámetros
relevantes para la actividad agrícola-ganadera.

Los datos relevados por los sensores son enviados a Firebase y consultados
por el agente de toma de decisiones [agente-campo](https://github.com/figueiromariano/agente-campo).

## Tecnologías

- **Hardware:** ESP32 WROOM-32
- **Conectividad:** WiFi
- **Base de datos:** Firebase
- **Lenguaje:** C++ (Arduino IDE)

## Sensores disponibles

| Sensor | Función | Estado |
|--------|---------|--------|
| DHT11  | Temperatura y humedad ambiente | 🔧 En desarrollo |

## Estructura del repositorio

campo-sensores/
├── sensores/
│   └── dht11-temperatura-humedad/
│       ├── codigo/
│       ├── documentacion/
│       └── esquema/
└── docs/
└── arquitectura-general.md

## Proyecto relacionado

- [agente-campo](https://github.com/figueiromariano/agente-campo)
