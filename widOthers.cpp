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

treeItem::treeItem(treeItem *parent): QTreeWidgetItem(parent)
{}

treeItem::treeItem(class treeWidget *parent): QTreeWidgetItem(parent)
{}
