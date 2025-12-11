# 🌿 Proyecto IoT: Monitoreo Climático Híbrido

## 1. 💡 Visión General del Proyecto

Este proyecto implementa un sistema robusto de **integración y comparación de datos climáticos** para la ciudad de Pasto, Nariño. El objetivo principal es construir un **"Dataset"** que combine lecturas de sensores físicos (ESP32) con datos de APIs meteorológicas externas (OpenWeatherMap) y evidencia visual (ESP32-CAM) para su posterior análisis y creacion de un modelo de clasificación.

El sistema elimina la dependencia de un servidor local, utilizando una arquitectura Serverless gratuita basada en Google Apps Script (GAS) como backend.

## 2. 🧠 Arquitectura del Sistema (Flujo de Datos)

La arquitectura es de tipo "Maestro-Esclavo-Nube", garantizando la estabilidad y la no dependencia de servidores locales.

1.  **Nodo Maestro (ESP32 Sensores):** Lee **DHT22**, **DS18B20** y Humedad de Suelo. Sincroniza la hora por NTP.
2.  **Comunicación Serial (UART):** El Maestro envía un JSON con los datos y la orden de foto (cada 30 minutos) a la Cámara a través de pines TX/RX.
3.  **Nodo Esclavo (ESP32-CAM):** Recibe el JSON, toma la foto, la codifica en Base64.
4.  **Backend (Google Apps Script - GAS):**
    * Recibe el paquete JSON + Foto.
    * Consulta en tiempo real la **API de OpenWeatherMap** para Pasto, Nariño.
    * Fusiona los datos del Sensor + API en una sola fila.
    * Guarda los datos en **Google Sheets** y la foto en **Google Drive**.
5.  **Visualización:** Los datos se explotan en un Dashboard de **Google Looker Studio**.



## 3. 🗺️ Ubicación del Despliegue

| Componente | Fuente | Coordenadas |
| :--- | :--- | :--- |
| **Sensores Locales** | ESP32 | Fijas para Pasto, Nariño |
| **API Externa** | OpenWeatherMap | Latitud: 1.2136, Longitud: -77.2811 |

## 4. ⚙️ Componentes y Dependencias

### Hardware
* **Nodo Maestro:** ESP32 DevKit V1
* **Nodo Esclavo:** ESP32-CAM (AI-Thinker)
* **Sensores:** DHT22, DS18B20, Sensor de Humedad Capacitivo, RTC DS3231.

### Software y Servicios
* **Lenguajes:** C++ (Arduino IDE), JavaScript (Google Apps Script).
* **Backend:** Google Apps Script (GAS)
* **Almacenamiento:** Google Sheets (Datos Estructurados), Google Drive (Imágenes).
* **API Externa:** OpenWeatherMap (API Key requerida).
* **Visualización:** Google Looker Studio.

## 5. 🛠️ Guía de Puesta en Marcha

Para replicar este proyecto:

1.  **Clonar el Repositorio:** Descarga todos los archivos.
2.  **Configurar Backend:**
    * Crear la Hoja de Cálculo en Google Sheets.
    * Copiar el código de `backend_cloud/apps_script_code.js` al editor de Apps Script.
    * Obtener la **URL de Implementación** (Deployment URL).
    * **CRÍTICO:** Implementar con acceso **"Cualquier usuario"**.
3.  **Configurar Firmware:**
    * Abrir `firmware/esp32_cam_esclavo/esp32_cam_esclavo.ino`.
    * Reemplazar la variable `GOOGLE_SCRIPT_URL` con la URL obtenida en el paso anterior.
    * Cargar el código en ambos ESP32 (ver `docs/PINOUT_WIRING.md` para las conexiones Serial).
4.  **Verificación:** Monitorear en Google Sheets y en el Dashboard de Looker Studio.

## 6. 🔗 Documentación Adicional

| Documento | Contenido |
| :--- | :--- |
| `docs/PINOUT_WIRING.md` | Detalle de las conexiones físicas (TX/RX, GND, pines de sensores). |
| `backend_cloud/deployment_info.md` | IDs de la Hoja de Cálculo y URL de la API. |
| `data_analysis/` | Notebooks de Python para análisis de validación y ML inicial. |