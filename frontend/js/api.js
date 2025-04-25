export async function fetchLatest() {
    const res = await fetch('/api/locations/latest');
    if (!res.ok) throw new Error('No hay datos aún');
    return await res.json();  // { lat, lng, timestamp, deviceId }
  }
  