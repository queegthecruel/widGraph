#include "widOthers.h"
#include <QTimer>

painterAntiAl::painterAntiAl(QPaintDevice *device):
    QPainter(device)
{
    setRenderHint(QPainter::Antialiasing);
}

treeWidget::treeWidget()
{
    setColumnCount(2);
    setHeaderLabels({"Item", "Value"});
    setColumnWidth(0, 100);
    setColumnWidth(1, 100);
}

treeItem *treeWidget::m_addChild(const std::string &text,
    QWidget *ptr_widget, treeItem *ptr_branch, bool isExpanded, const std::string &tooltip)
{
    // If no branch is given, connect new item to the branch,
    // Otherwise, connect to main tree, i.e., this
        treeItem *item;
        if (ptr_branch == nullptr)
            item = new treeItem(this);
        else
            item = new treeItem(ptr_branch);
    // Set texts, widgets, and properties
        item->setText(0, QString::fromStdString(text));
        item->setToolTip(0, QString::fromStdString(tooltip));
        setItemWidget(item, 1, ptr_widget);
    // Set expanded
        item->setExpanded(isExpanded);
    // Return the new item
        return item;
}

treeItem::treeItem(treeItem *parent):
    QTreeWidgetItem(parent)
{}

treeItem::treeItem(class treeWidget *parent):
    QTreeWidgetItem(parent)
{}

VBoxLayout::VBoxLayout(QWidget *parent):
    QVBoxLayout(parent)
{
    setContentsMargins(0,0,0,0);
    setSpacing(0);
}

graphLayout::graphLayout(QWidget *parent):
    QGridLayout(parent)
{
    setContentsMargins(0,0,0,0);
    setSpacing(0);
}

HBoxLayout::HBoxLayout(QWidget *parent):
    QHBoxLayout(parent)
{
    setContentsMargins(0,0,0,0);
    setSpacing(0);
}

lineEdit::lineEdit(enum validator valid)
{
    // Width
        int maxWidth = m_supWidth(valid);
        setMaximumWidth(maxWidth);
        setMinimumWidth(25);
    // Validator
        m_supValidator(valid);
}

void lineEdit::m_supValidator(enum validator valid)
{
    switch (valid) {
    case validator::NONE:
        break;
    case validator::INT:
        m_validator = new QIntValidator(-qInf(), qInf(), this);
        break;
    case validator::INT_POS:
        m_validator = new QIntValidator(1, qInf(), this);
        break;
    case validator::INT_POS_0:
        m_validator = new QIntValidator(0, qInf(), this);
        break;
    case validator::DOUBLE:
        m_validator = new QDoubleValidator(-qInf(), qInf(), 2, this);
        break;
    }
}

int lineEdit::m_supWidth(enum validator valid)
{
    int maxWidth = 0;
    switch (valid) {
    case validator::NONE:
        maxWidth = 200;
        break;
    case validator::INT:
    case validator::INT_POS:
    case validator::INT_POS_0:
    case validator::DOUBLE:
        maxWidth = 75;
        break;
    }
    return maxWidth;
}

void lineEdit::m_setText(const std::string &text)
{
    blockSignals(true);
    setText(QString::fromStdString(text));
    blockSignals(false);
}

void lineEdit::m_setNumber(double value)
{
    blockSignals(true);
    setText(QString::number(value));
    blockSignals(false);
}

void lineEdit::m_redBoxAnimation(int ms)
{
    QString stylesheet = styleSheet();

    if (m_redBoxTimer == nullptr)
        m_redBoxTimer = new QTimer(this);
    setStyleSheet("border: 2px solid red;");
    connect(m_redBoxTimer, &QTimer::timeout,
            this, [this, stylesheet](){
                    setStyleSheet(stylesheet);
                    m_redBoxTimer->stop();
                    });
    m_redBoxTimer->setInterval(ms);
    m_redBoxTimer->start();
}

void checkbox::m_setChecked(bool status)
{
    blockSignals(true);
    setChecked(status);
    blockSignals(false);
}
