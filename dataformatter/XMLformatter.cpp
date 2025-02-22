#include <iostream>
#include <fstream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemas.h>

#include "XMLformatter.h"

namespace validator {

XmlFormatter::XmlFormatter(const InputValues& input) : DataFormatter(input) {}

bool XmlFormatter::validate() {
    xmlDocPtr doc = xmlReadMemory(data_.c_str(), data_.size(), "noname.xml", nullptr, 0);
    if (!doc) {
        std::cerr << "Ошибка при парсинге XML.\n";
        return false;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (!root) {
        std::cerr << "Ошибка: Отсутствует корневой элемент XML.\n";
        return false;
    }    

    if (!validateElement(root)) {
        return false;
    }

    xmlFreeDoc(doc);
    return true;
}

std::string XmlFormatter::format() const {
    xmlDocPtr doc = xmlReadMemory(data_.c_str(), data_.size(), "noname.xml", nullptr, XML_PARSE_NOBLANKS);
    if (!doc) {
        std::cerr << "Ошибка при парсинге XML для форматирования.\n";
        return "";
    }

    xmlChar* xmlbuff;
    int buffersize;
    xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);
    std::string formattedXml(reinterpret_cast<char*>(xmlbuff), buffersize);

    xmlFree(xmlbuff);
    xmlFreeDoc(doc);

    return formattedXml;
}

void XmlFormatter::saveToFile() {
    xmlDocPtr doc = xmlReadMemory(data_.c_str(), data_.size(), "noname.xml", nullptr, 0);
    if (!doc) {
        std::cerr << "Ошибка при парсинге XML для сохранения.\n";
        return;
    }

    if (xmlSaveFormatFileEnc(target_file_.c_str(), doc, "UTF-8", 1) == -1) {
        std::cerr << "Ошибка при сохранении XML в файл: " << target_file_ << "\n";
    }

    xmlFreeDoc(doc);
}

xmlNodePtr XmlFormatter::findElement(xmlNodePtr root, const std::string& elementName) const {
    for (xmlNodePtr node = root->children; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE && xmlStrEqual(node->name, BAD_CAST elementName.c_str())) {
            return node;
        }
    }
    return nullptr;
}

bool XmlFormatter::validateElement(const xmlNodePtr & root) const {
    if (xml_element_name_.empty()) {
        return true;
    }

    xmlNodePtr someElement = findElement(root, xml_element_name_);
    if (!someElement) {
        std::cerr << "Ошибка: Отсутствует элемент <" << xml_element_name_ << ">.\n";
        return false;
    }

    return validateAttribute(someElement);
}

bool XmlFormatter::validateAttribute(const xmlNodePtr & someElement) const {
    if (xml_attribute_name_.empty()) {
        return true;
    }

    xmlChar* attr = xmlGetProp(someElement, BAD_CAST xml_attribute_name_.c_str());
    if (!attr) {
        std::cerr << "Ошибка: Отсутствует обязательный атрибут '" << xml_attribute_name_ << "'.\n";
        return false;
    }

    xmlFree(attr);
    return true;
}

bool XmlFormatter::validateWithSchema() {
    xmlDocPtr doc = xmlReadMemory(data_.c_str(), data_.size(), "noname.xml", nullptr, 0);
    if (!doc) {
        std::cerr << "Ошибка при парсинге XML для валидации по схеме.\n";
        return false;
    }

    xmlSchemaParserCtxtPtr schemaParserCtxt = xmlSchemaNewParserCtxt(xml_dtd_file_name_.c_str());
    if (!schemaParserCtxt) {
        std::cerr << "Не удалось создать контекст парсера для схемы: " << xml_dtd_file_name_ << "\n";
        xmlFreeDoc(doc);
        return false;
    }

    xmlSchemaPtr schema = xmlSchemaParse(schemaParserCtxt);
    if (!schema) {
        std::cerr << "Ошибка при парсинге XSD схемы.\n";
        xmlSchemaFreeParserCtxt(schemaParserCtxt);
        xmlFreeDoc(doc);
        return false;
    }

    xmlSchemaValidCtxtPtr validCtxt = xmlSchemaNewValidCtxt(schema);
    if (!validCtxt) {
        std::cerr << "Не удалось создать контекст для валидации.\n";
        xmlSchemaFree(schema);
        xmlSchemaFreeParserCtxt(schemaParserCtxt);
        xmlFreeDoc(doc);
        return false;
    }

    int ret = xmlSchemaValidateDoc(validCtxt, doc);
    if (ret != 0) {
        std::cerr << "XML не соответствует XSD схеме.\n";
    } else {
        std::cout << "XML валиден относительно схемы.\n";
    }

    xmlSchemaFreeValidCtxt(validCtxt);
    xmlSchemaFree(schema);
    xmlSchemaFreeParserCtxt(schemaParserCtxt);
    xmlFreeDoc(doc);

    return ret == 0;
}

}  // namespace validator
