//
// Created by wastl on 18.01.22.
//

#ifndef SENEC_EXPORTER_SENEC_INFLUX_H
#define SENEC_EXPORTER_SENEC_INFLUX_H

#include "senec_client.h"

namespace senec {

    int write_influx(const std::string& server, int port, const std::string& db, const SenecData& data);
}

#endif //SENEC_EXPORTER_SENEC_INFLUX_H
