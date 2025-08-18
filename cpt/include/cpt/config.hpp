#ifndef CPT_CONFIG_HPP
#define CPT_CONFIG_HPP

#include <string>
#include <variant>
#include <vector>
#include <unordered_map>

#include "cpt/io.hpp"

namespace cpt {

    class ConfigNode;
    using ConfigMap = std::unordered_map<std::string, ConfigNode>;

    class ConfigNode {
    public:
        std::variant<std::string, std::shared_ptr<ConfigMap>> value;

        ConfigNode() = default;
        explicit ConfigNode(std::string s);
        explicit ConfigNode(const ConfigMap& m);
        explicit ConfigNode(ConfigMap&& m);
    };

    class Config {
    public:
        static Config from_file(const Path& path);
        std::string get(const std::string key) const;
    private:
        static ConfigMap parse_yaml(const std::string& contents);
        static ConfigMap parse_block(const std::vector<std::string>& lines, int begin, int end, int indent);

        ConfigMap map_;
    };
}

#endif //CPT_CONFIG_HPP