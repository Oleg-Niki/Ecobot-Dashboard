#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <chrono>
#include <thread>
#include <string>
#include <ctime>

#include <nlohmann/json.hpp>
#include <httplib/httplib.h>

using json = nlohmann::json;
using namespace httplib;

// Load configuration from config.json
json load_config() {
    std::ifstream in("config.json");
    if (!in.is_open()) {
        std::cerr << "Failed to open config.json\n";
        std::exit(1);
    }
    json cfg; 
    in >> cfg;
    return cfg;
}

// Load category list from categories.json
std::vector<std::string> load_categories() {
    std::ifstream in("categories.json");
    if (!in.is_open()) {
        std::cerr << "Failed to open categories.json\n";
        std::exit(1);
    }
    json j; 
    in >> j;
    return j.get<std::vector<std::string>>();
}

struct SortEvent {
    std::string category;
    int latency_ms;
    int count;

    // ISO8601 timestamp
    std::string timestamp() const {
        auto now = std::chrono::system_clock::now();
        auto t   = std::chrono::system_clock::to_time_t(now);
        char buf[32];
        std::strftime(buf, sizeof(buf), "%FT%TZ", std::gmtime(&t));
        return buf;
    }

    json to_json() const {
        return json{
            {"category",   category},
            {"timestamp",  timestamp()},
            {"latency_ms", latency_ms},
            {"count",      count}
        };
    }
};

int main() {
    // 1) Load config
    json    cfg         = load_config();
    std::string api_url = cfg["api_url"];
    int     interval_ms = cfg["publish_interval_ms"];

    // 2) Load categories
    auto categories = load_categories();

    // 3) Parse host & path from api_url
    auto pos   = api_url.find("://");
    if (pos != std::string::npos) api_url = api_url.substr(pos + 3);
    auto slash = api_url.find('/');
    std::string host = api_url.substr(0, slash);
    std::string path = api_url.substr(slash);

    // 4) Create HTTP client
    Client cli(host.c_str());

    // 5) Build event queue by one event per category
    std::queue<SortEvent> queue;
    for (const auto &cat : categories) {
        // Here you could vary latency or count per category
        queue.push({cat, /*latency*/42, /*count*/1});
    }

    // 6) Publish loop
    while (!queue.empty()) {
        SortEvent ev = queue.front();
        queue.pop();

        std::string body = ev.to_json().dump();

        // Send POST
        if (auto res = cli.Post(path.c_str(), body, "application/json")) {
            if (res->status == 200) {
                std::cout << "[OK] " << body << "\n";
            } else {
                std::cerr << "[ERR] Status " << res->status
                          << " payload=" << body << "\n";
            }
        } else {
            std::cerr << "[ERR] No response, payload=" << body << "\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
    }

    return 0;
}
