//
// Created by wastl on 18.01.22.
//

#ifndef SENEC_EXPORTER_SENEC_CLIENT_H
#define SENEC_EXPORTER_SENEC_CLIENT_H

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
        uint32_t getPvBegrenzung() const;

        void setPvBegrenzung(uint32_t pvBegrenzung);

        double getAcLeistung() const;

        void setAcLeistung(double acLeistung);

        double getFrequenz() const;

        void setFrequenz(double frequenz);

        double getAcSpannungR() const;

        void setAcSpannungR(double acSpannungR);

        double getAcSpannungS() const;

        void setAcSpannungS(double acSpannungS);

        double getAcSpannungT() const;

        void setAcSpannungT(double acSpannungT);

        double getAcStromR() const;

        void setAcStromR(double acStromR);

        double getAcStromS() const;

        void setAcStromS(double acStromS);

        double getAcStromT() const;

        void setAcStromT(double acStromT);

        double getAcLeistungR() const;

        void setAcLeistungR(double acLeistungR);

        double getAcLeistungS() const;

        void setAcLeistungS(double acLeistungS);

        double getAcLeistungT() const;

        void setAcLeistungT(double acLeistungT);

        double getBatterieSoc() const;

        void setBatterieSoc(double batterieSoc);

        double getBatterieLeistung() const;

        void setBatterieLeistung(double batterieLeistung);

        double getBatterieSpannung() const;

        void setBatterieSpannung(double batterieSpannung);

        double getHausverbrauch() const;

        void setHausverbrauch(double hausverbrauch);

        double getPvLeistung() const;

        void setPvLeistung(double pvLeistung);

        double getNetzLeistung() const;

        void setNetzLeistung(double netzLeistung);

        uint32_t getStatus() const;

        void setStatus(uint32_t status);

        uint32_t getBetriebsstunden() const;

        void setBetriebsstunden(uint32_t betriebsstunden);

        uint32_t getAnzahlBatterien() const;

        void setAnzahlBatterien(uint32_t anzahlBatterien);

        double getGesamtstrom() const;

        void setGesamtstrom(double gesamtstrom);

        double getEinspeisungGesamt() const;

        void setEinspeisungGesamt(double einspeisungGesamt);

        double getBezugGesamt() const;

        void setBezugGesamt(double bezugGesamt);

        double getVerbrauchGesamt() const;

        void setVerbrauchGesamt(double verbrauchGesamt);

        double getProduktionGesamt() const;

        void setProduktionGesamt(double produktionGesamt);

        double getBatterieTemperatur() const;

        void setBatterieTemperatur(double batterieTemperatur);

        double getGehaeuseTemperatur() const;

        void setGehaeuseTemperatur(double gehaeuseTemperatur);

        double getMcuTemperatur() const;

        void setMcuTemperatur(double mcuTemperatur);

        uint32_t getFanSpeed() const;

        void setFanSpeed(uint32_t fanSpeed);

        double getEinspeisung() const;

        double getBezug() const;

        double getMppt1Strom() const;

        void setMppt1Strom(double mppt1Strom);

        double getMppt1Spannung() const;

        void setMppt1Spannung(double mppt1Spannung);

        double getMppt1Leistung() const;

        void setMppt1Leistung(double mppt1Leistung);

        double getMppt2Strom() const;

        void setMppt2Strom(double mppt2Strom);

        double getMppt2Spannung() const;

        void setMppt2Spannung(double mppt2Spannung);

        double getMppt2Leistung() const;

        void setMppt2Leistung(double mppt2Leistung);

        double getMppt3Strom() const;

        void setMppt3Strom(double mppt3Strom);

        double getMppt3Spannung() const;

        void setMppt3Spannung(double mppt3Spannung);

        double getMppt3Leistung() const;

        void setMppt3Leistung(double mppt3Leistung);

        double getLaden() const;

        double getEntladen() const;
    };

    senec::SenecData* query(const std::string& uri);
}

#endif //SENEC_EXPORTER_SENEC_CLIENT_H
