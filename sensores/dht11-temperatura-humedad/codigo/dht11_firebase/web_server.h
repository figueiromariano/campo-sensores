/*
 * web_server.h
 * Servidor web para configuración y monitoreo del sensor
 */

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <WebServer.h>
#include <LittleFS.h>
#include "firebase_manager.h"
#include "config.h"

WebServer server(80);
extern int modoActual;
extern unsigned long tiempoModoActivo;

// ─────────────────────────────────────────
String paginaBase(String contenido) {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>Agro - Sensor DHT11</title>";
  html += "<style>";
  html += "body{font-family:sans-serif;max-width:480px;margin:20px auto;padding:0 16px;}";
  html += "h1{font-size:20px;}";
  html += ".card{background:#f5f5f5;border-radius:8px;padding:16px;margin:12px 0;}";
  html += ".val{font-size:32px;font-weight:bold;color:#1a73e8;}";
  html += ".label{font-size:13px;color:#666;margin-bottom:4px;}";
  html += "button,input[type=submit]{background:#1a73e8;color:white;border:none;padding:10px 20px;";
  html += "border-radius:6px;font-size:15px;cursor:pointer;width:100%;margin:6px 0;}";
  html += ".warn{background:#e53935;}";
  html += "textarea{width:100%;height:200px;font-family:monospace;font-size:13px;box-sizing:border-box;}";
  html += "</style></head><body>";
  html += contenido;
  html += "</body></html>";
  return html;
}

// ─────────────────────────────────────────
void configurarServidorWeb(DHT &dht) {

  // Página principal
  server.on("/", [&dht]() {
    unsigned long tiempoRestante = 0;
    if (millis() < tiempoModoActivo + DURACION_MODO_ACTIVO) {
      tiempoRestante = (tiempoModoActivo + DURACION_MODO_ACTIVO - millis()) / 1000;
    }
    String contenido = "<h1>Agro - Sensor DHT11</h1>";
    contenido += "<meta http-equiv='refresh' content='30'>";
    contenido += "<div class='card'>";
    contenido += "<div class='label'>Temperatura</div>";
    contenido += "<div class='val'>" + String(ultimaTemperatura, 1) + " &deg;C</div>";
    contenido += "<div class='label' style='margin-top:12px'>Humedad</div>";
    contenido += "<div class='val'>" + String(ultimaHumedad, 0) + " %</div>";
    contenido += "<div class='label' style='margin-top:8px'>Ultima lectura: " + ultimoTimestamp + "</div>";
    contenido += "</div>";
    contenido += "<div class='card'>";
    contenido += "<div class='label'>Red: " + WiFi.SSID() + "</div>";
    contenido += "<div class='label'>IP: " + WiFi.localIP().toString() + "</div>";
    contenido += "<div class='label'>Modo activo por: " + String(tiempoRestante) + " seg mas</div>";
    contenido += "</div>";
    contenido += "<button onclick=\"location.href='/lectura'\">Tomar lectura ahora</button>";
    contenido += "<button onclick=\"location.href='/redes'\">Configurar redes WiFi</button>";
    contenido += "<button class='warn' onclick=\"location.href='/reiniciar'\">Reiniciar ESP32</button>";
    server.send(200, "text/html", paginaBase(contenido));
  });

  // Lectura manual
  server.on("/lectura", [&dht]() {
    leerYEnviarDatos(dht);
    server.sendHeader("Location", "/");
    server.send(303);
  });

  // Reiniciar
  server.on("/reiniciar", []() {
    server.send(200, "text/html", paginaBase("<h1>Reiniciando...</h1><p>El dispositivo se reiniciara en 2 segundos.</p>"));
    delay(2000);
    ESP.restart();
  });

  // Ver y editar redes
  server.on("/redes", []() {
    File archivo = LittleFS.open("/redes.json", "r");
    String contenido = "";
    if (archivo) {
      contenido = archivo.readString();
      archivo.close();
    }
    String html = "<h1>Redes WiFi</h1>";
    html += "<form action='/guardar_redes' method='POST'>";
    html += "<div class='label'>Editá el archivo redes.json:</div>";
    html += "<textarea name='json'>" + contenido + "</textarea><br>";
    html += "<input type='submit' value='Guardar y reiniciar'>";
    html += "</form>";
    html += "<button onclick=\"location.href='/'\">Volver</button>";
    server.send(200, "text/html", paginaBase(html));
  });

  // Guardar redes
  server.on("/guardar_redes", HTTP_POST, []() {
    if (server.hasArg("json")) {
      File archivo = LittleFS.open("/redes.json", "w");
      if (archivo) {
        archivo.print(server.arg("json"));
        archivo.close();
        server.send(200, "text/html", paginaBase("<h1>Guardado</h1><p>Reiniciando en 2 segundos...</p>"));
        delay(2000);
        ESP.restart();
      }
    }
    server.send(500, "text/plain", "Error al guardar");
  });

  server.begin();
}

// ─────────────────────────────────────────
void manejarServidorWeb() {
  server.handleClient();
}

#endif
