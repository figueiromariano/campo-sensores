/*
 * Proyecto: Agro - Campo Sensores
 * Sensor: DHT11 - Temperatura y Humedad
 * Descripción: Lee temperatura y humedad del sensor DHT11
 *              y envía los datos a Firebase Firestore.
 * Hardware: ESP32 WROOM-32 + DHT11 (3 pines)
 * Autor: figueiromariano
 * Repositorio: https://github.com/figueiromariano/campo-sensores
 */

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include "config.h"

// Objetos Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

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

  // Configurar Firebase
  configurarFirebase();
}

// ─────────────────────────────────────────
void loop() {
  unsigned long ahora = millis();

  // Leer y enviar cada INTERVALO_LECTURA milisegundos
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
  config.api_key = API_KEY;
  config.token_status_callback = tokenStatusCallback;

  // Autenticación anónima
  auth.user.email = "";
  auth.user.password = "";

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Firebase configurado");
}

// ─────────────────────────────────────────
void leerYEnviarDatos() {
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();

  // Verificar lectura válida
  if (isnan(temperatura) || isnan(humedad)) {
    Serial.println("Error: lectura inválida del sensor");
    return;
  }

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.println(" %");

  // Enviar a Firestore
  enviarAFirestore(temperatura, humedad);
}

// ─────────────────────────────────────────
void enviarAFirestore(float temperatura, float humedad) {
  FirebaseJson content;
  String documentPath = "sensores/dht11/lecturas/" + String(millis());

  content.set("fields/temperatura/doubleValue", temperatura);
  content.set("fields/humedad/doubleValue", humedad);
  content.set("fields/timestamp/stringValue", String(millis()));

  if (Firebase.Firestore.createDocument(
        &fbdo,
        PROJECT_ID,
        "",
        documentPath.c_str(),
        content.raw())) {
    Serial.println("Datos enviados a Firebase OK");
  } else {
    Serial.print("Error Firebase: ");
    Serial.println(fbdo.errorReason());
  }
}
