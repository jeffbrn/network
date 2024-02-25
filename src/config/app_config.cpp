#include "config/app_config.h"

#include <unistd.h>
#include <iostream>
#include <fstream>

using namespace std;

namespace netw::config {

    const static char* CONFIG_FILE_NAME = "config.yaml";

    void write_config(const AppConfig& config) {
        YAML::Emitter outy;
        outy.SetOutputCharset(YAML::EscapeNonAscii);
        outy.SetIndent(4);
        outy << config;
        ofstream outf(CONFIG_FILE_NAME);
        outf << outy.c_str();
    }

    ListenerConfig read_listener_config(YAML::Node root) {
        ListenerConfig retval;
        auto yconfig = root["listener"];
        if (yconfig.Type() == YAML::NodeType::Map) {
            auto n = yconfig["port"];
            if (n.Type() == YAML::NodeType::Scalar) {
                auto val = n.as<int>();
                if (val <= 0 || val > 65535) {
                    cout << "   Invalid listener.port: " << val;
                } else retval.port = val;
            }
        } else {
            cout << "   Missing 'listener' node" << endl;
        }
        return retval;
    }

    SenderConfig read_sender_config(YAML::Node root) {
        SenderConfig retval;
        auto sconfig = root["sender"];
        if (sconfig.Type() == YAML::NodeType::Map) {
            auto n = sconfig["server_addr"];
            if (n.Type() == YAML::NodeType::Scalar) {
                auto val = n.as<std::string>();
                if (val.empty()) {
                    cout << "   Missing sender.server_addr" << endl;
                } else retval.server_addr = val;
            }
            n = sconfig["server_port"];
            if (n.Type() == YAML::NodeType::Scalar) {
                auto val = n.as<int>();
                if (val <= 0 || val > 65535) {
                    cout << "   Invalid sender.server_port: " << val;
                } else retval.server_port = val;
            }
            n = sconfig["send_period_ms"];
            if (n.Type() == YAML::NodeType::Scalar) {
                auto val = n.as<int>();
                if (val <= 0) {
                    cout << "   Invalid sender.send_period_ms: " << val;
                } else retval.send_period_ms = val;
            }
        } else {
            cout << "   Missing 'client' node" << endl;
        }
        return retval;
    }

    AppConfig AppConfig::read_config() {
        try {
            YAML::Node config = YAML::LoadFile("config.yaml");
            cout << "Reading configuration file '" << CONFIG_FILE_NAME << "'" << endl;
            AppConfig retval {
                    read_listener_config(config),
                    read_sender_config(config)
            };
            return retval;
        } catch(const YAML::BadFile& ex) {
            cout << "Config file does not exist. Creating '" << CONFIG_FILE_NAME <<"' from defaults" << endl;
            AppConfig retval;
            write_config(retval);
            return retval;
        }
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const AppConfig& o) {
        out << YAML::BeginMap;
        out << YAML::Key << "listener" << YAML::Comment("Settings for message receiver");
        out << YAML::Value << o.server;
        out << YAML::Key << "sender" << YAML::Comment("Settings for message sender");
        out << YAML::Value << o.client;
        out << YAML::EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const ListenerConfig& o) {
        out << YAML::BeginMap;
        out << YAML::Key << "port" << YAML::Value << o.port << YAML::Comment("Port to listen for messages on all interfaces");
        out << YAML::EndMap;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const SenderConfig& o) {
        out << YAML::BeginMap;
        out << YAML::Key << "server_addr" << YAML::Value << o.server_addr << YAML::Comment("IP Address to send messages");
        out << YAML::Key << "server_port" << YAML::Value << o.server_port << YAML::Comment("IP Port to send messages");
        out << YAML::Key << "send_period_ms" << YAML::Value << o.send_period_ms << YAML::Comment("Period in millisecs to send messages");
        out << YAML::EndMap;
        return out;
    }

    std::ostream& operator<<(std::ostream& os, const AppConfig& o) {
        os << "Running Config:" << endl;
        os << "listener:" << std::endl;
        os << o.server << std::endl;
        os << "sender:" << std::endl << o.client;
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const ListenerConfig& o) {
        os << "    port = " << o.port;
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const SenderConfig& o) {
        os << "    server_addr = " << o.server_addr << endl;
        os << "    server_port = " << o.server_port << endl;
        os << "    period (ms) = " << o.send_period_ms;
        return os;
    }

}