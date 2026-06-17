# Arquitectura modular propuesta - Nucleo + Sensores

## Contexto

A medida que se agreguen mas sensores y mas dispositivos ESP32, el
codigo actual (todo junto en un solo sketch) se vuelve dificil de
mantener. Se propone una arquitectura de nucleo + modulos de sensores.

## Estructura propuesta

    campo-sensores/
    ├── core/
    │   ├── wifi_manager.h
    │   ├── firebase_manager.h
    │   ├── led_utils.h
    │   ├── web_server.h
    │   └── ota_manager.h
    ├── modulos_sensores/
    │   ├── dht11.h
    │   ├── bmp180.h
    │   ├── ldr.h
    │   └── (futuros sensores...)
    ├── dispositivos/
    │   ├── esp32-galpon/
    │   │   ├── config_sensores.h
    │   │   └── config.h
    │   └── esp32-otro/
    │       ├── config_sensores.h
    │       └── config.h

## Como funciona

- El nucleo (core/) maneja WiFi, Firebase, OTA, deep sleep, LED, web server
- Cada sensor es un modulo independiente con interfaz estandar:
  setup(), leer(), publicar()
- Cada dispositivo ESP32 tiene su propio config_sensores.h que activa
  los modulos que necesita mediante defines:

      #define SENSOR_DHT11   true
      #define SENSOR_BMP180  true
      #define SENSOR_LDR     false

- Se sigue compilando un binario por dispositivo (no hay carga dinamica
  de codigo en runtime, eso es muy complejo en C++/Arduino)
- OTA sigue funcionando igual, se sube el binario ya armado

## Ventajas

- Cada sensor se mantiene y mejora por separado
- Reutilizable entre todos los ESP32 del proyecto
- Mas facil de testear cada modulo individualmente
- El nucleo no necesita conocer detalles internos de cada sensor

## Cuando implementar

Esperar a tener:
- Al menos un segundo ESP32 con sensores diferentes
- O un tercer sensor que justifique la reorganizacion

Hasta entonces, mantener el codigo actual (monolitico) que ya funciona.
