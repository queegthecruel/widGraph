#include "widOthers.h"

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
    QWidget *ptr_widget, treeItem *ptr_branch, const std::string &tooltip)
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
    int maxWidth = 200;
    switch (valid) {
    case validator::NONE:
        maxWidth = 200;
        break;
    case validator::INT:
        maxWidth = 75;
        break;
    case validator::DOUBLE:
        maxWidth = 75;
        break;
    }
    setMaximumWidth(maxWidth);
    setMinimumWidth(25);
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

void checkbox::m_setChecked(bool status)
{
    blockSignals(true);
    setChecked(status);
    blockSignals(false);
}
