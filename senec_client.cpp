//
// Created by wastl on 18.01.22.
//

#include <iostream>
#include <cpprest/uri.h>                        // URI library
#include <cpprest/http_msg.h>
#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include <cpprest/json.h>

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

json::value senec_parse_decimal(const json::value& v) {
    if (!v.is_string()) {
        std::cout << "Warning: value is not a string: " << v.serialize() << std::endl;
        return json::value::number(0);
    }

    std::string s = v.as_string();

    if (s.rfind("u1", 0) == 0 || s.rfind("u3", 0) == 0 || s.rfind("u8", 0) == 0) {
        return json::value::number(std::stoul(s.substr(3), nullptr, 16));
    }
    if (s.rfind("i1", 0) == 0) {
        return json::value::number(std::stol(s.substr(3), nullptr, 16));
    }
    if (s.rfind("fl", 0) == 0) {
        return json::value::number(hex2float(s.substr(3)));
    }
    if (s.rfind("st", 0) == 0) {
        return json::value::string(s.substr(3));
    }
    return json::value::number(0);
}

senec::SenecData* senec::query(const std::string& uri) {
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

                        senec::SenecData *data = new senec::SenecData();
                        data->setPvBegrenzung(senec_parse_decimal(result["PV1"]["POWER_RATIO"]).as_integer());
                        data->setAcLeistung(senec_parse_decimal(result["PM1OBJ1"]["P_TOTAL"]).as_double());
                        data->setFrequenz(senec_parse_decimal(result["PM1OBJ1"]["FREQ"]).as_double());
                        data->setAcSpannungR(senec_parse_decimal(result["PM1OBJ1"]["U_AC"][0]).as_double());
                        data->setAcSpannungS(senec_parse_decimal(result["PM1OBJ1"]["U_AC"][0]).as_double());
                        data->setAcSpannungT(senec_parse_decimal(result["PM1OBJ1"]["U_AC"][0]).as_double());
                        data->setAcStromR(senec_parse_decimal(result["PM1OBJ1"]["I_AC"][0]).as_double());
                        data->setAcStromS(senec_parse_decimal(result["PM1OBJ1"]["I_AC"][0]).as_double());
                        data->setAcStromT(senec_parse_decimal(result["PM1OBJ1"]["I_AC"][0]).as_double());
                        data->setAcLeistungR(senec_parse_decimal(result["PM1OBJ1"]["P_AC"][0]).as_double());
                        data->setAcLeistungS(senec_parse_decimal(result["PM1OBJ1"]["P_AC"][0]).as_double());
                        data->setAcLeistungT(senec_parse_decimal(result["PM1OBJ1"]["P_AC"][0]).as_double());
                        data->setBatterieSoc(
                                senec_parse_decimal(result["ENERGY"]["GUI_BAT_DATA_FUEL_CHARGE"]).as_double());
                        data->setBatterieLeistung(
                                senec_parse_decimal(result["ENERGY"]["GUI_BAT_DATA_POWER"]).as_double());
                        data->setBatterieSpannung(
                                senec_parse_decimal(result["ENERGY"]["GUI_BAT_DATA_VOLTAGE"]).as_double());
                        data->setHausverbrauch(senec_parse_decimal(result["ENERGY"]["GUI_HOUSE_POW"]).as_double());
                        data->setNetzLeistung(senec_parse_decimal(result["ENERGY"]["GUI_GRID_POW"]).as_double());
                        data->setPvLeistung(senec_parse_decimal(result["ENERGY"]["GUI_INVERTER_POWER"]).as_double());
                        data->setStatus(senec_parse_decimal(result["ENERGY"]["STAT_STATE"]).as_integer());
                        data->setBetriebsstunden(
                                senec_parse_decimal(result["ENERGY"]["STAT_HOURS_OF_OPERATION"]).as_integer());
                        data->setAnzahlBatterien(senec_parse_decimal(result["BMS"]["NR_INSTALLED"]).as_integer());
                        data->setGesamtstrom(senec_parse_decimal(result["BMS"]["TOTAL_CURRENT"]).as_double());
                        data->setBezugGesamt(
                                senec_parse_decimal(result["STATISTIC"]["LIVE_GRID_IMPORT"]).as_double() * 1000);
                        data->setEinspeisungGesamt(
                                senec_parse_decimal(result["STATISTIC"]["LIVE_GRID_EXPORT"]).as_double() * 1000);
                        data->setVerbrauchGesamt(
                                senec_parse_decimal(result["STATISTIC"]["LIVE_HOUSE_CONS"]).as_double() * 1000);
                        data->setProduktionGesamt(
                                senec_parse_decimal(result["STATISTIC"]["LIVE_PV_GEN"]).as_double() * 1000);
                        data->setBatterieTemperatur(
                                senec_parse_decimal(result["TEMPMEASURE"]["BATTERY_TEMP"]).as_double());
                        data->setGehaeuseTemperatur(
                                senec_parse_decimal(result["TEMPMEASURE"]["CASE_TEMP"]).as_double());
                        data->setMcuTemperatur(senec_parse_decimal(result["TEMPMEASURE"]["MCU_TEMP"]).as_double());
                        data->setFanSpeed(senec_parse_decimal(result["FAN_SPEED"]["INV_LV"]).as_integer());

                        data->setMppt1Strom(senec_parse_decimal(result["PV1"]["MPP_CUR"][0]).as_double());
                        data->setMppt2Strom(senec_parse_decimal(result["PV1"]["MPP_CUR"][1]).as_double());
                        data->setMppt3Strom(senec_parse_decimal(result["PV1"]["MPP_CUR"][2]).as_double());
                        data->setMppt1Spannung(senec_parse_decimal(result["PV1"]["MPP_VOL"][0]).as_double());
                        data->setMppt2Spannung(senec_parse_decimal(result["PV1"]["MPP_VOL"][1]).as_double());
                        data->setMppt3Spannung(senec_parse_decimal(result["PV1"]["MPP_VOL"][2]).as_double());
                        data->setMppt1Leistung(senec_parse_decimal(result["PV1"]["MPP_POWER"][0]).as_double());
                        data->setMppt2Leistung(senec_parse_decimal(result["PV1"]["MPP_POWER"][1]).as_double());
                        data->setMppt3Leistung(senec_parse_decimal(result["PV1"]["MPP_POWER"][2]).as_double());

                        return data;
                    } else {
                        std::cout << "SENEC query failed: " << response.reason_phrase() << std::endl;
                        return (SenecData *) nullptr;
                    }
                }).get();
    } catch (const std::exception &e) {
        std::cerr << "Error while retrieving SENEC data: " << e.what() <<std::endl;
        return nullptr;
    }
}

