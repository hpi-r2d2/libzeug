#include <propertyguizeug/StringEditor.h>

#include <QComboBox>
#include <QLineEdit>

#include <reflectionzeug/Property.h>

#include "util.h"

using namespace reflectionzeug;
namespace propertyguizeug
{
    
StringEditor::StringEditor(StringPropertyInterface * property, QWidget * parent)
:   PropertyEditor{parent}
,   m_property{property}
{   
    QWidget * widget = nullptr;

    if (m_property->hasOption("choices"))
        widget = createComboBox();
    else
        widget = createLineEdit();

    addWidget(widget);
    setFocusProxy(widget);
}

StringEditor::~StringEditor()
{
}

QWidget * StringEditor::createComboBox()
{
    auto comboBox = new QComboBox{this};

    auto choices = m_property->option("choices").value<std::vector<std::string>>();

    comboBox->addItems(util::toQStringList(choices));
    comboBox->setCurrentText(QString::fromStdString(m_property->toString()));
    
    using StringActivatedPtr = void (QComboBox::*) (const QString &);
    connect(comboBox, static_cast<StringActivatedPtr>(&QComboBox::activated),
            this, &StringEditor::setString);

    m_propertyChangedConnection = m_property->valueChanged.connect(
        [this, comboBox]()
        {
            comboBox->setCurrentText(QString::fromStdString(m_property->toString()));
        });

    return comboBox;
}

QWidget * StringEditor::createLineEdit()
{
    auto lineEdit = new QLineEdit{this};
    
    lineEdit->setText(QString::fromStdString(m_property->toString()));
    
    const auto deferred = m_property->option<bool>("deferred", false);

    if (deferred)
    {
        connect(lineEdit, &QLineEdit::editingFinished,
            [this, lineEdit]
        {
            setString(lineEdit->text());
        });
    }
    else
    {
        connect(lineEdit, &QLineEdit::textEdited,
                this, &StringEditor::setString);
    }
    
    m_propertyChangedConnection = m_property->valueChanged.connect(
        [this, lineEdit]()
        {
            lineEdit->setText(QString::fromStdString(m_property->toString()));
        });

    return lineEdit;
}

void StringEditor::setString(const QString & text)
{
    m_property->fromString(text.toStdString());
}

} // namespace propertyguizeug
