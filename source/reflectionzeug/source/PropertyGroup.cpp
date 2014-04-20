
#include <reflectionzeug/PropertyGroup.h>

#ifdef USE_STD_REGEX
    #include <regex>

    namespace regex_namespace = std;
#else
    #include <boost/regex.hpp>

    namespace regex_namespace = boost;
#endif

#include <cassert>
#include <algorithm>


namespace reflectionzeug
{
    
PropertyGroup::PropertyGroup(const std::string & name)
:   AbstractProperty(name)
{
}

PropertyGroup::~PropertyGroup()
{
    for (AbstractProperty * property : m_properties)
        delete property;
}

bool PropertyGroup::isGroup() const
{
    return true;
}

bool PropertyGroup::addProperty(AbstractProperty * property)
{
    if (this->propertyExists(property->name()) || property->hasParent())
        return false;
    
    m_properties.push_back(property);
    m_propertiesMap.insert(std::make_pair(property->name(), property));
    property->setParent(this);
    return true;
}

PropertyGroup * PropertyGroup::addGroup(const std::string & name)
{
    PropertyGroup * group = new PropertyGroup(name);
    
    if (!this->addProperty(group)) {
        delete group;
        return nullptr;
    }
    
    return group;
}

AbstractProperty * PropertyGroup::property(const std::string & path)
{
    std::vector<std::string> splittedPath = util::split(path, '/');
    return const_cast<AbstractProperty *>(findProperty(splittedPath));
}
    
const AbstractProperty * PropertyGroup::property(const std::string & path) const
{
    std::vector<std::string> splittedPath = util::split(path, '/');
    return findProperty(splittedPath);
}

PropertyGroup * PropertyGroup::group(const std::string & path)
{
    return this->property(path)->as<PropertyGroup>();
}

const PropertyGroup * PropertyGroup::group(const std::string & path) const
{
    return this->property(path)->as<PropertyGroup>();
}
    
PropertyGroup * PropertyGroup::ensureGroup(const std::string & path)
{
    std::vector<std::string> splittedPath = util::split(path, '/');
    return ensureGroup(splittedPath);
}
    
AbstractProperty * PropertyGroup::at(size_t index)
{
    if (index >= this->count())
        return nullptr;
    
    return m_properties[index];
}

const AbstractProperty * PropertyGroup::at(size_t index) const
{
    if (index >= this->count())
        return nullptr;
    
    return m_properties[index];
}

bool PropertyGroup::propertyExists(const std::string & name) const
{
    return m_propertiesMap.find(name) != m_propertiesMap.end();
}

bool PropertyGroup::groupExists(const std::string & name) const
{
    return this->propertyExists(name) && m_propertiesMap.at(name)->isGroup();
}

bool PropertyGroup::removeProperty(AbstractProperty * property)
{
    if (!this->propertyExists(property->name()))
        return false;
    
    auto propertyIt = std::find(m_properties.begin(), m_properties.end(), property);
    m_properties.erase(propertyIt);
    m_propertiesMap.erase(property->name());
    return true;
}

AbstractProperty * PropertyGroup::takeProperty(const std::string & name)
{
    if (!this->propertyExists(name))
        return nullptr;
    
    AbstractProperty * property = m_propertiesMap.at(name);
    auto propertyIt = std::find(m_properties.begin(), m_properties.end(), property);
    m_properties.erase(propertyIt);
    m_propertiesMap.erase(property->name());
    property->removeParent();
    return property;
}

size_t PropertyGroup::count() const
{
    return m_properties.size();
}
    
int PropertyGroup::indexOf(const AbstractProperty * property) const
{
    assert(property);

    if (!this->propertyExists(property->name()))
        return -1;

    return std::distance(m_properties.begin(), std::find(m_properties.begin(), m_properties.end(), property));
}
    
bool PropertyGroup::isEmpty() const
{
    return this->count() == 0;
}

void PropertyGroup::forEach(const std::function<void(AbstractProperty &)> & functor)
{
    for (AbstractProperty * property : m_properties)
        functor(*property);
}

void PropertyGroup::forEach(const std::function<void(const AbstractProperty &)> & functor) const
{
    for (AbstractProperty * property : m_properties)
        functor(*property);
}

void PropertyGroup::forEachValue(const std::function<void(AbstractValueProperty &)> & functor)
{
    for (AbstractProperty * property : m_properties) {
        if (property->isValue())
            functor(*property->asValue());
    }
}

void PropertyGroup::forEachValue(const std::function<void(const AbstractValueProperty &)> & functor) const
{
    for (const AbstractProperty * property : m_properties) {
        if (property->isValue())
            functor(*property->asValue());
    }
}

void PropertyGroup::forEachCollection(const std::function<void(AbstractPropertyCollection &)> & functor)
{
    for (AbstractProperty * property : m_properties) {
        if (property->isCollection())
            functor(*property->asCollection());
    }
}

void PropertyGroup::forEachCollection(const std::function<void(const AbstractPropertyCollection &)> & functor) const
{
    for (const AbstractProperty * property : m_properties) {
        if (property->isCollection())
            functor(*property->asCollection());
    }
}

void PropertyGroup::forEachGroup(const std::function<void(PropertyGroup &)> & functor)
{
    for (AbstractProperty * property : m_properties) {
        if (property->isGroup())
            functor(*property->as<PropertyGroup>());
    }
}

void PropertyGroup::forEachGroup(const std::function<void(const PropertyGroup &)> & functor) const
{
    for (const AbstractProperty * property : m_properties) {
        if (property->isGroup())
            functor(*property->as<PropertyGroup>());
    }
}

const AbstractProperty * PropertyGroup::findProperty(const std::vector<std::string> & path) const
{
    if (path.size() == 0)
        return nullptr;
    
    if (!propertyExists(path.front()))
        return nullptr;
    
    AbstractProperty * property = m_propertiesMap.at(path.front());
    
    if (path.size() == 1)
        return property;
    
    if (!property->isGroup())
        return nullptr;
    
    return property->asGroup()->findProperty({ path.begin() + 1, path.end() });
}
    
PropertyGroup * PropertyGroup::ensureGroup(const std::vector<std::string> & path)
{
    if (path.size() == 0)
        return nullptr;
    
    PropertyGroup * group;
    if (propertyExists(path.front()))
    {
        AbstractProperty * property = m_propertiesMap.at(path.front());
        if (!property->isGroup())
            return nullptr;
        group = property->asGroup();
    }
    else
    {
        group = addGroup(path.front());
    }
    
    if (path.size() == 1)
        return group;
    else
        return group->ensureGroup(std::vector<std::string>(path.begin() + 1, path.end()));
}
    
} // namespace reflectionzeug
