#ifndef WIDOTHERS_H
#define WIDOTHERS_H

#include <QPainter>
#include <QPaintDevice>
#include <QLineEdit>
#include <QCheckBox>
#include <QTreeWidget>
#include <QVBoxLayout>

class painterAntiAl: public QPainter
{
public:
    painterAntiAl(QPaintDevice *device);
};

enum class validator {NONE, INT, INT_POS, INT_POS_0, DOUBLE};
class lineEdit: public QLineEdit
{
public:
    lineEdit(enum validator valid);
    ~lineEdit() = default;
    void m_setText(const std::string &text);
    void m_setNumber(double value);
    inline std::string m_text()
        {return text().toStdString();}
    inline double m_number()
        {return text().toDouble();}
    void m_redBoxAnimation(int ms = 1000);
private:
    void m_supValidator(enum validator valid);
    int m_supWidth(enum validator valid);
protected:
    QValidator *m_validator = nullptr;
    QTimer *m_redBoxTimer = nullptr;
};

class checkbox: public QCheckBox
{
public:
    checkbox() {}
    ~checkbox() = default;
    void m_setChecked(bool status);
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
        treeItem* ptr_branch = nullptr, bool isExpanded = true, const std::string &tooltip = "");
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
