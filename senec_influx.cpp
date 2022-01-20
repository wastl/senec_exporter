//
// Created by wastl on 18.01.22.
//

#include <chrono>
#include <influxdb.hpp>
#include <glog/logging.h>

#include "senec_influx.h"

using namespace std::chrono;

int senec::write_influx(const std::string &server, int port, const std::string &db, const senec::SenecData &data) {
    long long ts = time_point_cast<nanoseconds>(system_clock::now()).time_since_epoch().count();

    int result;
    std::string response;

    influxdb_cpp::server_info si(server, port, db);
    result = influxdb_cpp::builder()
            .meas("batterie")
            .field("soc", data.getBatterieSoc())
            .field("leistung", data.getBatterieLeistung())
            .field("spannung", data.getBatterieSpannung())
            .field("temperatur", data.getBatterieTemperatur())
            .timestamp(ts)
            .post_http(si, &response);

    if (result != 0) {
        LOG(ERROR) << "Error while writing to InfluxDB: " << response;
        return result;
    }

    result = influxdb_cpp::builder()
            .meas("leistung")
            .field("verbrauch", data.getHausverbrauch())
            .field("pv", data.getPvLeistung())
            .field("batterie", data.getBatterieLeistung())
            .field("netz", data.getNetzLeistung())
            .timestamp(ts)
            .post_http(si, &response);

    if (result != 0) {
        LOG(ERROR) << "Error while writing to InfluxDB: " << response;
        return result;
    }

    result = influxdb_cpp::builder()
            .meas("quellen")
            .field("bezug", data.getBezug())
            .field("einspeisung", data.getEinspeisung())
            .field("laden", data.getLaden())
            .field("entladen", data.getEntladen())
            .timestamp(ts)
            .post_http(si, &response);

    if (result != 0) {
        LOG(ERROR) << "Error while writing to InfluxDB: " << response;
        return result;
    }

    result = influxdb_cpp::builder()
            .meas("gesamt")
            .field("bezug", data.getBezugGesamt())
            .field("einspeisung", data.getEinspeisungGesamt())
            .field("verbrauch", data.getVerbrauchGesamt())
            .field("strom", data.getGesamtstrom())
            .field("produktion", data.getProduktionGesamt())
            .timestamp(ts)
            .post_http(si, &response);

    if (result != 0) {
        LOG(ERROR) << "Error while writing to InfluxDB: " << response;
        return result;
    }

    result = influxdb_cpp::builder()
            .meas("mppt1")
            .field("leistung", data.getMppt1Leistung())
            .field("spannung", data.getMppt1Leistung())
            .field("strom", data.getMppt1Strom())
            .timestamp(ts)
            .post_http(si, &response);

    if (result != 0) {
        LOG(ERROR) << "Error while writing to InfluxDB: " << response;
        return result;
    }

    result = influxdb_cpp::builder()
            .meas("mppt2")
            .field("leistung", data.getMppt2Leistung())
            .field("spannung", data.getMppt2Leistung())
            .field("strom", data.getMppt2Strom())
            .timestamp(ts)
            .post_http(si, &response);

    if (result != 0) {
        LOG(ERROR) << "Error while writing to InfluxDB: " << response;
        return result;
    }

    result = influxdb_cpp::builder()
            .meas("mppt3")
            .field("leistung", data.getMppt3Leistung())
            .field("spannung", data.getMppt3Leistung())
            .field("strom", data.getMppt3Strom())
            .timestamp(ts)
            .post_http(si, &response);

    if (result != 0) {
        LOG(ERROR) << "Error while writing to InfluxDB: " << response;
        return result;
    }

    return 0;
}
