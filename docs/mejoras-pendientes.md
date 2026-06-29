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

## Historial unificado de sensores
- Crear historial combinado con datos de todos los sensores (DHT11 + BMP180)
- Incluir: temperatura, humedad, presion, altitud, timestamp
- Disponible en Firebase, bot y panel

## Calidad y robustez
- Validar lecturas del DHT11 (filtrar valores fuera de rango antes de enviar)
- Reintentar envio a Firebase 2-3 veces si falla
- Guardar lecturas en LittleFS cuando no hay WiFi y sincronizar al reconectar

## Generales del proyecto
- Backup automatico periodico de datos de Firebase a archivo
- Script de verificacion: comprobar que Firebase responde antes de cada sesion

## Configuracion centralizada de credenciales
- Las credenciales del ESP32 (API_KEY, DATABASE_URL, OTA_PASSWORD, etc)
  actualmente se guardan localmente en config.h y se pierden al cambiar equipo
- Evaluar opciones para centralizar la configuracion:
  - Repositorio privado solo para configs (config-agro)
  - Firebase Remote Config para leer credenciales desde la nube
  - Script que descargue config.h desde un servidor seguro al compilar
  - Variables de entorno en el sistema de compilacion
- Las redes WiFi (redes.json) tienen el mismo problema
- Objetivo: poder compilar y subir el firmware desde cualquier equipo
  sin tener que recrear archivos de configuracion manualmente

## Configuracion centralizada de credenciales
- Las credenciales del ESP32 (API_KEY, DATABASE_URL, OTA_PASSWORD, etc)
  actualmente se guardan localmente en config.h y se pierden al cambiar equipo
- Evaluar opciones para centralizar la configuracion:
  - Repositorio privado solo para configs (config-agro)
  - Firebase Remote Config para leer credenciales desde la nube
  - Script que descargue config.h desde un servidor seguro al compilar
  - Variables de entorno en el sistema de compilacion
- Las redes WiFi (redes.json) tienen el mismo problema
- Objetivo: poder compilar y subir el firmware desde cualquier equipo
  sin tener que recrear archivos de configuracion manualmente

## Configuracion centralizada de credenciales
- Las credenciales del ESP32 (API_KEY, DATABASE_URL, OTA_PASSWORD, etc)
  actualmente se guardan localmente en config.h y se pierden al cambiar equipo
- Evaluar opciones para centralizar la configuracion:
  - Repositorio privado solo para configs (config-agro)
  - Firebase Remote Config para leer credenciales desde la nube
  - Script que descargue config.h desde un servidor seguro al compilar
  - Variables de entorno en el sistema de compilacion
- Las redes WiFi (redes.json) tienen el mismo problema
- Objetivo: poder compilar y subir el firmware desde cualquier equipo
  sin tener que recrear archivos de configuracion manualmente
