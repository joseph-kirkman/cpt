#include "cpt/config.hpp"
#include "cpt/string.hpp"

namespace cpt {
    ConfigNode::ConfigNode(std::string s) : value(std::move(s)) {}
    ConfigNode::ConfigNode(const ConfigMap& m) : value(std::make_shared<ConfigMap>(m)) {}
    ConfigNode::ConfigNode(ConfigMap&& m) : value(std::make_shared<ConfigMap>(std::move(m))) {}

    Config Config::from_file(const Path& path) {
        if(!path.is_file()){
            throw std::runtime_error("cannot find config " + path.str());
        }
        auto contents = File::read(path);
        Config config{};
        config.map_ = parse_yaml(contents);
        return config;
    }

    ConfigMap Config::parse_yaml(const std::string& contents) {
        auto lines = String::split(contents, '\n');
        return parse_block(lines, 0, lines.size(), 0);
    }

    int num_leading_spaces(const std::string& str) {
        int count = 0;
        while (count < static_cast<int>(str.size()) && str[count] == ' ') {
            count++;
        }
        return count;
    }

    ConfigMap Config::parse_block(const std::vector<std::string>& lines, int begin, int end, int indent) {
        int cur_index = begin;
        ConfigMap map{};

        while (cur_index < end) {
            auto& line = lines[cur_index];
            if (line.empty()) {
                ++cur_index;
                continue;
            }
            auto line_indent = num_leading_spaces(line);
            if (line_indent < indent) {
                break;
            }
            auto content = line.substr(indent);
            size_t colon_pos = content.find(':');
            if (colon_pos == std::string::npos) {
                throw std::runtime_error("YAML parsing error. Expected colon in line: " + line);
            }
            auto key = String::trim(content.substr(0, colon_pos));
            auto value = String::trim(content.substr(colon_pos + 1));
            if (!value.empty()) {
                map[key] = ConfigNode(value);
                ++cur_index;
            } else {
                int next_index = cur_index + 1;
                while (next_index < end) {
                    if (lines[next_index].empty()) {
                        ++next_index;
                    } else {
                        break;
                    }
                }
                if (next_index >= end) {
                    map[key] = ConfigNode(ConfigMap{});
                    cur_index = next_index;
                    break;
                }
                int next_line_indent = num_leading_spaces(lines[next_index]);
                if (next_line_indent <= indent) {
                    map[key] = ConfigNode(ConfigMap{});
                    cur_index = next_index;
                } else {
                    int next_end_index = next_index;
                    while (next_end_index < end) {
                        if (num_leading_spaces(lines[next_end_index]) < next_line_indent) {
                            break;
                        }
                        ++next_end_index;
                    }
                    ConfigMap nested_map = Config::parse_block(lines, next_index, next_end_index, next_line_indent);
                    map[key] = ConfigNode(std::move(nested_map));
                    cur_index = next_end_index;
                }
            }
        }
        return map;
    }

    std::string Config::get(const std::string key) const {
        auto subkeys = String::split(key, '/');
        auto node = map_.at(subkeys.front());
        for (size_t i = 1; i < subkeys.size(); ++i) {
            if (std::holds_alternative<std::shared_ptr<ConfigMap>>(node.value)) {
                auto next_map = std::get<std::shared_ptr<ConfigMap>>(node.value);
                node = next_map->at(subkeys[i]);
            } else {
                throw std::runtime_error("Cannot get key " + key + " from config");
            }
        }
        return std::get<std::string>(node.value);
    }
}