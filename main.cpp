#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>

#include "dataformatter/DataFormatter.h"
#include "./utils/utils.h"

namespace po = boost::program_options;

using namespace validator;

/**
 * @brief Главная точка входа в программу.
 *
 * Программа проверяет и форматирует файлы в форматах JSON, XML и YAML.
 * Использует Boost Program Options для обработки аргументов командной строки.
 *
 * Допустимые аргументы:
 *  -c, --check: Тип проверки (check-format или format)
 *  -d, --dtd: Имя DTD файла для XML
 *  -f, --format: Формат файла (json, xml, yaml, yml)
 *  -s, --input: Имя входного файла
 *  -o, --output: Вывод результата (console или file)
 *  -t, --target: Целевой файл (если вывод в файл)
 *  -e, --xml_element: Имя элемента (только для XML)
 *  -a, --xml_attr: Имя атрибута (только для XML)
 *  -h, --help: Показать справку
 */
int main(int argc, char* argv[]) {
    try {
        // Создаем объект с начальными значениями для входных данных
        auto input_values = create_default_input_values();

        // Описание допустимых опций командной строки
        po::options_description desc("Допустимые опции", 120);
        desc.add_options()
            ("check,c", po::value<std::string>(&input_values->check_type)->default_value("format"), "Тип проверки (check-format или format)")
            ("dtd,d", po::value<std::string>(&input_values->xml_dtd_file_name), "Имя файла формата (только для XML)")
            ("format,f", po::value<std::string>(&input_values->format)->default_value("json"), "Формат файла (json, xml, yaml, yml)")
            ("input,s", po::value<std::string>(&input_values->input_file)->required(), "Имя входного файла")
            ("output,o", po::value<std::string>(&input_values->output)->default_value("console"), "Вывод результата (console или file)")
            ("target,t", po::value<std::string>(&input_values->target_file), "Целевой файл (если вывод в файл)")
            ("xml_element,e", po::value<std::string>(&input_values->xml_element_name), "Имя элемента (только для XML)")
            ("xml_attr,a", po::value<std::string>(&input_values->xml_attribute_name), "Имя атрибута (только для XML)")
            ("help,h", "Помощь");

        // Парсим аргументы командной строки
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        // Выводим справку при необходимости
        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }

        // Проверяем обязательные параметры
        po::notify(vm);

        // Валидируем входные параметры
        auto validation_result = validate_input_values(*input_values);

        if (validation_result->result != 0) {
            std::cout << "Ошибка: " << validation_result->message << "\n";
            return 1;
        }

        // Обрабатываем файл согласно введенным параметрам
        process(*input_values);

    } catch (const std::exception& e) {
        // Обработка исключений и вывод ошибки
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }

    return 0;
} // Конец main.cpp
