#pragma once

#include <sstream>
#include <type_traits>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <reflectionzeug/Property.h>

namespace reflectionzeug
{

template <typename T, unsigned Count>
std::string glmToString(const T * data)
{
    std::stringstream ss;

    ss << "(";

    for (unsigned i = 0; i < Count; ++i)
    {
        if (i > 0)
            ss << ", ";

        ss << data[i];
    }

    ss << ")";

    return ss.str();
}

template <typename T, unsigned Count>
bool glmFromString(const std::string & string, T * data)
{
    std::string elementRegex = std::is_integral<T>::value ? "(-|\\+)?\\d+" : "(-|\\+)?\\d+\\.?\\d*";

    std::stringstream ss;
    ss << "\\s*\\(";
    for (unsigned i=0; i<Count-1; ++i)
    {
        ss << "(" << elementRegex << ")";
        ss << "\\s*,\\s*";
    }
    ss << elementRegex << "\\)\\s*";

    if (!reflectionzeug::util::matchesRegex(string, ss.str()))
        return false;

    std::vector<std::string> parts = reflectionzeug::util::extract(string, elementRegex);

    assert(parts.size() == Count);

    for (unsigned i = 0; i < Count; ++i)
    {
        const std::string & part = parts[i];
        data[i] = std::is_integral<T>::value ? static_cast<T>(std::stoi(part)) : static_cast<T>(std::stod(part));
    }

    return true;
}

class Vec2Property : public reflectionzeug::ValueProperty<glm::vec2>
{
public:
    using Type = glm::vec2;
    
public:
    template <typename... Arguments>
    Vec2Property(Arguments&&... args)
    : reflectionzeug::ValueProperty<glm::vec2>(std::forward<Arguments>(args)...) {}

    virtual std::string toString() const override 
    {
        return glmToString<glm::vec2::value_type, 2>(glm::value_ptr(this->value())); 
    }

    virtual bool fromString(const std::string & string) override
    {
        glm::vec2 value;
        if (!glmFromString<glm::vec2::value_type, 2>(string, glm::value_ptr(value)))
            return false;

        setValue(value);
        return true;
    }
};

class IVec2Property : public reflectionzeug::ValueProperty<glm::ivec2>
{
public:
    using Type = glm::ivec2;
    
public:
    template <typename... Arguments>
    IVec2Property(Arguments&&... args)
    : reflectionzeug::ValueProperty<glm::ivec2>(std::forward<Arguments>(args)...) {}

    virtual std::string toString() const override 
    { 
        return glmToString<glm::ivec2::value_type, 2>(glm::value_ptr(value())); 
    }

    virtual bool fromString(const std::string & string) override
    {
        glm::ivec2 value;
        if (!glmFromString<glm::ivec2::value_type, 2>(string, glm::value_ptr(value)))
            return false;

        setValue(value);
        return true;
    }
};

class Vec3Property : public reflectionzeug::ValueProperty<glm::vec3>
{
public:
    using Type = glm::vec3;
    
public:
    template <typename... Arguments>
    Vec3Property(Arguments&&... args)
    : reflectionzeug::ValueProperty<glm::vec3>(std::forward<Arguments>(args)...) {}

    virtual std::string toString() const override 
    { 
        return glmToString<glm::vec3::value_type, 3>(glm::value_ptr(value()));
    }

    virtual bool fromString(const std::string & string) override
    {
        glm::vec3 value;
        if (!glmFromString<glm::vec3::value_type, 3>(string, glm::value_ptr(value)))
            return false;

        setValue(value);
        return true;
    }
};

class IVec3Property : public reflectionzeug::ValueProperty<glm::ivec3>
{
public:
    using Type = glm::ivec3;
    
public:
    template <typename... Arguments>
    IVec3Property(Arguments&&... args)
    : reflectionzeug::ValueProperty<glm::ivec3>(std::forward<Arguments>(args)...) {}

    virtual std::string toString() const override 
    { 
        return glmToString<glm::ivec3::value_type, 3>(glm::value_ptr(value())); 
    }

    virtual bool fromString(const std::string & string) override
    {
        glm::ivec3 value;
        if (!glmFromString<glm::ivec3::value_type, 3>(string, glm::value_ptr(value)))
            return false;

        setValue(value);
        return true;
    }
};

class Vec4Property : public reflectionzeug::ValueProperty<glm::vec4>
{
public:
    using Type = glm::vec4;
    
public:
    template <typename... Arguments>
    Vec4Property(Arguments&&... args)
    : reflectionzeug::ValueProperty<glm::vec4>(std::forward<Arguments>(args)...) {}

    virtual std::string toString() const override 
    { 
        return glmToString<glm::vec4::value_type, 4>(glm::value_ptr(value())); 
    }

    virtual bool fromString(const std::string & string) override
    {
        glm::vec4 value;
        if (!glmFromString<glm::vec4::value_type, 4>(string, glm::value_ptr(value)))
            return false;

        setValue(value);
        return true;
    }
};

class IVec4Property : public reflectionzeug::ValueProperty<glm::ivec4>
{
public:
    using Type = glm::ivec4;
    
public:
    template <typename... Arguments>
    IVec4Property(Arguments&&... args)
    : reflectionzeug::ValueProperty<glm::ivec4>(std::forward<Arguments>(args)...) {}

    virtual std::string toString() const override 
    { 
        return glmToString<glm::ivec4::value_type, 4>(glm::value_ptr(value())); 
    }

    virtual bool fromString(const std::string & string) override
    {
        glm::ivec4 value;
        if (!glmFromString<glm::ivec4::value_type, 4>(string, glm::value_ptr(value)))
            return false;

        setValue(value);
        return true;
    }
};

template <>
struct PropertyClass<Vec2Property::Type>
{
    using Type = Vec2Property;
};

template <>
struct PropertyClass<IVec2Property::Type>
{
    using Type = IVec2Property;
};

template <>
struct PropertyClass<Vec3Property::Type>
{
    using Type = Vec3Property;
};

template <>
struct PropertyClass<IVec3Property::Type>
{
    using Type = IVec3Property;
};

template <>
struct PropertyClass<Vec4Property::Type>
{
    using Type = Vec4Property;
};

template <>
struct PropertyClass<IVec4Property::Type>
{
    using Type = IVec4Property;
};

} // namespace reflectionzeug
