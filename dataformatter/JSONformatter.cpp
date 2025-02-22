#include <string>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "JSONformatter.h"


namespace validator {

JsonFormatter::JsonFormatter(const InputValues& input) : DataFormatter(input) {}

bool JsonFormatter::validate() {
    try {
        [[maybe_unused]] auto parsed_data = nlohmann::json::parse(data_);
        std::cout << "JSON is valid." << std::endl;
        return true;
    } catch (const nlohmann::json::parse_error& e) {
        std::cout << "JSON validation failed: " << e.what() << std::endl;
        return false;
    }
}
 
std::string JsonFormatter::format() const {
    try {
        nlohmann::json json_data = nlohmann::json::parse(data_);
        return json_data.dump(4);
    } catch (const nlohmann::json::parse_error& e) {
        std::cout << "Error while formatting: " << e.what() << std::endl;
        return "{}";
    }
}

void JsonFormatter::saveToFile() {
    try {
        nlohmann::json json_data = nlohmann::json::parse(data_);
        std::ofstream file(target_file_);
        if (file.is_open()) {
            file << json_data.dump(4);
            file.close();
            std::cout << "Data saved to " << target_file_ << std::endl;
        } else {
            std::cout << "Error opening file: " << target_file_ << std::endl;
        }
    } catch (const nlohmann::json::parse_error& e) {
        std::cout << "Error while saving to file: " << e.what() << std::endl;
    }
}

} // namespace validator