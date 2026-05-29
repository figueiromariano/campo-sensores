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


// ─────────────────────────────────────────
// Configuración LED onboard
#define LED_PIN 2

void ledParpadeo(int veces, int duracion) {
  for (int i = 0; i < veces; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(duracion);
    digitalWrite(LED_PIN, LOW);
    delay(duracion);
  }
}

void ledEstado(String estado) {
  if (estado == "iniciando") {
    // Parpadeo rápido continuo
    for (int i = 0; i < 10; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(100);
    }
  } else if (estado == "wifi") {
    // Parpadeo medio
    for (int i = 0; i < 4; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      digitalWrite(LED_PIN, LOW);
      delay(500);
    }
  } else if (estado == "tiempo") {
    // Parpadeo lento
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(1000);
      digitalWrite(LED_PIN, LOW);
      delay(1000);
    }
  } else if (estado == "firebase_ok") {
    // 3 destellos cortos
    ledParpadeo(3, 150);
  } else if (estado == "ok") {
    // LED encendido 2 segundos luego apaga
    digitalWrite(LED_PIN, HIGH);
    delay(2000);
    digitalWrite(LED_PIN, LOW);
  } else if (estado == "error_sensor") {
    // 2 destellos largos
    ledParpadeo(2, 800);
  } else if (estado == "error_firebase") {
    // 5 destellos rápidos
    ledParpadeo(5, 100);
  } else if (estado == "sleep") {
    // LED apagado
    digitalWrite(LED_PIN, LOW);
  }
}


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

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  ledEstado("iniciando");

  unsigned long tiempoInicio = millis();

  Serial.println("Iniciando sistema...");

  // Iniciar sensor
  dht.begin();

  // Conectar WiFi
  conectarWiFi();

  // Sincroniza tiempo
  sincronizarTiempo();

  // Configurar Firebase
  configurarFirebase();

  // Espera a que el sensor se estabilice
  delay(2000);

  // Primera lectura inmediata al iniciar
  leerYEnviarDatos();

  ledEstado("ok");

  Serial.print("Tiempo total de inicio: ");
  Serial.print(millis() - tiempoInicio);
  Serial.println(" ms");
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
  ledEstado("wifi");
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
    ledEstado("error_sensor");
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
    ledEstado("firebase_ok");
    Serial.println("Datos enviados a Firebase OK");
  } else {
    ledEstado("error_firebase");
    Serial.print("Error Firebase: ");
    Serial.println(fbdo.errorReason());
  }
}

void sincronizarTiempo() {
  ledEstado("tiempo");
  configTime(-3 * 3600, 0, "pool.ntp.org");
  Serial.print("Sincronizando tiempo");
  while (time(nullptr) < 1000000000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" OK");
}
