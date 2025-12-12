# 🌿 Modelo de Detección de Patologías Vegetales (CNN)

Esta carpeta contiene el modelo de Red Neuronal Convolucional (CNN) entrenado para la clasificación de patologías y estados de salud en plantas (Dataset PlantVillage). El modelo es el componente central de visión artificial del Sistema Experto IoT.

---

## 1. Archivos Contenidos

| Archivo | Tipo | Propósito Principal |
| :--- | :--- | :--- |
| `modelo_plantas_final.keras` | Modelo Binario | **Artefacto principal.** Es el modelo entrenado con la mejor precisión (87-89% global). Utilizado para la inferencia en el nodo IoT. |
| `inferencia_modelo.py` | Script Python | Código ligero para cargar el modelo (`.keras`) y ejecutar una predicción. Útil para probar la funcionalidad rápidamente en un entorno de producción. |
| `requirements.txt` | Configuración | Lista de las versiones exactas de librerías utilizadas para el desarrollo (TensorFlow, NumPy, etc.). |
| `matriz_confusion_[...].png` | Imagen (PNG) | Evidencia visual de la evaluación del modelo, demostrando su rendimiento en cada clase. |

---

## 2. Arquitectura del Modelo

El modelo es una Red Neuronal Convolucional (CNN) secuencial de tres bloques convolucionales, diseñada para el procesamiento de imágenes de 224x224 píxeles.

* **Tipo:** Red Neuronal Convolucional (CNN)
* **Capas Clave:** `Rescaling`, `RandomFlip`, 3x `Conv2D` (32, 64, 64 filtros), `Flatten`, `Dense` (64 neuronas), y `Softmax` de salida.
* **Precisión de Validación:** Aproximadamente **87%**.
* **Formato:** Keras (TensorFlow 2.x).

---

## 3. Uso y Reproducción

### 3.1. Instalación del Entorno

Para cargar y utilizar este modelo, es necesario recrear el entorno virtual.

1.  Cree el entorno virtual (si no existe):
    ```bash
    python -m venv venv
    ```
2.  Active el entorno:
    ```bash
    .\venv\Scripts\activate  # En Windows (PowerShell/CMD)
    ```
3.  Instale las dependencias:
    ```bash
    pip install -r requirements.txt
    ```

### 3.2. Carga y Prueba del Modelo

El modelo se carga mediante la función `load_model` de TensorFlow/Keras.

```python
import tensorflow as tf
import numpy as np

# Ruta al modelo guardado
MODEL_PATH = "modelo_plantas_final.keras"

# Cargar el modelo en memoria
try:
    modelo_cargado = tf.keras.models.load_model(MODEL_PATH)
    print("Modelo cargado exitosamente.")
    
    # Ejemplo de uso: La función de predicción espera un tensor 4D (lote, 224, 224, 3).
    # pred_result = modelo_cargado.predict(input_tensor_ejemplo)
    
except Exception as e:
    print(f"Error al cargar el modelo: {e}")


