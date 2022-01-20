//
// Created by wastl on 18.01.22.
//

#ifndef SENEC_EXPORTER_SENEC_CLIENT_H
#define SENEC_EXPORTER_SENEC_CLIENT_H
#include <functional>

namespace senec {
    class SenecData {
    private:
        uint32_t pv_begrenzung;
        double ac_leistung;
        double frequenz;

        double ac_spannung_r, ac_spannung_s, ac_spannung_t;
        double ac_strom_r, ac_strom_s, ac_strom_t;
        double ac_leistung_r, ac_leistung_s, ac_leistung_t;

        double batterie_soc;
        double batterie_leistung;
        double batterie_spannung;

        double hausverbrauch;
        double pv_leistung;
        double netz_leistung;

        double einspeisung;
        double bezug;
        double laden;
        double entladen;

        uint32_t status;
        uint32_t betriebsstunden;
        uint32_t anzahl_batterien;

        double gesamtstrom;
        double einspeisung_gesamt;
        double bezug_gesamt;
        double verbrauch_gesamt;
        double produktion_gesamt;

        double batterie_temperatur, gehaeuse_temperatur, mcu_temperatur;

        uint32_t fan_speed;

        double mppt1_strom, mppt1_spannung, mppt1_leistung;
        double mppt2_strom, mppt2_spannung, mppt2_leistung;
        double mppt3_strom, mppt3_spannung, mppt3_leistung;
    public:
        SenecData() {}

        uint32_t getPvBegrenzung() const {
            return pv_begrenzung;
        }

        void setPvBegrenzung(uint32_t pvBegrenzung) {
            pv_begrenzung = pvBegrenzung;
        }

        double getAcLeistung() const {
            return ac_leistung;
        }

        void setAcLeistung(double acLeistung) {
            ac_leistung = acLeistung;
        }

        double getFrequenz() const {
            return frequenz;
        }

        void setFrequenz(double frequenz) {
            SenecData::frequenz = frequenz;
        }

        double getAcSpannungR() const {
            return ac_spannung_r;
        }

        void setAcSpannungR(double acSpannungR) {
            ac_spannung_r = acSpannungR;
        }

        double getAcSpannungS() const {
            return ac_spannung_s;
        }

        void setAcSpannungS(double acSpannungS) {
            ac_spannung_s = acSpannungS;
        }

        double getAcSpannungT() const {
            return ac_spannung_t;
        }

        void setAcSpannungT(double acSpannungT) {
            ac_spannung_t = acSpannungT;
        }

        double getAcStromR() const {
            return ac_strom_r;
        }

        void setAcStromR(double acStromR) {
            ac_strom_r = acStromR;
        }

        double getAcStromS() const {
            return ac_strom_s;
        }

        void setAcStromS(double acStromS) {
            ac_strom_s = acStromS;
        }

        double getAcStromT() const {
            return ac_strom_t;
        }

        void setAcStromT(double acStromT) {
            ac_strom_t = acStromT;
        }

        double getAcLeistungR() const {
            return ac_leistung_r;
        }

        void setAcLeistungR(double acLeistungR) {
            ac_leistung_r = acLeistungR;
        }

        double getAcLeistungS() const {
            return ac_leistung_s;
        }

        void setAcLeistungS(double acLeistungS) {
            ac_leistung_s = acLeistungS;
        }

        double getAcLeistungT() const {
            return ac_leistung_t;
        }

        void setAcLeistungT(double acLeistungT) {
            ac_leistung_t = acLeistungT;
        }

        double getBatterieSoc() const {
            return batterie_soc;
        }

        void setBatterieSoc(double batterieSoc) {
            batterie_soc = batterieSoc;
        }

        double getBatterieLeistung() const;

        void setBatterieLeistung(double batterieLeistung);

        double getBatterieSpannung() const {
            return batterie_spannung;
        }

        void setBatterieSpannung(double batterieSpannung) {
            batterie_spannung = batterieSpannung;
        }

        double getHausverbrauch() const {
            return hausverbrauch;
        }

        void setHausverbrauch(double hausverbrauch) {
            SenecData::hausverbrauch = hausverbrauch;
        }

        double getPvLeistung() const {
            return pv_leistung;
        }

        void setPvLeistung(double pvLeistung) {
            pv_leistung = pvLeistung;
        }

        double getNetzLeistung() const;

        void setNetzLeistung(double netzLeistung);


        uint32_t getStatus() const {
            return status;
        }

