#!/bin/bash
# ─────────────────────────────────────────
# install.sh
# Instalacion del entorno para campo-sensores
# ─────────────────────────────────────────

echo "Instalando arduino-cli..."
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
sudo mv ~/campo-sensores/bin/arduino-cli /usr/local/bin/ 2>/dev/null || \
sudo mv ~/bin/arduino-cli /usr/local/bin/ 2>/dev/null || true

echo "Configurando arduino-cli..."
arduino-cli config init
arduino-cli config add board_manager.additional_urls \
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
arduino-cli core update-index
arduino-cli core install esp32:esp32@2.0.17

echo "Instalando librerias..."
arduino-cli lib install "Firebase ESP32 Client@4.3.15"
arduino-cli lib install "DHT sensor library"
arduino-cli lib install "ArduinoJson"

echo "Instalando esptool..."
pip3 install esptool --break-system-packages

echo ""
echo "Configuracion pendiente:"
echo "  1. Copiar config.h.ejemplo a dht11_firebase/config.h"
echo "  2. Completar API_KEY y DATABASE_URL en config.h"
echo "  3. Crear data/redes.json con las redes WiFi"
echo ""
echo "Compilar:"
echo "  arduino-cli compile --fqbn esp32:esp32:esp32 sensores/dht11-temperatura-humedad/codigo/dht11_firebase/"
echo ""
echo "Subir firmware:"
echo "  arduino-cli upload --fqbn esp32:esp32:esp32 --port /dev/ttyACM0 sensores/dht11-temperatura-humedad/codigo/dht11_firebase/"
echo ""
echo "Subir filesystem:"
echo "  ~/.arduino15/packages/esp32/tools/mklittlefs/3.0.0-gnu12-dc7f933/mklittlefs -c sensores/dht11-temperatura-humedad/codigo/dht11_firebase/data -s 0x160000 /tmp/littlefs.bin"
echo "  python3 -m esptool --chip esp32 --port /dev/ttyACM0 --baud 115200 write-flash 0x290000 /tmp/littlefs.bin"
echo ""
echo "Listo!"
