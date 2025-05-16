# Ecobot-Dashboard  
**Real-Time Sorting Dashboard for Your Trash-Sorting Robot**

A live-metrics pipeline streaming sorting events from a C++ publisher into a web dashboard for real-time visualization with bar and line charts.

---

## Table of Contents

1. [Architecture](#architecture)  
2. [Prerequisites](#prerequisites)  
3. [Repository Layout](#repository-layout)  
4. [Getting Started](#getting-started)  
   1. [Clone & Prepare](#clone--prepare)  
   2. [Build & Configure C++ Publisher](#build--configure-c-publisher)  
   3. [Configure & Run Dashboard](#configure--run-dashboard)  
5. [Usage](#usage)  
6. [Configuration Files](#configuration-files)  
7. [Troubleshooting](#troubleshooting)  
8. [License](#license)  

---

## Architecture

```text
┌───────────────────┐     HTTP POST     ┌───────────────────────────┐
│  C++ Publisher    │ ─────────────────>│  Node.js/Express Server  │
│  (publisher.exe)  │                   │  • POST /api/events      │
│  • Buffers events │                   │  • Stores in-memory      │
│  • Serializes to  │                   │  • GET /api/data returns │
│    JSON           │                   │    collected events      │
└───────────────────┘                   └───────────────────────────┘
                                               │
                                               │  Serves static UI
                                               ▼
                                         ┌─────────────────┐
                                         │ public/index.html │
                                         │ • Chart.js UI     │
                                         │ • Polls /api/data │
                                         └─────────────────┘
```

---

## Prerequisites

- **C++ Publisher**  
  - C++17-compatible compiler (g++, clang, or MSVC)  
  - CMake ≥ 3.10  
- **Web Dashboard**  
  - Node.js ≥ 14.x and npm  

---

## Repository Layout

```text
Ecobot-Dashboard/
├── .gitignore
├── README.md
├── publisher/                # C++ publisher
│   ├── src/
│   │   └── main.cpp
│   ├── CMakeLists.txt
│   └── config.json
└── dashboard/                # Node.js dashboard
    ├── index.js
    ├── package.json
    ├── .env.example
    └── public/
        └── index.html
```

---

## Getting Started

### Clone & Prepare

```bash
git clone https://github.com/Oleg-Niki/Ecobot-Dashboard.git
cd Ecobot-Dashboard
```

---

### Build & Configure C++ Publisher

```bash
cd publisher

# (Optional) Fetch nlohmann/json and cpp-httplib headers into third_party/
# Example:
# curl -L https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp \
#      -o third_party/json/include/nlohmann/json.hpp
# curl -L https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h \
#      -o third_party/httplib/include/httplib/httplib.h

mkdir build && cd build
cmake ..
cmake --build . --config Debug    # use `make` on Unix
```

**Result:**  
- Windows: `publisher/build/Debug/publisher.exe`  
- Unix:    `publisher/build/publisher`

Edit `publisher/config.json` if needed:

```json
{
  "api_url": "http://localhost:4000/api/events",
  "publish_interval_ms": 500
}
```

---

### Configure & Run Dashboard

```bash
cd ../../dashboard
npm install
cp .env.example .env   # on Windows: copy .env.example .env
```

Edit `.env` if you need to change:

```ini
PORT=4000
API_ROUTE=/api/events
```

Start the server:

```bash
npm start
```

You should see:

```
Dashboard listening at http://localhost:4000
```

---

## Usage

1. **Start the Dashboard**  
   ```bash
   cd dashboard
   npm start
   ```
2. **Run the Publisher**  
   ```bash
   cd ../publisher/build/Debug   # or build/ on Unix
   publisher.exe                 # or ./publisher
   ```
3. **View Live Metrics**  
   Open your browser at:  
   ```
   http://localhost:4000
   ```

---

## Configuration Files

- **publisher/config.json**  
  ```json
  {
    "api_url": "http://localhost:4000/api/events",
    "publish_interval_ms": 500
  }
  ```
- **dashboard/.env.example**  
  ```ini
  PORT=4000
  API_ROUTE=/api/events
  ```

---

## Troubleshooting

- **Publisher can’t find `config.json`**  
  Ensure you run the executable from the `publisher/` folder or copy `config.json` into your build output directory.

- **HTTP errors**  
  Verify `api_url` in `config.json` matches `PORT` and `API_ROUTE` in `.env`.

- **CMake errors**  
  Confirm CMake ≥ 3.10 and a C++17 compiler are installed.

- **Dashboard won’t start**  
  Rerun `npm install`, check Node.js version with `node -v`, and confirm you renamed `.env.example` to `.env`.

---

## License

This project is released under the **MIT License**. See [LICENSE](LICENSE) for details.

## Simple and short version README:
1. C++ Publisher
- Your robot (or a simulator) generates “sort events” (e.g. “I just sorted a plastic bottle, it took 42 ms”).

- The C++ program gathers those events, turns each one into a small JSON message, and sends it over HTTP to your dashboard server at regular intervals.

2. Node.js/Express Server

- Listens for incoming POSTs at /api/events.

- Every time it gets one of those JSON messages, it tucks it into an in-memory list.

- Also exposes a GET endpoint (/api/data) that returns the full list of events as JSON.

3. Browser Front-End

- You open your browser to the dashboard’s URL (e.g. http://localhost:4000).

- The page loads Chart.js and some JavaScript that every second calls /api/data.

- It takes the returned JSON array of events and updates two charts:

- A bar chart showing how many items of each category have been sorted so far.

- A line chart showing the sort latency (time) for each event in the order they arrived.