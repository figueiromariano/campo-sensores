/*
 * Proyecto: Agro - Campo Sensores
 * Sensor: DHT11 - Temperatura y Humedad
 * Descripción: Lee temperatura y humedad del sensor DHT11
 *              y envía los datos a Firebase Realtime Database.
 * Hardware: ESP32 WROOM-32 + DHT11 (3 pines)
 * Autor: figueiromariano
 * Repositorio: https://github.com/figueiromariano/campo-sensores
 */

#include <DHT.h>
#include "config.h"
#include "led_utils.h"
#include "wifi_manager.h"
#include "firebase_manager.h"
#include "web_server.h"
#include <ArduinoOTA.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>


// ─────────────────────────────────────────
// Modos de operación
#define MODO_ACTIVO 0
#define MODO_CAMPO  1
#define BOOT_PIN    0
#define VERSION "1.0"

int modoActual = MODO_ACTIVO;
unsigned long tiempoModoActivo = 0;

DHT dht(DHTPIN, DHT11);
Adafruit_BMP085 bmp;
bool bmp_disponible = false;

// ─────────────────────────────────────────
void entrarModoCampo() {
  Serial.println("Fin del modo activo. Pasando a modo campo...");
  publicarEstado("campo", VERSION);
  Serial.println("Entrando en modo campo (deep sleep)...");
  Serial.flush();
  ledApagar();
  esp_sleep_enable_timer_wakeup((uint64_t)TIEMPO_SLEEP * 1000000ULL);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);
  esp_deep_sleep_start();
}

// ─────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  ledSetup();
  pinMode(BOOT_PIN, INPUT_PULLUP);

  // Detectar causa del despertar
  esp_sleep_wakeup_cause_t causa = esp_sleep_get_wakeup_cause();
  if (causa == ESP_SLEEP_WAKEUP_EXT0) {
    Serial.println("Despertado por boton BOOT");
  } else if (causa == ESP_SLEEP_WAKEUP_TIMER) {
    Serial.println("Despertado por timer");
  } else {
    Serial.println("Encendido normal");
  }

  dht.begin();
  if (!conectarWiFi()) entrarModoCampo();
  sincronizarTiempo();
  if (!configurarFirebase()) entrarModoCampo();

// Inicializar BMP180
  Wire.begin(21, 22);
  if (bmp.begin()) {
    bmp_disponible = true;
    Serial.println("BMP180 inicializado OK");
  } else {
    bmp_disponible = false;
    Serial.println("BMP180 no encontrado");
  }

  delay(2000);
  leerYEnviarDatos(dht);

  // Publicar estado en Firebase
  publicarEstado("activo", VERSION);
  publicarSistema();

  // Iniciar modo activo
  tiempoModoActivo = millis();
  modoActual = MODO_ACTIVO;
  Serial.println("Modo activo iniciado. Duracion: 30 minutos.");
  Serial.print("Servidor web en: http://");
  Serial.println(WiFi.localIP());

  configurarServidorWeb(dht);
}

void loop() {
  // Verificar si hay que pasar a modo campo
  if (millis() - tiempoModoActivo >= DURACION_MODO_ACTIVO) {
    Serial.println("Fin del modo activo. Pasando a modo campo...");
    entrarModoCampo();
  }

  // Lectura cada INTERVALO_LECTURA
  if (millis() - ultimaLectura >= INTERVALO_LECTURA) {
    ultimaLectura = millis();
    leerYEnviarDatos(dht);

    // Leer BMP180 si esta disponible
    if (bmp_disponible) {
      leerYEnviarBMP180(bmp);
    }
  }

  // Atender peticiones web y OTA
  manejarServidorWeb();
  ArduinoOTA.handle();
}
