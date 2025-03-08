#include <memory>
#include <string>

#include "../dataformatter/DataFormatter.h"
#include "../dataformatter/JSONformatter.h"
#include "../dataformatter/XMLformatter.h"
#include "utils.h"

namespace validator {

void process(const InputValues& input) {
    auto fmt = get_formatter(input);
    bool print = true;

    if (input.check_type == "check-format") {
        if (fmt->validate()) {
            std::cout << "Файл " << input.format << " валиден." << std::endl;
        } else {
            std::cout << "Файл " << input.format << " невалиден." << std::endl;
            print = false;
        }
    }

    if (print) {
        if (input.check_type == "format" || input.output == "console") {
            std::cout << fmt->format() << std::endl;
        }

        if (input.output == "file" && !input.target_file.empty()) {
            fmt->saveToFile();
        }            
    }
}

std::unique_ptr<DataFormatter> get_formatter(const InputValues& input) {
    if (input.format == "xml") {
        return std::make_unique<XmlFormatter>(input);
    }

    return std::make_unique<JsonFormatter>(input);
}

std::unique_ptr<InputValues> create_default_input_values() {
    return std::make_unique<InputValues>(InputValues {
        "json", "", "format", "console", "", "", "", ""
    });
};

std::unique_ptr<ValidationResult> validate_output(const InputValues& input) {
    if (input.output == "file" && input.target_file.empty()) {
        return std::make_unique<ValidationResult>(ValidationResult{
            "необходимо указать целевой файл с помощью --target или -t при выборе вывода в файл.", 1
        });
    }
    return std::make_unique<ValidationResult>(ValidationResult{"", 0});
}

std::unique_ptr<ValidationResult> validate_format(const InputValues& input) {
    if (input.format != "json" && input.format != "xml") {
        return std::make_unique<ValidationResult>(ValidationResult{
            "неподдерживаемый формат файла. Допустимые значения: json, xml.", 1
        });
    }
    return std::make_unique<ValidationResult>(ValidationResult{"", 0});
}

std::unique_ptr<ValidationResult> validate_check_type(const InputValues& input) {
    if (input.check_type != "check-format" && input.check_type != "format") {
        return std::make_unique<ValidationResult>(ValidationResult{
            "неподдерживаемый тип проверки. Допустимые значения: check-format, format.", 1
        });
    }
    return std::make_unique<ValidationResult>(ValidationResult{"", 0});
}

std::unique_ptr<ValidationResult> validate_input_values(const InputValues& input) {
    auto output_result = validate_output(input);
    if (output_result->result != 0) return output_result;

    auto format_result = validate_format(input);
    if (format_result->result != 0) return format_result;

    auto check_type_result = validate_check_type(input);
    if (check_type_result->result != 0) return check_type_result;

    return std::make_unique<ValidationResult>(ValidationResult{"Все проверки пройдены успешно.", 0});
}

}