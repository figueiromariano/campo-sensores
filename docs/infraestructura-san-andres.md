# Infraestructura - San Andrés

## Distribución del espacio

Todos los puntos están distribuidos linealmente con energía eléctrica:

    [Casa] --60m-- [Galpón] --50m-- [Casa puestero] --60m-- [Molino/Motor]

### Puntos
- **Casa principal:** vivienda familiar
- **Galpón:** taller/depósito, tablero eléctrico principal
- **Casa puestero:** vivienda del encargado
- **Molino/Motor:** bomba de agua eléctrica, tanque australiano, torre molino con tanque de agua potable

## Conectividad Starlink

### Instalación actual
- **Antena:** instalada en techo del galpón (plena visual a satélites)
- **Alimentación:** cable desde tablero principal del galpón
- **Repetidor:** dentro de la casa del puestero

### Cobertura WiFi
| Punto | Señal | Notas |
|-------|-------|-------|
| Galpón | Excelente | Punto de instalación |
| Casa puestero | Muy buena | Con repetidor |
| Corrales/molino | Débil pero funciona | Límite de cobertura |
| Casa principal | Deficiente | Muchos árboles, distancia |

### Pendientes conectividad
- Protección eléctrica de la antena Starlink
- Router 3 (Standard 4, Mini) en camino para mejorar cobertura casa principal
- Cablear algunos puntos cuando llegue el router

### Red WiFi
- SSID: SanAndrés
- Router/repetidor ubicado en casa puestero

## Dispositivos

| Nombre | Descripción | Tailscale IP |
|--------|-------------|--------------|
| unlpam | PC desarrollo UNLPam | 100.118.185.59 |
| lenovo | Notebook campo Debian | 100.98.76.68 |
| sala | PC casa Windows | 100.70.27.105 |
| esp32-galpon | ESP32 DHT11 sensor | sin Tailscale |

## Hardware IoT

### esp32-galpon
- Sensor de temperatura y humedad DHT11
- Funciona bien dentro de la casa principal
- Mejor conectividad WiFi que la notebook
- Pendiente: instalación permanente en el galpón cuando esté seguro

### lenovo
- Bot Telegram funcionando correctamente
- Conectividad WiFi limitada por hardware antiguo
- Pendiente: instalación permanente cuando haya conectividad estable

## Observaciones de campo

- El ESP32 tiene mejor adaptador WiFi que la notebook Lenovo
- Los árboles alrededor de la casa del puestero impiden buena señal directa de satélite
- La ubicación en el galpón es la mejor para la antena por visual despejada
- Seguridad: el galpón no es seguro para dejar equipos sin supervisión por ahora

## Próximos pasos en campo

1. Instalar protección eléctrica en antena
2. Esperar llegada Router 3 para mejorar cobertura casa principal
3. Evaluar punto de instalación permanente y seguro para notebook y ESP32
4. Cuando haya instalación permanente: configurar todo para inicio automático
