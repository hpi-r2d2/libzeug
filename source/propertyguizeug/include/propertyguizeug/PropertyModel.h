#pragma once

#include <QAbstractItemModel>

#include <propertyguizeug/propertyguizeug_api.h>

namespace reflectionzeug 
{
    class AbstractProperty;
    class PropertyGroup;
}

namespace propertyguizeug
{

class PropertyItem;  

/** \brief Property model for Qt's Model-View-Controller framework
 *
 * Supported options
 * - title (std::string): used instead of property name if set
 * - tooltip (std::string): tooltip to be shown when hovering over property row
 * - disabled (bool): disables the property row and all children
 * - readonly (bool): makes the property readonly and all children
 */
class PROPERTYGUIZEUG_API PropertyModel : public QAbstractItemModel
{
public:
    PropertyModel(reflectionzeug::PropertyGroup * group, QObject * parent = nullptr);
    virtual ~PropertyModel();

    reflectionzeug::AbstractProperty * propertyAt(const QModelIndex & index) const;

    virtual QModelIndex index(int row, int column,
                              const QModelIndex & parentIndex = QModelIndex()) const;
    
    virtual QModelIndex parent(const QModelIndex & index) const;
    
    virtual int rowCount(const QModelIndex & parentIndex = QModelIndex()) const;
    
    virtual int columnCount(const QModelIndex & parentIndex = QModelIndex()) const;
    
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;

    void onValueChanged(PropertyItem * item);

    void onBeforeAdd(PropertyItem * item, 
                     size_t position, 
                     reflectionzeug::AbstractProperty * property);
    void onAfterAdd();

    void onBeforeRemove(PropertyItem * item, 
                        size_t position);
    void onAfterRemove();

private:
    static void addChildren(PropertyItem * item, 
                            reflectionzeug::PropertyGroup * group);
    
    static void addChild(PropertyItem * item, 
                         reflectionzeug::AbstractProperty * property);

    QModelIndex createIndex(PropertyItem * item, int column = 0) const;
    
private:
    PropertyItem * m_root;
};

} // namespace propertyguizeug
