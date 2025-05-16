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
## Prerequisites
C++ Publisher

C++17-compatible compiler (g++, clang, or MSVC)

CMake ≥ 3.10

Web Dashboard

Node.js ≥ 14.x and npm

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

## Getting Started
Clone & Prepare
```bash
git clone https://github.com/Oleg-Niki/Ecobot-Dashboard.git
cd Ecobot-Dashboard
```

Build & Configure C++ Publisher
```bash
cd publisher

# (Optional) Fetch nlohmann/json and cpp-httplib headers into third_party/
# Example:
# curl -L https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp \\
#      -o third_party/json/include/nlohmann/json.hpp
# curl -L https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h \\
#      -o third_party/httplib/include/httplib/httplib.h

mkdir build && cd build
cmake ..
cmake --build . --config Debug    # use `make` on Unix
```

Result:

Windows: publisher/build/Debug/publisher.exe

Unix: publisher/build/publisher

Edit publisher/config.json if needed:
``` json
{
  "api_url": "http://localhost:4000/api/events",
  "publish_interval_ms": 500
}
```

