# Ecobot-Dashboard
Real‑Time Sorting Trashrobot Dashboard
# Real‑Time Sorting Dashboard

A step‑by‑step guide to setting up a live metrics dashboard for your trash‑sorting robot project, combining a C++ data publisher and a web‑based dashboard.

## Table of Contents

1. [Overview](#overview)
2. [Prerequisites](#prerequisites)
3. [Step 1: Clone the Repository](#step-1-clone-the-repository)
4. [Step 2: Install Dependencies](#step-2-install-dependencies)
5. [Step 3: Build the C++ Data Publisher](#step-3-build-the-c-data-publisher)
6. [Step 4: Configure the Publisher](#step-4-configure-the-publisher)
7. [Step 5: Set Up the Web Dashboard](#step-5-set-up-the-web-dashboard)
8. [Step 6: Run the System](#step-6-run-the-system)
9. [Step 7: Viewing Live Metrics](#step-7-viewing-live-metrics)
10. [Troubleshooting](#troubleshooting)
11. [License](#license)

---

## Overview

This project streams real‑time sorting statistics (items sorted per category, latency, etc.) from your robot’s C++ application to a web dashboard for live visualization. You’ll learn how to:

* Buffer and publish events in C++
* Expose a REST endpoint for metric ingestion
* Build a simple Node.js/Express dashboard with Chart.js visualizations

---

## Prerequisites

* **Hardware:** Trash‑sorting robot with network connectivity (e.g., Raspberry Pi or onboard PC)
* **Software:**

  * C++17-compatible compiler (GCC/Clang)
  * CMake (>=3.10)
  * Node.js (>=14.x) & npm

---

## Step 1: Clone the Repository

```bash
git clone https://github.com/yourusername/trash-sorting-dashboard.git
cd trash-sorting-dashboard
```

---

## Step 2: Install Dependencies

### 2.1 C++ Dependencies

* [nlohmann/json](https://github.com/nlohmann/json)
* [cpp-httplib](https://github.com/yhirose/cpp-httplib)

You can fetch these via git submodules or your package manager.

```bash
# Using git submodules
git submodule update --init --recursive
```

### 2.2 Node.js Dependencies

```bash
cd dashboard
npm install
```

---

## Step 3: Build the C++ Data Publisher

```bash
mkdir build && cd build
cmake ..
make
```

This produces an executable named `publisher`.

---

## Step 4: Configure the Publisher

The publisher reads settings from `config.json`:

```json
{
  "api_url": "http://<DASHBOARD_HOST>:4000/api/events",
  "publish_interval_ms": 500
}
```

Edit `config.json` to point at your dashboard server.

---

## Step 5: Set Up the Web Dashboard

1. Navigate to the `dashboard` folder:

   ```bash
   cd dashboard
   ```
2. Copy `example.env` to `.env` and adjust if needed:

   ```bash
   cp example.env .env
   ```
3. Fields in `.env`:

   ```ini
   PORT=4000        # Dashboard server port
   API_ROUTE=/api/events
   ```

---

## Step 6: Run the System

1. **Start the dashboard server**

   ```bash
   cd dashboard
   npm start
   ```
2. **Launch the C++ publisher**

   ```bash
   cd build
   ./publisher
   ```

---

## Step 7: Viewing Live Metrics

Open your browser to `http://<DASHBOARD_HOST>:4000` to see real‑time bar charts and line graphs of:

* Items sorted per category
* Sorting latency over time
* Total throughput

---

## Troubleshooting

* **No data appearing?** Check publisher logs for HTTP errors and ensure `api_url` matches the dashboard’s `.env` settings.
* **CMake errors?** Verify your CMake version is >=3.10 and that dependencies are in `third_party/` or installed system‑wide.
* **Dashboard won’t start?** Run `npm install` again and confirm Node.js version is >=14.

---

## License

This project is released under the MIT License. See [LICENSE](LICENSE) for details.
