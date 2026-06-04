/*
 * firebase_manager.h
 * Manejo de autenticación y envío de datos a Firebase Realtime Database
 */

#ifndef FIREBASE_MANAGER_H
#define FIREBASE_MANAGER_H

#include <FirebaseESP32.h>
#include <time.h>
#include "led_utils.h"
#include "config.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config_fb;

unsigned long ultimaLectura = 0;

float ultimaTemperatura = 0;
float ultimaHumedad     = 0;
String ultimoTimestamp  = "";

// ─────────────────────────────────────────
String obtenerFecha() {
  time_t ahora = time(nullptr);
  struct tm* t = localtime(&ahora);
  char buffer[25];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);
  return String(buffer);
}

// ─────────────────────────────────────────
void sincronizarTiempo() {
  configTime(-3 * 3600, 0, "pool.ntp.org");
  Serial.print("Sincronizando tiempo");
  unsigned long inicio = millis();
  while (time(nullptr) < 1000000000) {
    if (millis() - inicio > TIMEOUT_NTP) {
      Serial.println(" Error: timeout NTP");
      ledParpadeo(5, 100);
      return;
    }
    delay(500);
    Serial.print(".");
  }
  Serial.println(" OK");
}

// ─────────────────────────────────────────
bool configurarFirebase() {
  config_fb.api_key      = API_KEY;
  config_fb.database_url = DATABASE_URL;
  if (Firebase.signUp(&config_fb, &auth, "", "")) {
    Serial.println("Firebase autenticado");
  } else {
    Serial.printf("Error auth: %s\n", config_fb.signer.signupError.message.c_str());
    ledParpadeo(5, 100);
    return false;
  }
  Firebase.begin(&config_fb, &auth);
  Firebase.reconnectWiFi(true);
  return true;
}

// ─────────────────────────────────────────
void enviarAFirebase(float temperatura, float humedad) {
  String fecha = obtenerFecha();
  String rutaUltima    = "/sensores/dht11/ultima_lectura";
  String rutaHistorial = "/sensores/dht11/historial/" + fecha;
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
    ultimoTimestamp = fecha;
    Serial.println("Datos enviados a Firebase OK");
  } else {
    ledParpadeo(5, 100);
    Serial.print("Error Firebase: ");
    Serial.println(fbdo.errorReason());
  }
}

// ─────────────────────────────────────────
void leerYEnviarDatos(DHT &dht) {
  float temperatura = dht.readTemperature();
  float humedad     = dht.readHumidity();

  if (isnan(temperatura) || isnan(humedad)) {
    Serial.println("Error: lectura invalida del sensor");
    ledParpadeo(2, 800);
    return;
  }

  ultimaTemperatura = temperatura;
  ultimaHumedad     = humedad;

  Serial.print("Temperatura: "); Serial.print(temperatura); Serial.println(" C");
  Serial.print("Humedad: ");     Serial.print(humedad);     Serial.println(" %");

  enviarAFirebase(temperatura, humedad);
}

// ─────────────────────────────────────────
void publicarEstado(String modo, String version) {
  String ruta = "/dispositivos/dht11_esp32/estado";

  Firebase.setString(fbdo, ruta + "/ip",              WiFi.localIP().toString());
  Firebase.setString(fbdo, ruta + "/red",             WiFi.SSID());
  Firebase.setString(fbdo, ruta + "/modo",            modo);
  Firebase.setString(fbdo, ruta + "/ultimo_arranque", obtenerFecha());
  Firebase.setString(fbdo, ruta + "/version",         version);
  Firebase.setString(fbdo, ruta + "/ultimo_cambio",   obtenerFecha());

  Serial.print("Estado publicado: ");
  Serial.println(modo);
}

#endif
