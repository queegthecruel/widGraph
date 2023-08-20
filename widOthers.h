#ifndef WIDOTHERS_H
#define WIDOTHERS_H

#include <QPainter>
#include <QPaintDevice>
#include <QLineEdit>
#include <QTreeWidget>

class painterAntiAl: public QPainter
{
public:
    painterAntiAl(QPaintDevice *device);
};

class lineEdit: public QLineEdit
{
public:
    lineEdit()
    {}
    ~lineEdit() = default;
    inline void m_setNumber(double value)
        {setText(QString::number(value));}
    inline double m_number()
        {return text().toDouble();}
};

class treeWidget;
class treeItem: public QTreeWidgetItem
{
public:
    treeItem(treeItem *parent);
    treeItem(class treeWidget *parent);
    ~treeItem() = default;
};

class treeWidget: public QTreeWidget
{
public:
    treeWidget();
    ~treeWidget() = default;
    treeItem* m_addChild(const std::string &text, QWidget *ptr_widget,
        treeItem* ptr_branch = nullptr, const std::string &tooltip = "");
};

#endif // WIDOTHERS_H
