require('dotenv').config();
console.log("Starting Dashboard…  PORT=", process.env.PORT);

const express = require('express');
const cors = require('cors');
const path = require('path');
const { spawn } = require('child_process');

const app = express();
const port = process.env.PORT || 4000;
const apiRoute = process.env.API_ROUTE || '/api/events';

console.log("apiRoute=", apiRoute);

app.use(express.static(path.join(__dirname, 'public')));
app.get('/categories.json', (req, res) => {
    res.sendFile(path.join(__dirname, '..', 'categories.json'));
});


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

// Endpoint to launch the C++ publisher
app.post('/api/run-publisher', (req, res) => {
    // Path to the built executable
    const exePath = path.join(
        __dirname, '..', 'publisher', 'build', 'Debug', 'publisher.exe'
    );

    // Launch with working directory = exe’s folder
    const child = spawn(exePath, [], {
        cwd: path.dirname(exePath),
        stdio: 'inherit'
    });

    child.on('error', err => {
        console.error('Failed to start publisher:', err);
        res.status(500).json({ success: false, error: err.message });
    });

    child.on('exit', code => {
        if (code === 0) {
            res.json({ success: true });
        } else {
            console.error(`Publisher exited with code ${code}`);
            res.status(500).json({ success: false, code });
        }
    });
});

app.listen(port, () => {
    console.log(`Dashboard listening at http://localhost:${port}`);
});