double senec::SenecData::getAcLeistung() const {
    return ac_leistung;
}

void senec::SenecData::setAcLeistung(double acLeistung) {
    ac_leistung = acLeistung;
}

double senec::SenecData::getFrequenz() const {
    return frequenz;
}

void senec::SenecData::setFrequenz(double frequenz) {
    SenecData::frequenz = frequenz;
}

double senec::SenecData::getAcSpannungR() const {
    return ac_spannung_r;
}

void senec::SenecData::setAcSpannungR(double acSpannungR) {
    ac_spannung_r = acSpannungR;
}

double senec::SenecData::getAcSpannungS() const {
    return ac_spannung_s;
}

void senec::SenecData::setAcSpannungS(double acSpannungS) {
    ac_spannung_s = acSpannungS;
}

double senec::SenecData::getAcSpannungT() const {
    return ac_spannung_t;
}

void senec::SenecData::setAcSpannungT(double acSpannungT) {
    ac_spannung_t = acSpannungT;
}

double senec::SenecData::getAcStromR() const {
    return ac_strom_r;
}

void senec::SenecData::setAcStromR(double acStromR) {
    ac_strom_r = acStromR;
}

double senec::SenecData::getAcStromS() const {
    return ac_strom_s;
}

void senec::SenecData::setAcStromS(double acStromS) {
    ac_strom_s = acStromS;
}

double senec::SenecData::getAcStromT() const {
    return ac_strom_t;
}

void senec::SenecData::setAcStromT(double acStromT) {
    ac_strom_t = acStromT;
}

double senec::SenecData::getAcLeistungR() const {
    return ac_leistung_r;
}

void senec::SenecData::setAcLeistungR(double acLeistungR) {
    ac_leistung_r = acLeistungR;
}

double senec::SenecData::getAcLeistungS() const {
    return ac_leistung_s;
}

void senec::SenecData::setAcLeistungS(double acLeistungS) {
    ac_leistung_s = acLeistungS;
}

double senec::SenecData::getAcLeistungT() const {
    return ac_leistung_t;
}

void senec::SenecData::setAcLeistungT(double acLeistungT) {
    ac_leistung_t = acLeistungT;
}

double senec::SenecData::getBatterieSoc() const {
    return batterie_soc;
}

