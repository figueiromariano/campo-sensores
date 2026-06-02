/*
 * led_utils.h
 * Funciones para el control del LED onboard
 * LED azul en GPIO2
 */

#ifndef LED_UTILS_H
#define LED_UTILS_H

#define LED_PIN 2

void ledSetup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void ledParpadeo(int veces, int duracion) {
  for (int i = 0; i < veces; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(duracion);
    digitalWrite(LED_PIN, LOW);
    delay(duracion);
  }
}

void ledApagar() {
  digitalWrite(LED_PIN, LOW);
}

#endif
