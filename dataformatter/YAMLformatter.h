#pragma once

#include <string>
#include "DataFormatter.h"


namespace validator {

class YamlFormatter : public DataFormatter {
public:
    explicit YamlFormatter(const InputValues& input);

    bool validate() override;
    std::string format() const override;
    void saveToFile() override;

    YamlFormatter(const YamlFormatter&) = delete;
    YamlFormatter& operator=(const YamlFormatter&) = delete;

private:
    bool pre_validate();
};

} // namespace validator
