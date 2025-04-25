import { fetchLatest } from './api.js';
import L from 'https://unpkg.com/leaflet/dist/leaflet-src.esm.js';

let map, marker;

async function initMap() {
  try {
    const { lat, lng } = await fetchLatest();
    map = L.map('map').setView([lat, lng], 13);
    L.tileLayer(
      'https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png'
    ).addTo(map);
    marker = L.marker([lat, lng]).addTo(map);
  } catch (e) {
    console.warn(e);
    return;
  }

  // Polling cada 5 s
  setInterval(async () => {
    try {
      const { lat, lng } = await fetchLatest();
      marker.setLatLng([lat, lng]);
      map.setView([lat, lng]);
    } catch (e) {
      console.warn('Error al refrescar:', e);
    }
  }, 5000);
}

initMap();
