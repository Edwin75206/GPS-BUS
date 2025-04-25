const express = require('express');
const cors    = require('cors');

const app = express();
const PORT = process.env.PORT || 3000;

// Middleware
app.use(cors());
app.use(express.json());

// Store temporal (en producción, reemplaza por BD)
let locations = [];

// Recibe datos del ESP32
app.post('/api/locations', (req, res) => {
  const { deviceId, lat, lng, timestamp } = req.body;
  if (
    typeof lat !== 'number' ||
    typeof lng !== 'number'
  ) {
    return res.status(400).json({ error: 'Lat/lng inválidos' });
  }
  const loc = { deviceId, lat, lng, timestamp };
  locations.push(loc);
  // opcional: limitar solo al último
  if (locations.length > 1000) locations.shift();
  res.json({ status: 'ok', loc });
});

// Devuelve la última posición
app.get('/api/locations/latest', (_, res) => {
  if (!locations.length) {
    return res.status(404).json({ error: 'Sin datos aún' });
  }
  res.json(locations[locations.length - 1]);
});

// Sirve el front-end
app.use(express.static('../frontend'));

app.listen(PORT, () =>
  console.log(`API y front en http://localhost:${PORT}`)
);
