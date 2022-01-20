//
// Created by wastl on 18.01.22.
//

#include <iostream>
#include <cpprest/uri.h>                        // URI library
#include <cpprest/http_msg.h>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <glog/logging.h>

#include "senec_client.h"

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web::json;                  // JSON library

json::value build_senec_request() {
    json::value request;
    request["PV1"]["POWER_RATIO"]=json::value::string("");
    request["PM1OBJ1"]["P_TOTAL"]=json::value::string("");
    request["PM1OBJ1"]["FREQ"]=json::value::string("");
    request["PM1OBJ1"]["U_AC"]=json::value::string("");
    request["PM1OBJ1"]["I_AC"]=json::value::string("");
    request["PM1OBJ1"]["P_AC"]=json::value::string("");
    request["ENERGY"]["GUI_BAT_DATA_FUEL_CHARGE"]=json::value::string("");
    request["ENERGY"]["GUI_BAT_DATA_POWER"]=json::value::string("");
    request["ENERGY"]["GUI_BAT_DATA_VOLTAGE"]=json::value::string("");
    request["ENERGY"]["GUI_HOUSE_POW"]=json::value::string("");
    request["ENERGY"]["GUI_GRID_POW"]=json::value::string("");
    request["ENERGY"]["GUI_INVERTER_POWER"]=json::value::string("");
    request["ENERGY"]["STAT_STATE"]=json::value::string("");
    request["ENERGY"]["STAT_HOURS_OF_OPERATION"]=json::value::string("");
    request["BMS"]["NR_INSTALLED"]=json::value::string("");
    request["BMS"]["TOTAL_CURRENT"]=json::value::string("");
    request["STATISTIC"]["LIVE_GRID_IMPORT"]=json::value::string("");
    request["STATISTIC"]["LIVE_GRID_EXPORT"]=json::value::string("");
    request["STATISTIC"]["LIVE_HOUSE_CONS"]=json::value::string("");
    request["STATISTIC"]["LIVE_PV_GEN"]=json::value::string("");
    request["STATISTIC"]["CURRENT_STATE"]=json::value::string("");
    request["TEMPMEASURE"]["BATTERY_TEMP"]=json::value::string("");
    request["TEMPMEASURE"]["CASE_TEMP"]=json::value::string("");
    request["TEMPMEASURE"]["MCU_TEMP"]=json::value::string("");
    request["FAN_SPEED"]["INV_LV"]=json::value::string("");
    request["PV1"]["MPP_CUR"]=json::value::string("");
    request["PV1"]["MPP_VOL"]=json::value::string("");
    request["PV1"]["MPP_POWER"]=json::value::string("");

    return request;
}

float hex2float(const std::string& s) {
    uint32_t num;
    sscanf(s.c_str(), "%x", &num);  // assuming you checked input
    return *((float*)&num);
}

template<typename T>
T senec_parse(const json::value& v) {
    if (!v.is_string()) {
        LOG(ERROR) << "Warning: value is not a string: " << v.serialize();
        return 0;
    }

    std::string s = v.as_string();

    if (s.rfind("u1", 0) == 0 || s.rfind("u3", 0) == 0 || s.rfind("u8", 0) == 0) {
        return std::stoul(s.substr(3), nullptr, 16);
    }
    if (s.rfind("i1", 0) == 0) {
        return std::stol(s.substr(3), nullptr, 16);
    }
    if (s.rfind("fl", 0) == 0) {
        return hex2float(s.substr(3));
    }
    if (s.rfind("st", 0) == 0) {
        LOG(ERROR) << "Warning: value is not a number: " << v.serialize();
        return 0;
    }
    return 0;
}