        void setStatus(uint32_t status) {
            SenecData::status = status;
        }

        uint32_t getBetriebsstunden() const {
            return betriebsstunden;
        }

        void setBetriebsstunden(uint32_t betriebsstunden) {
            SenecData::betriebsstunden = betriebsstunden;
        }

        uint32_t getAnzahlBatterien() const {
            return anzahl_batterien;
        }

        void setAnzahlBatterien(uint32_t anzahlBatterien) {
            anzahl_batterien = anzahlBatterien;
        }

        double getGesamtstrom() const {
            return gesamtstrom;
        }

        void setGesamtstrom(double gesamtstrom) {
            SenecData::gesamtstrom = gesamtstrom;
        }

        double getEinspeisungGesamt() const {
            return einspeisung_gesamt;
        }

        void setEinspeisungGesamt(double einspeisungGesamt) {
            einspeisung_gesamt = einspeisungGesamt;
        }

        double getBezugGesamt() const {
            return bezug_gesamt;
        }

        void setBezugGesamt(double bezugGesamt) {
            bezug_gesamt = bezugGesamt;
        }

        double getVerbrauchGesamt() const {
            return verbrauch_gesamt;
        }

        void setVerbrauchGesamt(double verbrauchGesamt) {
            verbrauch_gesamt = verbrauchGesamt;
        }

        double getProduktionGesamt() const {
            return produktion_gesamt;
        }

        void setProduktionGesamt(double produktionGesamt) {
            produktion_gesamt = produktionGesamt;
        }

        double getBatterieTemperatur() const {
            return batterie_temperatur;
        }

        void setBatterieTemperatur(double batterieTemperatur) {
            batterie_temperatur = batterieTemperatur;
        }

        double getGehaeuseTemperatur() const {
            return gehaeuse_temperatur;
        }

        void setGehaeuseTemperatur(double gehaeuseTemperatur) {
            gehaeuse_temperatur = gehaeuseTemperatur;
        }

        double getMcuTemperatur() const {
            return mcu_temperatur;
        }

        void setMcuTemperatur(double mcuTemperatur) {
            mcu_temperatur = mcuTemperatur;
        }

        uint32_t getFanSpeed() const {
            return fan_speed;
        }

        void setFanSpeed(uint32_t fanSpeed) {
            fan_speed = fanSpeed;
        }

        double getMppt1Strom() const {
            return mppt1_strom;
        }

        void setMppt1Strom(double mppt1Strom) {
            mppt1_strom = mppt1Strom;
        }

        double getMppt1Spannung() const {
            return mppt1_spannung;
        }

        void setMppt1Spannung(double mppt1Spannung) {
            mppt1_spannung = mppt1Spannung;
        }

        double getMppt1Leistung() const {
            return mppt1_leistung;
        }

        void setMppt1Leistung(double mppt1Leistung) {
            mppt1_leistung = mppt1Leistung;
        }

        double getMppt2Strom() const {
            return mppt2_strom;
        }

        void setMppt2Strom(double mppt2Strom) {
            mppt2_strom = mppt2Strom;
        }

        double getMppt2Spannung() const {
            return mppt2_spannung;
        }

        void setMppt2Spannung(double mppt2Spannung) {
            mppt2_spannung = mppt2Spannung;
        }

        double getMppt2Leistung() const {
            return mppt2_leistung;
        }

        void setMppt2Leistung(double mppt2Leistung) {
            mppt2_leistung = mppt2Leistung;
        }

        double getMppt3Strom() const {
            return mppt3_strom;
        }

        void setMppt3Strom(double mppt3Strom) {
            mppt3_strom = mppt3Strom;
        }

        double getMppt3Spannung() const {
            return mppt3_spannung;
        }

        void setMppt3Spannung(double mppt3Spannung) {
            mppt3_spannung = mppt3Spannung;
        }

        double getMppt3Leistung() const {
            return mppt3_leistung;
        }

        void setMppt3Leistung(double mppt3Leistung) {
            mppt3_leistung = mppt3Leistung;
        }

        double getEinspeisung() const {
            return einspeisung;
        }

        double getBezug() const {
            return bezug;
        }

        double getLaden() const {
            return laden;
        }

        double getEntladen() const {
            return entladen;
        }
    };

    void query(const std::string& uri, const std::function<void(const SenecData&)>& handler);
}

#endif //SENEC_EXPORTER_SENEC_CLIENT_H
