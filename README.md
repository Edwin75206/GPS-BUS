# Localizador de Autobús en Vivo

Este proyecto implementa un sistema completo para rastrear en tiempo real la ubicación de un autobús mediante un **ESP32 + GPS Neo-6M**, un **servidor Node.js/Express** y un **cliente web** con **Leaflet** y **Bootstrap**.

---

## 📦 Estructura del proyecto

```
mi-tracker-gps/
├── arduino/                # Firmware para ESP32
│   └── gps_sender.ino      # Sketch Arduino
├── backend/                # Servidor Node.js + Express
│   ├── server.js
│   ├── package.json
│   └── ...
└── frontend/               # Interfaz web estática
    ├── index.html
    ├── js/
    │   ├── api.js
    │   └── map.js
    ├── css/                # (opcional) estilos adicionales
    └── ...
```

---

## 🔧 Prerrequisitos

### 🌐 Obtener dirección IP local

Para que tu ESP32 envíe datos al servidor correcto, necesitas conocer la IP de tu PC en la red local:

**Windows**

1. Abre **Símbolo del sistema** (CMD) o PowerShell.
2. Ejecuta:

   ```bash
   ipconfig
   ```
3. Localiza la sección de tu adaptador **Wi-Fi** (o Ethernet) y copia la **Dirección IPv4** (p. ej. `192.168.1.105`).

**macOS**

1. Abre **Terminal** (⌘ + Espacio y escribe "Terminal").
2. Para obtener directamente la IP de Wi‑Fi, ejecuta:

   ```bash
   ipconfig getifaddr en0
   ```

   O bien usa:

   ```bash
   ifconfig
   ```

   y busca bajo `en0` la línea que empiece por `inet ` (p. ej. `inet 192.168.1.105`).
3. También puedes abrir **Preferencias del Sistema > Red**, seleccionar tu conexión activa y leer la **Dirección IP**.

## 🔧 Prerrequisitos

1. **Node.js** ≥ v14
2. **npm** (viene con Node.js)
3. **Arduino IDE** o **VSCode + PlatformIO**
4. **ESP32 Dev Module** con cable USB
5. **Módulo GPS Neo-6M** conectado a UART2 (TX2 = GPIO16 → RX GPS, RX2 = GPIO17 ← TX GPS)

---

## ⚙️ Configuración

### 1. Backend

1. Abre una terminal y ve a la carpeta `backend/`:

   ```bash
   cd backend
   ```
2. Instala dependencias:

   ```bash
   npm install
   ```
3. (Opcional) Abre `server.js` y ajusta el puerto si deseas usar otro distinto de `3000`.
4. Inicia el servidor:

   ```bash
   node server.js
   ```
5. Verás en consola:

   ```
   API y front en http://localhost:3000
   ```

> **Nota**: Asegúrate de que el puerto (`3000` por defecto) esté abierto en tu firewall.

### 2. Firmware ESP32

1. Abre el sketch `arduino/gps_sender.ino` en el IDE.
2. Modifica las siguientes líneas con tus datos:

   ```cpp
   const char* ssid     = "TU_SSID";
   const char* password = "TU_PASSWORD";
   // Debe ser la IP de tu PC en la LAN
   const char* endpoint = "http://192.168.X.Y:3000/api/locations";
   ```
3. Selecciona la placa **ESP32 Dev Module** y el **puerto COM** correcto.
4. Compila y sube el sketch.
5. Abre el **Monitor Serie** a **115200 bps** para ver debug y confirmaciones.

### 3. Cliente Web

1. No requiere pasos adicionales: el servidor Express sirve los archivos de `frontend/`.
2. Abre en el navegador:

   ```
   http://localhost:3000
   ```
3. Verás el mapa y el panel de datos, que se actualizan cada 5 s.

---

## 🚀 Uso

* Enciende el ESP32 (puede alimentarse con USB o fuente externa).
* Asegúrate de que esté conectado a la misma red Wi-Fi que tu PC.
* Inicia backend (`node server.js`).
* Abre el navegador en `http://localhost:3000`.
* El sistema mostrará la ubicación del autobús en tiempo real.

---

## 📝 Personalización

* **Frecuencia de actualización**: en `map.js` y en `gps_sender.ino` (variable `interval`, en ms).
* **Puerto del servidor**: cambia `PORT` en `server.js` y actualiza `endpoint` en el ESP32.
* **Estilos**: modifica `frontend/index.html` o agrega CSS en `frontend/css/`.

---
