#include <reflectionzeug/JsonPropertySerializer.h>

#include <iostream>

#include <loggingzeug/logging.h>

#include <reflectionzeug/util.h>

using namespace loggingzeug;

namespace reflectionzeug
{
    
JsonPropertySerializer::JsonPropertySerializer()
    : m_nestingLevel(0)
{
}

JsonPropertySerializer::~JsonPropertySerializer()
{
}

void JsonPropertySerializer::serialize(Variant & variant, std::ostream * outStream)
{
    m_outStream = outStream;
    startSerializing(variant);
}

std::string JsonPropertySerializer::serialize(Variant & variant)
{
    if (!m_stringStream.get())
    {
        m_stringStream = std::unique_ptr<std::ostringstream>(new std::ostringstream());
    }
    startSerializing(variant);

    return m_stringStream->str();
}

std::ostream & JsonPropertySerializer::stream()
{
    if (m_outStream)
    {
        return *m_outStream;
    }
    else if(m_stringStream.get())
    {
        return *m_stringStream;
    }
    else
    {
        assert(false);
    }
}

void JsonPropertySerializer::startSerializing(Variant & variant)
{
    m_nestingLevel = 0;
    m_elementCount.push_back(1);

    serializeVariant(variant);

    m_elementCount.clear();
}

void JsonPropertySerializer::serializeVariant(Variant & variant)
{
    if (variant.isMap())
    {
        serializeMap(variant.toMap());
    }
    else if (variant.isArray())
    {
        serializeArray(variant.toArray());
    }
    else
    {
        serializeValue(variant);
    }
}

void JsonPropertySerializer::serializeMap(const VariantMap * map)
{
    stream() << "{" << std::endl;

    m_nestingLevel++;
    m_elementCount.push_back(map->size());
    for (auto stringVariantPair : *map)
    {
        stream() << indent(m_nestingLevel) << "\"" << stringVariantPair.first << "\": ";
        serializeVariant(stringVariantPair.second);
    }
    m_elementCount.pop_back();
    m_nestingLevel--;

    stream() << indent(m_nestingLevel) << "}";
    endLine();
}

void JsonPropertySerializer::serializeArray(const VariantArray * array)
{
    stream() << "[" << std::endl;

    m_nestingLevel++;
    m_elementCount.push_back(array->size());
    for (auto variant : *array)
    {
        serializeVariant(variant);
    }
    m_elementCount.pop_back();
    m_nestingLevel--;

    stream() << indent(m_nestingLevel) << "]";
    endLine();
}
    
void JsonPropertySerializer::serializeValue(Variant & value)
{
    writeJsonString(value);
    endLine();
}

void JsonPropertySerializer::writeJsonString(Variant & value)
{
    if (value.isNull())
    {
        stream() << "null";
    }
    else if (value.hasType<bool>() && value.canConvert<bool>())
    {
        stream() << (value.value<bool>() ? "true" : "false");
    }
    else if ((value.hasType<float>() || value.hasType<double>()) &&
             value.canConvert<double>())
    {
        stream() << value.value<double>();
    }
    else if ((value.hasType<char>() || value.hasType<unsigned char>() ||
             value.hasType<int>() || value.hasType<unsigned int>() ||
             value.hasType<long>() || value.hasType<unsigned long>() ||
             value.hasType<long long>() || value.hasType<unsigned long long>()) &&
             value.canConvert<long long>())
    {
        stream() << value.value<long long>();
    }
    else if (value.canConvert<std::string>())
    {
        stream() << "\"" << value.value<std::string>() << "\"";
    }
    else
    {
        critical() << "Could not serialize value, please register appropriate converter." << std::endl;
        stream() << "null";
    }
}

std::string JsonPropertySerializer::indent(unsigned int nestingLevel)
{
    std::string str;
    for (unsigned int i = 0; i < nestingLevel; i++)
    {
        str += "    ";
    }
    return str;
}

void JsonPropertySerializer::endLine()
{
    m_elementCount[m_nestingLevel]--;
    if (m_elementCount.at(m_nestingLevel) > 0)
    {
        stream() << ",";
    }
    stream() << std::endl;
}

} // namespace reflectionzeug
