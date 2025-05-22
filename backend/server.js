  const express = require('express');
const cors    = require('cors');
const path    = require('path');

const app = express();
const PORT = process.env.PORT || 3000;

app.use(cors());
app.use(express.json());

let locations = [];

// POST /api/locations
app.post('/api/locations', (req, res) => {
  const { deviceId, lat, lng, speed, alt, timestamp } = req.body;
  if (
    typeof lat       !== 'number' ||
    typeof lng       !== 'number' ||
    typeof speed     !== 'number' ||
    typeof alt       !== 'number' ||
    typeof timestamp !== 'string'
  ) {
    return res.status(400).json({ error: 'Payload inválido' });
  }
  const loc = { deviceId, lat, lng, speed, alt, timestamp };
  locations.push(loc);
  if (locations.length > 1000) locations.shift();
  return res.json({ status: 'ok', loc });
});

// GET /api/locations/latest — nunca devuelve 404
app.get('/api/locations/latest', (req, res) => {
  const last = locations[locations.length - 1];
  if (last) {
    return res.json(last);
  }
  return res.json({
    deviceId: null,
    lat: 0,
    lng: 0,
    speed: 0,
    alt: 0,
    timestamp: new Date().toISOString()
  });
});

// Sirve front-end
const frontendDir = path.join(__dirname, '../frontend');
app.use(express.static(frontendDir));

// Fallback para SPA
app.get(/.*/, (req, res) => {
  res.sendFile(path.join(frontendDir, 'index.html'));
});

app.listen(PORT, () => {
  console.log(`API y front en http://localhost:${PORT}`);
}); 