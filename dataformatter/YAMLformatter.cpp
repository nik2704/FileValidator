#include <string>
#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include "YAMLformatter.h"

namespace validator {

YamlFormatter::YamlFormatter(const InputValues& input) : DataFormatter(input) {}

bool YamlFormatter::pre_validate() {
    int quote_count = 0;
    for (char c : data_) {
        if (c == '"') quote_count++;
    }
    if (quote_count % 2 != 0) {
        std::cout << "YAML pre-validation: не закрытая кавычка." << std::endl;
        return false;
    }
    return true;
}

bool YamlFormatter::validate() {
    if (!pre_validate()) {
        return false;
    }

    try {
        YAML::Node yaml_data = YAML::Load(data_);
        return true;
    } catch (const YAML::ParserException& e) {
        std::cout << "Ошибка валидации: " << e.what() << std::endl;
        return false;
    }
}

std::string YamlFormatter::format() const {
    try {
        YAML::Node yaml_data = YAML::Load(data_);
        YAML::Emitter out;
        out.SetIndent(4);
        out << yaml_data;
        return out.c_str();
    } catch (const YAML::ParserException& e) {
        std::cout << "Ошибка форматирования: " << e.what() << std::endl;
        return "{}";
    }
}

void YamlFormatter::saveToFile() {
    try {
        YAML::Node yaml_data = YAML::Load(data_);
        std::ofstream file(target_file_);
        if (file.is_open()) {
            YAML::Emitter out;
            out.SetIndent(4);
            out << yaml_data;
            file << out.c_str();
            file.close();
        } else {
            std::cout << "Ошибка чтения файла: " << target_file_ << std::endl;
        }
    } catch (const YAML::ParserException& e) {
        std::cout << "Ошибка записи в файл: " << e.what() << std::endl;
    }
}

} // namespace validator
