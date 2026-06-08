# Mejoras pendientes

## Energía
- Implementar deep sleep entre lecturas para ahorro de batería

## Firebase
- Agregar historial de lecturas además de la última

## Integración
- Conectar datos al agente-campo

## Hardware
- Agregar segundo sensor

## Visualización
- Dashboard simple para ver datos desde el celular

## Panel de control por consola
- Ver monitor serie
- Enviar reinicio al ESP32
- Ver estado de conexión WiFi
- Ver datos del sensor en tiempo real
- Ver estado de conexión Firebase
- Otras funcionalidades a futuro

## Interfaz web en el ESP32
- Panel de configuración accesible desde el navegador
- Reconfigurar red WiFi
- Reiniciar ESP32
- Ver estado del sensor
- Otras funcionalidades a futuro

## Nuevos sensores
- Incorporar sensor de presión barométrica

## App / Interfaz de monitoreo
- Crear aplicación o interfaz web para ver el estado de los sensores en tiempo real
- Permitir configuración de sensores desde la interfaz
- Evaluar si va en este repositorio, en agente-campo o en uno nuevo
- Conectar con datos de Firebase y con el agente

## Sensores futuros
- Presión barométrica
- Precipitaciones (pluviómetro) - muy importante
- Velocidad y dirección del viento
- Radiación UV

## OTA - Actualización inalámbrica del firmware
- Implementar OTA en el ESP32 para poder actualizar el código por WiFi
- Útil cuando el dispositivo está instalado en el campo sin acceso físico
- Opciones a evaluar:
  - OTA básico de Arduino (ArduinoOTA)
  - OTA desde la interfaz web del dispositivo
  - OTA desde el panel de control
