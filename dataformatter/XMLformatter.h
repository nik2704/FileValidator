#pragma once

#include <string>
#include <libxml/xmlschemas.h>
#include "DataFormatter.h"


namespace validator {

class XmlFormatter : public DataFormatter {
public:
    explicit XmlFormatter(const InputValues& input);

    bool validate() override;
    std::string format() const override;
    void saveToFile() override;

    XmlFormatter(const XmlFormatter&) = delete;
    XmlFormatter& operator=(const XmlFormatter&) = delete;

private:
    bool validateWithSchema();
    xmlNodePtr findElement(xmlNodePtr root, const std::string& elementName) const;
    bool validateElement(const xmlNodePtr & root) const;
    bool validateAttribute(const xmlNodePtr & someElement) const;
};

} // namespace validator
