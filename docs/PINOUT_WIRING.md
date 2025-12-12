# 🔌 Diagrama de Cableado y Pinout del Sistema

Este documento detalla la conexión física entre el módulo ESP32 Maestro (Sensores), el ESP32-CAM (Esclavo) y los periféricos (DHT11, Sensor de Suelo), utilizando la comunicación UART (Serial 2).

## 1. 🌐 Comunicación UART (Maestro a Esclavo)

La comunicación entre el ESP32 Maestro y el ESP32-CAM (Esclavo) se realiza a través de UART2 del Maestro.

| Módulo Maestro (TX/RX) | Pin Maestro (GPIO) | Pin Esclavo (GPIO) | Módulo Esclavo (CAM) | Propósito |
| :--- | :--- | :--- | :--- | :--- |
| **TX2** | **GPIO 17** | **RX0** | **GPIO 3** | Transmite la orden JSON (Datos/Foto) |
| **RX2** | **GPIO 16** | **TX0** | **GPIO 1** | (No usado en este proyecto, pero debe estar conectado) |
| **GND** | GND | GND | **GND** | Referencia Común |
| **VCC** | 5V / VIN | 5V / VIN | **5V** | Alimentación Compartida (Requiere fuente robusta) |

> **NOTA CRÍTICA:** Se recomienda encarecidamente alimentar el ESP32-CAM con una fuente externa robusta de 5V @ 2A para evitar reinicios por "brownout" durante el POST o la captura de fotos.



## 2. 🌡️ Conexión de Sensores (Módulo Maestro)

El ESP32 Maestro es el encargado de leer los datos ambientales y la humedad del suelo.

### A. Sensor DHT11 (Temperatura y Humedad Ambiental)

| Pin del Sensor | Pin Maestro (GPIO) | Propósito |
| :--- | :--- | :--- |
| **VCC** | 3.3V | Alimentación |
| **GND** | GND | Tierra |
| **DATA** | **GPIO 4** | Señal de datos del sensor |

### B. Sensor de Humedad del Suelo (Analógico Capacitivo)

| Pin del Sensor | Pin Maestro (GPIO) | Propósito |
| :--- | :--- | :--- |
| **VCC** | 3.3V | Alimentación |
| **GND** | GND | Tierra |
| **A0** | **GPIO 34 (ADC1_CH6)** | Entrada Analógica para medir la humedad |



## 3. 💡 Pines Auxiliares (ESP32-CAM)

Estos pines controlan funciones secundarias en la cámara.

| Pin Esclavo (CAM) | Propósito | Notas |
| :--- | :--- | :--- |
| **GPIO 33** | **LED Rojo** | Indicador de actividad (ON: Trabajando, OFF: Esperando) |
| **GPIO 4** | LED Blanco (Flash) | Usado para iluminación (Controlado por la librería de la cámara) |
