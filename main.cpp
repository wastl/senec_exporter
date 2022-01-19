#include <iostream>
#include <thread>
#include <chrono>
#include <yaml-cpp/yaml.h>

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

    config* cfg = load_config(argv[1]);
    std::cout << "SENEC Server: " << cfg->senec_server << std::endl;
    std::cout << "Influx Server: " << cfg->influx_server << std::endl;
    std::cout << "Poll Interval: " << cfg->poll_interval << std::endl;

    auto next = system_clock::now();
    while (true) {
        next += seconds(cfg->poll_interval);
        std::this_thread::sleep_until(next);

        senec::SenecData *data = senec::query(cfg->senec_server);

        if (data == nullptr) {
            std::cerr << "Keine Daten von SENEC-Anlage empfangen." << std::endl;
            continue;
        }

        std::cout << "Aktuell:" << std::endl;
        std::cout << "Hausverbrauch:      " << data->getHausverbrauch() << "W" << std::endl;
        std::cout << "Netzleistung:       " << data->getNetzLeistung() << "W" << std::endl;
        std::cout << "Batterieleistung:   " << data->getBatterieLeistung() << "W" << std::endl;
        std::cout << "PV-Leistung:        " << data->getPvLeistung() << "W" << std::endl;

        /*
        std::cout << std::endl;
        std::cout << "Betrieb:" << std::endl;
        std::cout << "Gehäusetemperatur:  " << data->getGehaeuseTemperatur() << "°" << std::endl;
        std::cout << "Batterietemperatur: " << data->getBatterieTemperatur() << "°" << std::endl;
        std::cout << "MCU-Temperatur:     " << data->getMcuTemperatur() << "°" << std::endl;
        std::cout << "Lüfter:             " << data->getFanSpeed() << "U/min" << std::endl;
        std::cout << std::endl;
        std::cout << "Gesamt:" << std::endl;
        std::cout << "Hausverbrauch:      " << data->getVerbrauchGesamt() << "W" << std::endl;
        std::cout << "Netzbezug:          " << data->getBezugGesamt() << "W" << std::endl;
        std::cout << "Einspeisung:        " << data->getEinspeisungGesamt() << "W" << std::endl;
        std::cout << "PV-Produktion:      " << data->getProduktionGesamt() << "W" << std::endl;
         */

        senec::write_influx(cfg->influx_server, cfg->influx_port, cfg->influx_db, *data);

        delete data;
    }

    return 0;
}
