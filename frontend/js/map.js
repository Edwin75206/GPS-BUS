import { fetchLatest } from './api.js';

let map, marker;

// Función para volcar datos en pantalla
function updateInfo({ lat, lng, speed, alt, timestamp }) {
  document.getElementById('lat').innerText       = lat.toFixed(6);
  document.getElementById('lng').innerText       = lng.toFixed(6);
  document.getElementById('speed').innerText     = speed.toFixed(1);
  document.getElementById('alt').innerText       = alt.toFixed(1);
  document.getElementById('timestamp').innerText = timestamp;
}

async function initMap() {
  try {
    const data = await fetchLatest();

    // Inicializa el mapa
    map = L.map('map').setView([data.lat, data.lng], 13);
    L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png').addTo(map);
    marker = L.marker([data.lat, data.lng]).addTo(map);

    // Muestra los datos iniciales
    updateInfo(data);

    // Polling cada 5 s
    setInterval(async () => {
      try {
        const d = await fetchLatest();
        marker.setLatLng([d.lat, d.lng]);
        map.setView([d.lat, d.lng]);
        updateInfo(d);
      } catch (err) {
        console.warn('Error al refrescar:', err);
      }
    }, 5000);

  } catch (err) {
    console.error('No se pudo inicializar mapa:', err);
  }
}

initMap(); 