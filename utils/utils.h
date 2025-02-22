#pragma once

#include <memory>
#include <string>

#include "../dataformatter/DataFormatter.h"
#include "../dataformatter/JSONformatter.h"

namespace validator {

/**
 * @brief Результат валидации.
 *
 * Хранит сообщение об ошибке и код результата.
 */
struct ValidationResult {
    std::string message;  ///< Сообщение об ошибке
    int result;           ///< Код результата (0 - успех, иное - ошибка)
};

/**
 * @brief Основная логика обработки ввода.
 * @param input Структура с параметрами ввода.
 */
void process(const InputValues& input);

/**
 * @brief Создает объект форматора для указанного формата.
 * @param input Параметры ввода.
 * @return Указатель на объект форматора.
 */
std::unique_ptr<DataFormatter> get_formatter(const InputValues& input);

/**
 * @brief Создает объект InputValues с начальными значениями.
 * @return Указатель на созданный объект.
 */
std::unique_ptr<InputValues> create_default_input_values();

/**
 * @brief Валидирует параметр вывода.
 * @param input Параметры ввода.
 * @return Результат валидации.
 */
std::unique_ptr<ValidationResult> validate_output(const InputValues& input);

/**
 * @brief Валидирует формат ввода.
 * @param input Параметры ввода.
 * @return Результат валидации.
 */
std::unique_ptr<ValidationResult> validate_format(const InputValues& input);

/**
 * @brief Валидирует тип проверки.
 * @param input Параметры ввода.
 * @return Результат валидации.
 */
std::unique_ptr<ValidationResult> validate_check_type(const InputValues& input);

/**
 * @brief Валидирует все входные параметры.
 * @param input Параметры ввода.
 * @return Результат валидации.
 */
std::unique_ptr<ValidationResult> validate_input_values(const InputValues& input);

} // namespace validator
