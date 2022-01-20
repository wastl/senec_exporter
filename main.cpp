#include <iostream>
#include <thread>
#include <chrono>
#include <yaml-cpp/yaml.h>
#include <glog/logging.h>

#include "senec_client.h"
#include "senec_influx.h"

using namespace std::chrono;

class config {
public:
    config() {}

    // SENEC configuration
    std::string senec_server;

    // Influx configuration
    std::string influx_server = "127.0.0.1";
    int influx_port = 8086;
    std::string influx_db = "senec";
    std::string influx_user = "";
    std::string influx_password = "";

    // Tool configuration
    int poll_interval = 1;
};

config* load_config(const std::string& file) {
    YAML::Node yaml = YAML::LoadFile(file);

    config* c = new config();
    if (!yaml["senec_server"]) {
        std::cerr << "SENEC Server nicht konfiguriert" << std::endl;
        return c;
    }
    c->senec_server = yaml["senec_server"].as<std::string>();

    if (yaml["influx_server"]) {
        c->influx_server = yaml["influx_server"].as<std::string>();
    }
    if (yaml["influx_db"]) {
        c->influx_db = yaml["influx_db"].as<std::string>();
    }
    if (yaml["influx_port"]) {
        c->influx_port = yaml["influx_port"].as<int>();
    }
    if (yaml["influx_user"]) {
        c->influx_user = yaml["influx_user"].as<std::string>();
    }
    if (yaml["influx_password"]) {
        c->influx_password = yaml["influx_password"].as<std::string>();
    }
    if (yaml["poll_interval"]) {
        c->poll_interval = yaml["poll_interval"].as<int>();
    }

    return c;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: senec_exporter <path-to-config.yml>" << std::endl;
        return 1;
    }

    google::InitGoogleLogging(argv[0]);

    config* cfg = load_config(argv[1]);
    LOG(INFO) << "SENEC Server: " << cfg->senec_server;
    LOG(INFO) << "Influx Server: " << cfg->influx_server;
    LOG(INFO) << "Poll Interval: " << cfg->poll_interval;

    auto next = system_clock::now();
    while (true) {
        next += seconds(cfg->poll_interval);
        std::this_thread::sleep_until(next);

        senec::query(cfg->senec_server, [=](const senec::SenecData& data) {
            LOG(INFO) << "SENEC Daten empfangen (Verbrauch: " << data.getHausverbrauch()
                      << ", Produktion: " << data.getPvLeistung()
                      << ", Ladestand: " << data.getBatterieSoc() <<")";

            senec::write_influx(cfg->influx_server, cfg->influx_port, cfg->influx_db, data);
        });
    }

    return 0;
}
