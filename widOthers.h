#ifndef WIDOTHERS_H
#define WIDOTHERS_H

#include <QPainter>
#include <QPaintDevice>
#include <QLineEdit>
#include <QTreeWidget>
#include <QVBoxLayout>

class painterAntiAl: public QPainter
{
public:
    painterAntiAl(QPaintDevice *device);
};

class lineEdit: public QLineEdit
{
public:
    lineEdit(int maxWidth = -1);
    ~lineEdit() = default;
    inline void m_setText(const std::string &text)
        {setText(QString::fromStdString(text));}
    inline void m_setNumber(double value)
        {setText(QString::number(value));}
    inline std::string m_text()
        {return text().toStdString();}
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

class VBoxLayout: public QVBoxLayout
{
public:
    VBoxLayout(QWidget *parent);
    ~VBoxLayout() = default;
};

class HBoxLayout: public QHBoxLayout
{
public:
    HBoxLayout(QWidget *parent);
    ~HBoxLayout() = default;
};

class graphLayout: public QGridLayout
{
public:
    graphLayout(QWidget *parent);
    ~graphLayout() = default;
};

#endif // WIDOTHERS_H
