require('dotenv').config();
console.log("Starting Dashboard…  PORT=", process.env.PORT);

const express = require('express');
const cors = require('cors');
const path = require('path');

const app = express();
const port = process.env.PORT || 4000;
const apiRoute = process.env.API_ROUTE || '/api/events';

console.log("apiRoute=", apiRoute);

// In-memory store for incoming events
let events = [];

app.use(cors());
app.use(express.json());
app.use(express.static(path.join(__dirname, 'public')));

// Receive events
app.post(apiRoute, (req, res) => {
    events.push(req.body);
    res.sendStatus(200);
});

// Expose stored events to the front-end
app.get('/api/data', (req, res) => {
    res.json(events);
});

app.listen(port, () => {
    console.log(`Dashboard listening at http://localhost:${port}`);
});