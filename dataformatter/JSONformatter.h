#pragma once

#include <string>
#include "DataFormatter.h"


namespace validator {

class JsonFormatter : public DataFormatter {
public:
    explicit JsonFormatter(const InputValues& input);

    bool validate() override;

    std::string format() const override;

    void saveToFile() override;

    JsonFormatter(const JsonFormatter&) = delete;
    
    JsonFormatter& operator=(const JsonFormatter&) = delete;
};

} // namespace validator