# Sensor DHT11 - Temperatura y Humedad

## Descripcion

Sensor de temperatura y humedad ambiente. Utilizado para monitorear
las condiciones climaticas en el campo y enviar los datos a Firebase
Realtime Database.

## Hardware necesario

| Componente | Descripcion |
|------------|-------------|
| ESP32 WROOM-32 | Microcontrolador con WiFi integrado |
| DHT11 (3 pines) | Sensor de temperatura y humedad |
| Cables Dupont | Conexion entre componentes |

## Conexion

| DHT11 | ESP32 |
|-------|-------|
| +     | 3.3V  |
| -     | GND   |
| S     | GPIO4 |

## Especificaciones del DHT11

| Parametro | Valor |
|-----------|-------|
| Temperatura | 0C a 50C |
| Precision temperatura | +/-2C |
| Humedad | 20% a 80% |
| Precision humedad | +/-5% |
| Intervalo de lectura | minimo 2 segundos |

## Dependencias

- Libreria DHT sensor library by Adafruit
- Libreria Firebase ESP32 Client v4.3.15 by Mobizt
- Libreria ArduinoJson by Benoit Blanchon
- ESP32 Arduino core 2.0.17

## Modos de operacion

Con deep sleep implementado el ESP32 opera en dos modos:

### Modo activo (primeros 30 minutos)
- Lee y envia datos cada 5 minutos
- Servidor web disponible en http://IP_DEL_DISPOSITIVO
- OTA disponible para actualizacion inalambrica
- Se activa tambien al presionar boton BOOT durante deep sleep

### Modo campo (deep sleep)
- Ciclo de 5 minutos: 19 segundos activo, 287 segundos durmiendo
- 96% del tiempo en sleep para ahorro de bateria
- Reinicio completo en cada ciclo evita procesos colgados

## Timeouts

| Proceso | Tiempo maximo |
|---------|--------------|
| Conexion WiFi | 20 segundos |
| Sincronizacion NTP | 15 segundos |

## Redes WiFi

Las redes se configuran en data/redes.json sin necesidad de recompilar.
Se puede editar desde la interfaz web del dispositivo.

## OTA - Actualizacion inalambrica

Durante el modo activo el dispositivo acepta actualizaciones OTA.

Compilar y subir por OTA:

    arduino-cli compile --fqbn esp32:esp32:esp32 sensores/dht11-temperatura-humedad/codigo/dht11_firebase/
    arduino-cli upload --fqbn esp32:esp32:esp32 --port IP_DISPOSITIVO --protocol network --upload-field password=PASSWORD sensores/dht11-temperatura-humedad/codigo/dht11_firebase/

## LED onboard (GPIO2)

| Estado | Senal | Descripcion |
|--------|-------|-------------|
| Todo OK | 3 destellos cortos (150ms) | Lectura y envio exitosos |
| Error sensor | 2 destellos largos (800ms) | Lectura invalida del DHT11 |
| Error Firebase | 5 destellos rapidos (100ms) | Fallo al enviar datos |
| OTA iniciando | 3 destellos rapidos | Actualizacion comenzando |
| OTA completado | 5 destellos rapidos | Actualizacion exitosa |
| Deep sleep | LED apagado | ESP32 en modo de bajo consumo |

## Diagnostico rapido en el campo

- 3 destellos cortos y apagado: todo funcionando correctamente
- 2 destellos largos: revisar conexion del sensor DHT11
- 5 destellos rapidos: revisar WiFi o Firebase
- LED apagado: en sleep o sin energia

## Datos publicados en Firebase

    dispositivos/
      dht11_esp32/
        estado/
          nombre, ip, red, modo, ultimo_arranque, ultimo_cambio, version
        sistema/
          temp_chip, ram_libre, ram_total, rssi, uptime, causa_reset, mac
    sensores/
      dht11/
        ultima_lectura/
          temperatura, humedad, unidad_temp, timestamp
        historial/
          FECHA_HORA/
            temperatura, humedad, unidad_temp

## Archivos

| Archivo | Descripcion |
|---------|-------------|
| codigo/dht11_firebase.ino | Codigo principal |
| codigo/config.h.ejemplo | Plantilla de configuracion |
| codigo/dht11_firebase/config.h | Configuracion real (no en git) |
| codigo/dht11_firebase/data/redes.json | Redes WiFi (no en git) |
| esquema/conexion.svg | Diagrama de conexion |

## Instalacion

Ver install.sh en la raiz del repositorio.

## Estado

Funcionando con OTA habilitado