void senec::query(const std::string& uri, const std::function<void(const SenecData&)>& handler) {
    // Create http_client to send the request.
    http_client client(U(uri));

    // request data

    // Build request URI and start the request.
    uri_builder builder(U("/lala.cgi"));
    try {
        return client.request(methods::POST, builder.to_string(), build_senec_request()).then(
                [=](http_response response) {
                    if (response.status_code() == status_codes::OK) {
                        json::value result = response.extract_json(true).get();

                        senec::SenecData data;
                        data.setPvBegrenzung(senec_parse<int>(result["PV1"]["POWER_RATIO"]));
                        data.setAcLeistung(senec_parse<double>(result["PM1OBJ1"]["P_TOTAL"]));
                        data.setFrequenz(senec_parse<double>(result["PM1OBJ1"]["FREQ"]));
                        data.setAcSpannungR(senec_parse<double>(result["PM1OBJ1"]["U_AC"][0]));
                        data.setAcSpannungS(senec_parse<double>(result["PM1OBJ1"]["U_AC"][0]));
                        data.setAcSpannungT(senec_parse<double>(result["PM1OBJ1"]["U_AC"][0]));
                        data.setAcStromR(senec_parse<double>(result["PM1OBJ1"]["I_AC"][0]));
                        data.setAcStromS(senec_parse<double>(result["PM1OBJ1"]["I_AC"][0]));
                        data.setAcStromT(senec_parse<double>(result["PM1OBJ1"]["I_AC"][0]));
                        data.setAcLeistungR(senec_parse<double>(result["PM1OBJ1"]["P_AC"][0]));
                        data.setAcLeistungS(senec_parse<double>(result["PM1OBJ1"]["P_AC"][0]));
                        data.setAcLeistungT(senec_parse<double>(result["PM1OBJ1"]["P_AC"][0]));
                        data.setBatterieSoc(
                                senec_parse<double>(result["ENERGY"]["GUI_BAT_DATA_FUEL_CHARGE"]));
                        data.setBatterieLeistung(
                                senec_parse<double>(result["ENERGY"]["GUI_BAT_DATA_POWER"]));
                        data.setBatterieSpannung(
                                senec_parse<double>(result["ENERGY"]["GUI_BAT_DATA_VOLTAGE"]));
                        data.setHausverbrauch(senec_parse<double>(result["ENERGY"]["GUI_HOUSE_POW"]));
                        data.setNetzLeistung(senec_parse<double>(result["ENERGY"]["GUI_GRID_POW"]));
                        data.setPvLeistung(senec_parse<double>(result["ENERGY"]["GUI_INVERTER_POWER"]));
                        data.setStatus(senec_parse<int>(result["ENERGY"]["STAT_STATE"]));
                        data.setBetriebsstunden(
                                senec_parse<int>(result["ENERGY"]["STAT_HOURS_OF_OPERATION"]));
                        data.setAnzahlBatterien(senec_parse<int>(result["BMS"]["NR_INSTALLED"]));
                        data.setGesamtstrom(senec_parse<double>(result["BMS"]["TOTAL_CURRENT"]));
                        data.setBezugGesamt(
                                senec_parse<double>(result["STATISTIC"]["LIVE_GRID_IMPORT"]) * 1000);
                        data.setEinspeisungGesamt(
                                senec_parse<double>(result["STATISTIC"]["LIVE_GRID_EXPORT"]) * 1000);
                        data.setVerbrauchGesamt(
                                senec_parse<double>(result["STATISTIC"]["LIVE_HOUSE_CONS"]) * 1000);
                        data.setProduktionGesamt(
                                senec_parse<double>(result["STATISTIC"]["LIVE_PV_GEN"]) * 1000);
                        data.setBatterieTemperatur(
                                senec_parse<double>(result["TEMPMEASURE"]["BATTERY_TEMP"]));
                        data.setGehaeuseTemperatur(
                                senec_parse<double>(result["TEMPMEASURE"]["CASE_TEMP"]));
                        data.setMcuTemperatur(senec_parse<double>(result["TEMPMEASURE"]["MCU_TEMP"]));
                        data.setFanSpeed(senec_parse<int>(result["FAN_SPEED"]["INV_LV"]));

                        data.setMppt1Strom(senec_parse<double>(result["PV1"]["MPP_CUR"][0]));
                        data.setMppt2Strom(senec_parse<double>(result["PV1"]["MPP_CUR"][1]));
                        data.setMppt3Strom(senec_parse<double>(result["PV1"]["MPP_CUR"][2]));
                        data.setMppt1Spannung(senec_parse<double>(result["PV1"]["MPP_VOL"][0]));
                        data.setMppt2Spannung(senec_parse<double>(result["PV1"]["MPP_VOL"][1]));
                        data.setMppt3Spannung(senec_parse<double>(result["PV1"]["MPP_VOL"][2]));
                        data.setMppt1Leistung(senec_parse<double>(result["PV1"]["MPP_POWER"][0]));
                        data.setMppt2Leistung(senec_parse<double>(result["PV1"]["MPP_POWER"][1]));
                        data.setMppt3Leistung(senec_parse<double>(result["PV1"]["MPP_POWER"][2]));

                        handler(data);
                    } else {
                        LOG(ERROR) << "SENEC query failed: " << response.reason_phrase();
                    }
                }).get();
    } catch (const std::exception &e) {
        LOG(ERROR) << "Error while retrieving SENEC data: " << e.what();
    }
}

double senec::SenecData::getBatterieLeistung() const {
    return batterie_leistung;
}

void senec::SenecData::setBatterieLeistung(double batterieLeistung) {
    batterie_leistung = batterieLeistung;

    if (batterieLeistung < 0) {
        laden = 0;
        entladen = -batterieLeistung;
    } else {
        laden = batterieLeistung;
        entladen = 0;
    }
}

double senec::SenecData::getNetzLeistung() const {
    return netz_leistung;
}

void senec::SenecData::setNetzLeistung(double netzLeistung) {
    netz_leistung = netzLeistung;

    if (netzLeistung < 0) {
        einspeisung = -netzLeistung;
        bezug = 0;
    } else {
        einspeisung = 0;
        bezug = netzLeistung;
    }
}

