#include <iostream>
#include <fstream>
#include <queue>
#include <chrono>
#include <thread>
#include <string>
#include <ctime>

#include <nlohmann/json.hpp>
#include <httplib/httplib.h>cmake --build . --config Debug

using json = nlohmann::json;
using namespace httplib;

// Load configuration from config.json
json load_config() {
    std::ifstream in("config.json");
    if (!in.is_open()) {
        std::cerr << "Failed to open config.json\n";
        std::exit(1);
    }
    json cfg; in >> cfg;
    return cfg;
}

struct SortEvent {
    std::string category;
    int latency_ms;
    int count;

    std::string timestamp() const {
        auto now = std::chrono::system_clock::now();
        auto t   = std::chrono::system_clock::to_time_t(now);
        char buf[32];
        std::strftime(buf, sizeof(buf), "%FT%TZ", std::gmtime(&t));
        return std::string(buf);
    }

    json to_json() const {
        return {
            {"category",   category},
            {"timestamp",  timestamp()},
            {"latency_ms", latency_ms},
            {"count",      count}
        };
    }
};

int main() {
    // 1) Load config
    json cfg = load_config();
    std::string api_url = cfg["api_url"];
    int interval_ms     = cfg["publish_interval_ms"];

    // 2) Parse host & path
    auto pos = api_url.find("://");
    if (pos != std::string::npos)
        api_url = api_url.substr(pos + 3);
    auto slash = api_url.find('/');
    std::string host = api_url.substr(0, slash);
    std::string path = api_url.substr(slash);

    // 3) Create HTTP client
    Client cli(host.c_str());

    // 4) Prepare event queue
    std::queue<SortEvent> queue;
    queue.push({"plastic", 42, 1});  // dummy event

    // 5) Publish loop
    while (!queue.empty()) {
        SortEvent ev = queue.front();
        queue.pop();

        std::string body = ev.to_json().dump();

        // Send POST
        auto res = cli.Post(path.c_str(), body, "application/json");
        if (res && res->status == 200) {
            std::cout << "[OK] " << body << "\n";
        } else {
            std::cerr << "[ERR] Failed to POST: "
                      << (res ? std::to_string(res->status) : "no response")
                      << "\n  payload=" << body << "\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
    }

    return 0;
}
