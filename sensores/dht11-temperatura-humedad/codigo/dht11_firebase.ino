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
#include "config.h"
#include <time.h>

// Objetos Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config_fb;

// Objeto sensor
DHT dht(DHTPIN, DHT11);

// Variables de control de tiempo
unsigned long ultimaLectura = 0;

// ─────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando sistema...");

  // Iniciar sensor
  dht.begin();

  // Conectar WiFi
  conectarWiFi();

  // Sincroniza tiempo
  sincronizarTiempo();

  // Configurar Firebase
  configurarFirebase();

  // Primera lectura inmediata al iniciar
  leerYEnviarDatos();
}

// ─────────────────────────────────────────
void loop() {
  unsigned long ahora = millis();

  if (ahora - ultimaLectura >= INTERVALO_LECTURA) {
    ultimaLectura = ahora;
    leerYEnviarDatos();
  }
}

// ─────────────────────────────────────────
void conectarWiFi() {
  Serial.print("Conectando a WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// ─────────────────────────────────────────
void configurarFirebase() {
  config_fb.api_key = API_KEY;
  config_fb.database_url = DATABASE_URL;

  // Autenticación anónima
  if (Firebase.signUp(&config_fb, &auth, "", "")) {
    Serial.println("Firebase autenticado");
  } else {
    Serial.printf("Error auth: %s\n", config_fb.signer.signupError.message.c_str());
  }

  Firebase.begin(&config_fb, &auth);
  Firebase.reconnectWiFi(true);
}

// ─────────────────────────────────────────
void leerYEnviarDatos() {
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();

  if (isnan(temperatura) || isnan(humedad)) {
    Serial.println("Error: lectura invalida del sensor");
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
String obtenerFecha() {
  time_t ahora = time(nullptr);
  struct tm* t = localtime(&ahora);
  char buffer[25];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);
  return String(buffer);
}



// ─────────────────────────────────────────
void enviarAFirebase(float temperatura, float humedad) {
  String ruta = "/sensores/dht11/ultima_lectura";

  if (Firebase.setFloat(fbdo, ruta + "/temperatura", temperatura) &&
      Firebase.setFloat(fbdo, ruta + "/humedad", humedad) &&
      Firebase.setString(fbdo, ruta + "/unidad_temp", "C") &&
      Firebase.setString(fbdo, ruta + "/timestamp", obtenerFecha())) {
    Serial.println("Datos enviados a Firebase OK");
  } else {
    Serial.print("Error Firebase: ");
    Serial.println(fbdo.errorReason());
  }
}

void sincronizarTiempo() {
  configTime(-3 * 3600, 0, "pool.ntp.org");
  Serial.print("Sincronizando tiempo");
  while (time(nullptr) < 1000000000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" OK");
}
