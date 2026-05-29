/*
 * Proyecto: Agro - Campo Sensores
 * Sensor: DHT11 - Temperatura y Humedad
 * Descripción: Lee temperatura y humedad del sensor DHT11
 *              y envía los datos a Firebase Realtime Database.
 * Hardware: ESP32 WROOM-32 + DHT11 (3 pines)
 * Autor: figueiromariano
 * Repositorio: https://github.com/figueiromariano/campo-sensores
 * Librería Firebase: FirebaseESP32 by Mobizt
 */

#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>
#include <time.h>
#include "config.h"

// LED onboard
#define LED_PIN 2

// Objetos Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config_fb;

// Objeto sensor
DHT dht(DHTPIN, DHT11);

// ─────────────────────────────────────────
void ledParpadeo(int veces, int duracion) {
  for (int i = 0; i < veces; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(duracion);
    digitalWrite(LED_PIN, LOW);
    delay(duracion);
  }
}

// ─────────────────────────────────────────
void entrarSleep() {
  Serial.println("Entrando en deep sleep...");
  Serial.flush();
  digitalWrite(LED_PIN, LOW);
  esp_sleep_enable_timer_wakeup((uint64_t)TIEMPO_SLEEP * 1000000ULL);
  esp_deep_sleep_start();
}

// ─────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  unsigned long tiempoInicio = millis();
  Serial.println("Iniciando sistema...");

  dht.begin();
  conectarWiFi();
  sincronizarTiempo();
  configurarFirebase();

  delay(2000);
  leerYEnviarDatos();

  Serial.print("Tiempo total de ciclo: ");
  Serial.print(millis() - tiempoInicio);
  Serial.println(" ms");

  entrarSleep();
}

// ─────────────────────────────────────────
void loop() {
  // No se usa con deep sleep
}

// ─────────────────────────────────────────
void conectarWiFi() {
  Serial.print("Conectando a WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long inicio = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - inicio > TIMEOUT_WIFI) {
      Serial.println("Error: timeout WiFi");
      ledParpadeo(5, 100);
      entrarSleep();
    }
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// ─────────────────────────────────────────
void sincronizarTiempo() {
  configTime(-3 * 3600, 0, "pool.ntp.org");
  Serial.print("Sincronizando tiempo");

  unsigned long inicio = millis();
  while (time(nullptr) < 1000000000) {
    if (millis() - inicio > TIMEOUT_NTP) {
      Serial.println("Error: timeout NTP");
      ledParpadeo(5, 100);
      entrarSleep();
    }
    delay(500);
    Serial.print(".");
  }
  Serial.println(" OK");
}

// ─────────────────────────────────────────
void configurarFirebase() {
  config_fb.api_key = API_KEY;
  config_fb.database_url = DATABASE_URL;

  if (Firebase.signUp(&config_fb, &auth, "", "")) {
    Serial.println("Firebase autenticado");
  } else {
    Serial.printf("Error auth: %s\n", config_fb.signer.signupError.message.c_str());
    ledParpadeo(5, 100);
    entrarSleep();
  }

  Firebase.begin(&config_fb, &auth);
  Firebase.reconnectWiFi(true);
}

// ─────────────────────────────────────────
String obtenerFecha() {
  time_t ahora = time(nullptr);
  struct tm* t = localtime(&ahora);
  char buffer[25];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);
  return String(buffer);
}

// ─────────────────────────────────────────
void leerYEnviarDatos() {
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();

  if (isnan(temperatura) || isnan(humedad)) {
    Serial.println("Error: lectura invalida del sensor");
    ledParpadeo(2, 800);
    return;
  }

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" C");
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.println(" %");

  enviarAFirebase(temperatura, humedad);
}

// ─────────────────────────────────────────
void enviarAFirebase(float temperatura, float humedad) {
  String fecha = obtenerFecha();
  String rutaUltima = "/sensores/dht11/ultima_lectura";
  String rutaHistorial = "/sensores/dht11/historial/" + fecha;

  // Reemplaza espacios y ":" por guiones para la ruta del historial
  rutaHistorial.replace(" ", "_");
  rutaHistorial.replace(":", "-");

  bool okUltima = Firebase.setFloat(fbdo, rutaUltima + "/temperatura", temperatura) &&
                  Firebase.setFloat(fbdo, rutaUltima + "/humedad", humedad) &&
                  Firebase.setString(fbdo, rutaUltima + "/unidad_temp", "C") &&
                  Firebase.setString(fbdo, rutaUltima + "/timestamp", fecha);

  bool okHistorial = Firebase.setFloat(fbdo, rutaHistorial + "/temperatura", temperatura) &&
                     Firebase.setFloat(fbdo, rutaHistorial + "/humedad", humedad) &&
                     Firebase.setString(fbdo, rutaHistorial + "/unidad_temp", "C");

  if (okUltima && okHistorial) {
    ledParpadeo(3, 150);
    Serial.println("Datos enviados a Firebase OK");
  } else {
    ledParpadeo(5, 100);
    Serial.print("Error Firebase: ");
    Serial.println(fbdo.errorReason());
  }
}
