/*
 * wifi_manager.h
 * Manejo de conexión WiFi y carga de redes desde LittleFS
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "led_utils.h"
#include "config.h"

struct RedWifi {
  String ssid;
  String password;
  int prioridad;
};

RedWifi redes[10];
int cantidadRedes = 0;

// ─────────────────────────────────────────
void cargarRedes() {
  if (!LittleFS.begin(true)) {
    Serial.println("Error: no se pudo iniciar LittleFS");
    return;
  }
  File archivo = LittleFS.open("/redes.json", "r");
  if (!archivo) {
    Serial.println("Error: no se encontro redes.json");
    return;
  }
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, archivo);
  archivo.close();
  if (error) {
    Serial.println("Error: JSON invalido en redes.json");
    return;
  }
  JsonArray arr = doc["redes"].as<JsonArray>();
  cantidadRedes = 0;
  for (JsonObject red : arr) {
    redes[cantidadRedes].ssid      = red["ssid"].as<String>();
    redes[cantidadRedes].password  = red["password"].as<String>();
    redes[cantidadRedes].prioridad = red["prioridad"].as<int>();
    cantidadRedes++;
    if (cantidadRedes >= 10) break;
  }
  Serial.print("Redes cargadas: ");
  Serial.println(cantidadRedes);
}

// ─────────────────────────────────────────
bool intentarConexion(String ssid, String password) {
  Serial.print("Intentando: ");
  Serial.println(ssid);
  WiFi.disconnect();
  WiFi.begin(ssid.c_str(), password.c_str());
  unsigned long inicio = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - inicio > TIMEOUT_WIFI) {
      Serial.println("Timeout");
      return false;
    }
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado. IP: ");
  Serial.println(WiFi.localIP());
  return true;
}

// ─────────────────────────────────────────
bool conectarWiFi() {
  cargarRedes();
  if (cantidadRedes == 0) {
    Serial.println("Error: sin redes configuradas");
    ledParpadeo(5, 100);
    return false;
  }
  for (int i = 0; i < cantidadRedes; i++) {
    if (intentarConexion(redes[i].ssid, redes[i].password)) return true;
  }
  Serial.println("Error: no se pudo conectar a ninguna red");
  ledParpadeo(5, 100);
  return false;
}

#endif
