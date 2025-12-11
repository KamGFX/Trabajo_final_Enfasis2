# ⚙️ Información de Despliegue y Credenciales (Backend)

Este documento centraliza todas las variables de entorno, claves de acceso y URLs necesarias para que la arquitectura Serverless (Google Apps Script) funcione correctamente.

---

## 1. ☁️ Configuración de Google Apps Script (GAS)

La siguiente URL es el punto final HTTP al que la ESP32-CAM se conecta para enviar el JSON y la foto.

| Parámetro | Valor | Notas |
| :--- | :--- | :--- |
| **URL de Implementación (Deploy URL)** | `https://script.google.com/macros/s/AKfycbyyV9ssSRekLMhbwNKal6BVfquDHs9KL3IBu9UqAfuq2kJcidr7m-zvfVFHBbpKtIb98Q/exec` | URL pegada en `firmware/esp32_cam_esclavo.ino` (GOOGLE_SCRIPT_URL) |
| **ID de la Hoja de Cálculo** | `1j-cqReINek2gllWAopJKMUmSFpJiBqFM-dtAaAGLXrE` | ID usado en el archivo `apps_script_code.js` (ID_HOJA) |
| **ID de la Carpeta de Drive** | (Vacío) | Actualmente guarda en la raíz de Google Drive (ID_CARPETA) |
| **Acceso Requerido** | Cualquiera (Anyone) | La implementación del script DEBE estar configurada como "Cualquier usuario". |

---

## 2. 🌎 Configuración de APIs y Geografía

Los datos de la API se consultan basados en la ubicación del despliegue del hardware.

| Parámetro | Valor | Notas |
| :--- | :--- | :--- |
| **Ubicación Actual** | **Pasto, Nariño** | Este es el foco actual del proyecto. |
| **Latitud** | `1.2136` | Usada por el ESP32 Maestro (documentación) y GAS (consulta API). |
| **Longitud** | `-77.2811` | Usada por el ESP32 Maestro (documentación) y GAS (consulta API). |
| **API Key (OpenWeatherMap)** | `637b6c96194459c872185532208f47d1` | Clave utilizada en `apps_script_code.js` (API_KEY_OWM) |
| **Zona Horaria** | GMT-5 | Hora estándar de Colombia. |

---

## 3. 📶 Credenciales del Firmware (WiFi)

Estas son las credenciales cableadas en el firmware de ambos módulos (`esp32_maestro_sensores.ino` y `esp32_cam_esclavo.ino`).

| Parámetro | Valor | Notas |
| :--- | :--- | :--- |
| **SSID** | `PruebaESP` | Nombre de la red WiFi |
| **Password** | `ESP32EVEYCAM` | Contraseña de la red WiFi |

---

## 4. 🗃️ Estructura del Dataset

La hoja de cálculo tiene 16 columnas para asegurar la integridad de los datos brutos y enriquecidos, incluyendo la separación de fecha/hora para Looker Studio.

| Columnas | Tipo de Dato | Fuente | Propósito |
| :--- | :--- | :--- | :--- |
| **A-D** (Fecha, T.Sen, H.Sen, Suelo) | Fecha/Hora, Número | ESP32 Maestro | Datos del sensor local. |
| **E-F** (Link Foto, Nombre) | URL de Imagen, Texto | ESP32-CAM + Drive | Evidencia visual. |
| **G-N** (T.API...Lluvia 1h) | Número, Texto | OpenWeatherMap | Datos enriquecidos para ML. |
| **O-P** (Solo_Fecha, Solo_Hora) | Fecha, Hora | GAS (Calculado) | Optimización para el Dashboard. |