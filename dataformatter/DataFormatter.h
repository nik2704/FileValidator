#pragma once

#include <string>
#include <fstream>
#include <iostream>

namespace validator {

/**
 * @brief Структура для хранения входных параметров.
 */
struct InputValues {
    std::string format;             ///< Формат файла (json, xml, yaml)
    std::string input_file;         ///< Имя входного файла
    std::string check_type;         ///< Тип проверки
    std::string output;             ///< Тип вывода (console или file)
    std::string target_file;        ///< Целевой файл для вывода
    std::string xml_dtd_file_name;  ///< Имя DTD файла для XML
    std::string xml_element_name;   ///< Имя XML элемента
    std::string xml_attribute_name; ///< Имя XML атрибута
};

/**
 * @brief Абстрактный базовый класс для форматов.
 */
class DataFormatter {
public:
    explicit DataFormatter(const InputValues& input) {
        loadFromFile(input.input_file);

        format_ = input.format;
        input_file_ = input.input_file;
        check_type_ = input.check_type;
        output_ = input.output;
        target_file_ = input.target_file;
        xml_dtd_file_name_ = input.xml_dtd_file_name;
        xml_element_name_ = input.xml_element_name;
        xml_attribute_name_ = input.xml_attribute_name;
    }

    virtual ~DataFormatter() {}

    DataFormatter(const DataFormatter&) = delete;
    DataFormatter& operator=(const DataFormatter&) = delete;

    /**
     * @brief Проверяет валидность данных.
     * @return true если данные валидны, иначе false.
     */
    virtual bool validate() = 0;

    /**
     * @brief Форматирует данные.
     * @return Отформатированные данные в виде строки.
     */
    virtual std::string format() const = 0;

    /**
     * @brief Сохраняет отформатированные данные в файл.
     */
    virtual void saveToFile() = 0;

    friend std::ostream& operator<<(std::ostream& os, const DataFormatter& formatter) {
        os << formatter.format();
        return os;
    }

protected:
    std::string data_;               ///< Сырые данные из файла
    std::string format_;             ///< Формат данных
    std::string input_file_;         ///< Имя входного файла
    std::string check_type_;         ///< Тип проверки
    std::string output_;             ///< Тип вывода
    std::string target_file_;        ///< Имя целевого файла
    std::string xml_dtd_file_name_;  ///< Имя DTD файла для XML
    std::string xml_element_name_;   ///< Имя XML элемента
    std::string xml_attribute_name_; ///< Имя XML атрибута

private:
    /**
     * @brief Загружает данные из файла.
     * @param filename Имя файла.
     * @return true если файл загружен успешно, иначе false.
     */
    bool loadFromFile(const std::string& filename) {
        std::ifstream input_file(filename);

        if (!input_file) {
            std::cerr << "Ошибка при открытии файла: " << filename << "\n";
            return false;
        }

        data_ = std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());

        return true;
    }
};

} // namespace validator
