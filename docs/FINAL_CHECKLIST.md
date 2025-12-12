# ✅ Lista de Verificación Final para Producción

Este checklist debe ser completado y verificado antes de la instalación final del nodo en Pasto. Asegura que la configuración del Backend, la red y el Hardware estén alineados.

---

## 1. ☁️ Configuración del Backend (Google Apps Script)

| Tarea | Estado | Notas |
| :--- | :--- | :--- |
| **Implementación Final** | [X] | La implementación del script ha sido actualizada a la **Última Versión** después de la corrección del método **`doPost`** (POST Cruda). |
| **Permisos de Acceso** | [X] | La implementación está configurada como **"Cualquier usuario" (Anyone)** para permitir el acceso anónimo del ESP32-CAM. |
| **ID de Hoja** | [X] | El `ID_HOJA` en el código GAS (`1tx92p6PDRQYddBpEeSTD_W8unKUvCXdkUY5eeAiJz1c`) coincide con la hoja de cálculo de destino. |
| **URL del Firmware** | [X] | El `GOOGLE_SCRIPT_URL` en el ESP32-CAM utiliza la URL de implementación **ACTIVA** y simplificada. |
| **Separación Fecha/Hora** | [X] | La lógica `doPost` separa `Fecha_Completa` en `Solo_Fecha` y `Solo_Hora` para el análisis de Looker Studio. |
| **Encabezados Sheets** | [X] | La pestaña `Sheet1` tiene los **16 encabezados** correctos en la Fila 1 (A-P). |

---

## 2. ⚡️ Configuración de Hardware y Estabilidad

| Tarea | Estado | Notas |
| :--- | :--- | :--- |
| **Fuente de Alimentación** | [X] | La ESP32-CAM se alimenta con una fuente **5V @ 2A (o superior)** para manejar los picos de consumo (WiFi/POST/Foto). |
| **Conexión Serial** | [X] | Cableado confirmado: Maestro GPIO 17 (TX2) a CAM GPIO 3 (RX0) y GND común.  |
| **Ciclo de POST** | [X] | Se confirmó que el ESP32-CAM publica de forma continua (sin caer en el error de las dos filas). |
| **Pinout de Sensores** | [X] | Cableado confirmado (DHT11 a GPIO 4, Suelo a GPIO 34). |

---

## 3. 🌐 Pruebas y Red (Pasto)

| Tarea | Estado | Notas |
| :--- | :--- | :--- |
| **Coordenadas** | [X] | Las coordenadas de Pasto (1.2136, -77.2811) están configuradas en el backend. |
| **Prueba Manual GAS** | [X] | La función `fetchApiDataOnly` se ejecutó con éxito, confirmando que la API y el guardado funcionan. |
| **Prueba de Producción** | [X] | Se verificó que el **Maestro** está publicando datos reales de forma continua. |

---

## 🎉 Proyecto Completado

Todo el sistema de adquisición de datos para Pasto está configurado, depurado y documentado.
