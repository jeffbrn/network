#ifndef NETWORK_APP_CONFIG_H
#define NETWORK_APP_CONFIG_H

#include <string>
#include <array>
#include <iostream>
#include "yaml-cpp/yaml.h"

namespace netw::config {

    struct ListenerConfig {
        int port {2233};
    };

    struct SenderConfig {
        std::string server_addr {"127.0.0.1"};
        int server_port {2233};
        int send_period_ms {100};
    };

    struct AppConfig {
        ListenerConfig server {};
        SenderConfig client {};

        static AppConfig read_config();
    };

    YAML::Emitter& operator<<(YAML::Emitter&, const AppConfig&);
    YAML::Emitter& operator<<(YAML::Emitter&, const ListenerConfig&);
    YAML::Emitter& operator<<(YAML::Emitter&, const SenderConfig&);

    std::ostream& operator<<(std::ostream&, const AppConfig&);
    std::ostream& operator<<(std::ostream&, const ListenerConfig&);
    std::ostream& operator<<(std::ostream&, const SenderConfig&);
}

#endif //NETWORK_APP_CONFIG_H