void senec::SenecData::setBatterieSoc(double batterieSoc) {
    batterie_soc = batterieSoc;
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

double senec::SenecData::getBatterieSpannung() const {
    return batterie_spannung;
}

void senec::SenecData::setBatterieSpannung(double batterieSpannung) {
    batterie_spannung = batterieSpannung;
}

double senec::SenecData::getHausverbrauch() const {
    return hausverbrauch;
}

void senec::SenecData::setHausverbrauch(double hausverbrauch) {
    SenecData::hausverbrauch = hausverbrauch;
}

double senec::SenecData::getPvLeistung() const {
    return pv_leistung;
}

void senec::SenecData::setPvLeistung(double pvLeistung) {
    pv_leistung = pvLeistung;
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

uint32_t senec::SenecData::getStatus() const {
    return status;
}

void senec::SenecData::setStatus(uint32_t status) {
    SenecData::status = status;
}

uint32_t senec::SenecData::getBetriebsstunden() const {
    return betriebsstunden;
}

void senec::SenecData::setBetriebsstunden(uint32_t betriebsstunden) {
    SenecData::betriebsstunden = betriebsstunden;
}

uint32_t senec::SenecData::getAnzahlBatterien() const {
    return anzahl_batterien;
}

void senec::SenecData::setAnzahlBatterien(uint32_t anzahlBatterien) {
    anzahl_batterien = anzahlBatterien;
}

uint32_t senec::SenecData::getPvBegrenzung() const {
    return pv_begrenzung;
}

void senec::SenecData::setPvBegrenzung(uint32_t pvBegrenzung) {
    pv_begrenzung = pvBegrenzung;
}

double senec::SenecData::getGesamtstrom() const {
    return gesamtstrom;
}

void senec::SenecData::setGesamtstrom(double gesamtstrom) {
    SenecData::gesamtstrom = gesamtstrom;
}

double senec::SenecData::getEinspeisungGesamt() const {
    return einspeisung_gesamt;
}

void senec::SenecData::setEinspeisungGesamt(double einspeisungGesamt) {
    einspeisung_gesamt = einspeisungGesamt;
}

double senec::SenecData::getBezugGesamt() const {
    return bezug_gesamt;
}

void senec::SenecData::setBezugGesamt(double bezugGesamt) {
    bezug_gesamt = bezugGesamt;
}

double senec::SenecData::getVerbrauchGesamt() const {
    return verbrauch_gesamt;
}

void senec::SenecData::setVerbrauchGesamt(double verbrauchGesamt) {
    verbrauch_gesamt = verbrauchGesamt;
}

double senec::SenecData::getProduktionGesamt() const {
    return produktion_gesamt;
}

void senec::SenecData::setProduktionGesamt(double produktionGesamt) {
    produktion_gesamt = produktionGesamt;
}

double senec::SenecData::getBatterieTemperatur() const {
    return batterie_temperatur;
}

void senec::SenecData::setBatterieTemperatur(double batterieTemperatur) {
    batterie_temperatur = batterieTemperatur;
}

double senec::SenecData::getGehaeuseTemperatur() const {
    return gehaeuse_temperatur;
}

void senec::SenecData::setGehaeuseTemperatur(double gehaeuseTemperatur) {
    gehaeuse_temperatur = gehaeuseTemperatur;
}

double senec::SenecData::getMcuTemperatur() const {
    return mcu_temperatur;
}

void senec::SenecData::setMcuTemperatur(double mcuTemperatur) {
    mcu_temperatur = mcuTemperatur;
}

uint32_t senec::SenecData::getFanSpeed() const {
    return fan_speed;
}

void senec::SenecData::setFanSpeed(uint32_t fanSpeed) {
    fan_speed = fanSpeed;
}

double senec::SenecData::getEinspeisung() const {
    return einspeisung;
}

double senec::SenecData::getBezug() const {
    return bezug;
}

double senec::SenecData::getMppt1Strom() const {
    return mppt1_strom;
}

void senec::SenecData::setMppt1Strom(double mppt1Strom) {
    mppt1_strom = mppt1Strom;
}

double senec::SenecData::getMppt1Spannung() const {
    return mppt1_spannung;
}

void senec::SenecData::setMppt1Spannung(double mppt1Spannung) {
    mppt1_spannung = mppt1Spannung;
}

double senec::SenecData::getMppt1Leistung() const {
    return mppt1_leistung;
}

void senec::SenecData::setMppt1Leistung(double mppt1Leistung) {
    mppt1_leistung = mppt1Leistung;
}

double senec::SenecData::getMppt2Strom() const {
    return mppt2_strom;
}

void senec::SenecData::setMppt2Strom(double mppt2Strom) {
    mppt2_strom = mppt2Strom;
}

double senec::SenecData::getMppt2Spannung() const {
    return mppt2_spannung;
}

void senec::SenecData::setMppt2Spannung(double mppt2Spannung) {
    mppt2_spannung = mppt2Spannung;
}

double senec::SenecData::getMppt2Leistung() const {
    return mppt2_leistung;
}

void senec::SenecData::setMppt2Leistung(double mppt2Leistung) {
    mppt2_leistung = mppt2Leistung;
}

double senec::SenecData::getMppt3Strom() const {
    return mppt3_strom;
}

void senec::SenecData::setMppt3Strom(double mppt3Strom) {
    mppt3_strom = mppt3Strom;
}

double senec::SenecData::getMppt3Spannung() const {
    return mppt3_spannung;
}

void senec::SenecData::setMppt3Spannung(double mppt3Spannung) {
    mppt3_spannung = mppt3Spannung;
}

double senec::SenecData::getMppt3Leistung() const {
    return mppt3_leistung;
}

void senec::SenecData::setMppt3Leistung(double mppt3Leistung) {
    mppt3_leistung = mppt3Leistung;
}

double senec::SenecData::getLaden() const {
    return laden;
}

double senec::SenecData::getEntladen() const {
    return entladen;
}

