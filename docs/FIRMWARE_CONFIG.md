# 💻 Configuración del Firmware y Constantes (ESP32 Maestro & Esclavo)

Este documento detalla todas las variables críticas y constantes definidas en los archivos de firmware (`.ino`) para garantizar la operación en Pasto, Nariño, y la comunicación con el Backend de Google Apps Script.

---

## 1. 📶 Credenciales y Red (Común)

Estas constantes se utilizan en ambos módulos (Maestro y Esclavo) para establecer la conexión a la red local.

| Archivo | Constante | Valor | Propósito |
| :--- | :--- | :--- | :--- |
| `esp32_cam_esclavo.ino` | `ssid` | `"PruebaESP"` | Nombre de la red Wi-Fi. |
| `esp32_cam_esclavo.ino` | `password` | `"ESP32EVEYCAM"` | Contraseña de la red Wi-Fi. |

---

## 2. ☁️ Configuración de la Nube (ESP32-CAM Esclavo)

La URL de destino es la variable más crítica en el firmware del Esclavo, ya que dirige la solicitud al Backend de Google Workspace.

| Archivo | Constante | Valor | Notas |
| :--- | :--- | :--- | :--- |
| `esp32_cam_esclavo.ino` | `GOOGLE_SCRIPT_URL` | `https://script.google.com/macros/s/AKfycbz8dW5pI6Xxen7XfGHDMRTlRAbE1Vm6q7vgnMwvan-39iH3zjnzLCSB6Wmm70bpBxxReg/exec` | **URL de Implementación Final.** Se ha simplificado (quitado el subdominio `/a/macros/unicauca.edu.co/`) para evitar el error `400 Bad Request`. |
| `esp32_cam_esclavo.ino` | `http.setTimeout` | `20000` (20 segundos) | Tiempo de espera extendido para compensar la latencia de la API de OpenWeatherMap y la escritura en Google Sheets. |

---

## 3. ⏱️ Frecuencias y Geografía (ESP32 Maestro)

Estas constantes controlan el ciclo de lectura y envío de datos.

| Archivo | Constante | Valor | Propósito |
| :--- | :--- | :--- | :--- |
| `esp32_maestro_sensores.ino` | `intervaloLectura` | `15000` (15 segundos) | Frecuencia con la que el Maestro lee los sensores y realiza el envío por Serial. |
| `esp32_maestro_sensores.ino` | `ciclosParaFoto` | `9999` (Ejemplo) | Frecuencia con la que el Maestro ordena a la CAM tomar una foto (Ej: cada 9999 ciclos para desactivar la foto, o cada 2 ciclos para prueba). |
| `esp32_maestro_sensores.ino` | `LATITUD` | `1.2136` | Usada por el Maestro para estampar el JSON (aunque el Backend es la fuente oficial). |
| `esp32_maestro_sensores.ino` | `LONGITUD` | `-77.2811` | Usada por el Maestro para estampar el JSON. |

---

## 4. 📌 Definición de Pines (ESP32 Maestro)

Estos pines deben coincidir con la documentación de cableado (`PINOUT_WIRING.md`).

| Pin Maestro | Función | Módulo |
| :--- | :--- | :--- |
| **GPIO 4** | `DHTPIN` | Sensor DHT11 |
| **GPIO 34** | `SUELO_PIN` | Sensor de Humedad (ADC) |
| **GPIO 17** | `TX2` | Envío de datos a ESP32-CAM |
| **GPIO 16** | `RX2` | Recepción de datos de ESP32-CAM |
