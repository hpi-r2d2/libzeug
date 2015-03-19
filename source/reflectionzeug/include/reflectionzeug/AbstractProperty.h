#pragma once

#include <string>
#include <set>

#include <reflectionzeug/reflectionzeug_api.h>
#include <reflectionzeug/Variant.h>
#include <signalzeug/Signal.h>

namespace reflectionzeug
{

class AbstractValueProperty;
class AbstractPropertyCollection;
class PropertyGroup;

/** 
 * \brief The base class of all properties.
 *
 * When added to a PropertyGroup, the name is used as the unique key.
 * Do not change its name when it's in one or more groups.
 * All subclasses use virtual inheritance.
 *
 * \see PropertyGroup
 * \ingroup property_hierarchy
 */
class REFLECTIONZEUG_API AbstractProperty 
{
public:
    static const std::string s_nameRegexString;

public:
    AbstractProperty();
    AbstractProperty(const std::string & name);
    
    virtual ~AbstractProperty() = 0;

    virtual Variant toVariant() const = 0;
    virtual bool fromVariant(const Variant & value) = 0;

    std::string name() const;
    bool setName(const std::string & name);
    bool hasName() const;
    
    /**
     * \name Additional meta informations 
     * Add additional meta information to your properties.
     * Use options to add, e.g., minimum and maximum values, 
     * affixes or flags. These can be used to configure editor widgets.
     */
    /** \{ */
    
    template <typename T>
    T option(const std::string & key, const T & defaultValue) const;

    Variant option(const std::string & key) const;
    void setOption(const std::string & key, const Variant & value);
    void setOptions(const VariantMap & map);
    bool removeOption(const std::string & key);
    bool hasOption(const std::string & key) const;

    const VariantMap & options() const;

    /** \} */
    
    /**
     * \name Convenience casting methods
     * Use them, when you need to cast to sub classes.
     * All casting is done with dynamic casts.
     */
    /** \{ */
    
    template <class Property>
    Property * as();
    
    template <class Property>
    const Property * as() const;

    AbstractValueProperty * asValue();
    const AbstractValueProperty * asValue() const;

    AbstractPropertyCollection * asCollection();
    const AbstractPropertyCollection * asCollection() const;

    PropertyGroup * asGroup();
    const PropertyGroup * asGroup() const;

    virtual bool isCollection() const;
    virtual bool isValue() const;
    virtual bool isGroup() const;
    
    /** \} */

public:
    signalzeug::Signal<const std::string &> optionChanged;
    
private:
    std::string m_name;
    VariantMap m_options;
};
    
} // namespace reflectionzeug

#include <reflectionzeug/AbstractProperty.hpp>
