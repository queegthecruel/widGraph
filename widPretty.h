#ifndef WIDPRETTY_H
#define WIDPRETTY_H

#include "constants.h"
#include "widPretty_global.h"
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QMenu>

enum class validator {NONE, INT, INT_POS, INT_POS_0, DOUBLE};
class WIDPRETTY_EXPORT lineEdit: public QLineEdit
{
    Q_OBJECT
public:
    lineEdit(enum validator valid);
    lineEdit(const unit& _unit, enum validator valid = validator::DOUBLE);
    void m_setText(const std::string &text);
    void m_setNumber(double value);
    inline std::string m_text()
        {return text().toStdString();}
    inline double m_number()
        {return text().toDouble();}
    void m_redBoxAnimation(int ms = 1000);
public slots:
    void m_slotTextFinished();
signals:
    void m_signalTextFinished();
private:
    void m_init(enum validator valid);
    void m_supValidator(enum validator valid);
    int m_supWidth(enum validator valid);
protected:
    QValidator *m_validator = nullptr;
    QTimer *m_redBoxTimer = nullptr;
};

class WIDPRETTY_EXPORT lineedit2: public QWidget
{
    Q_OBJECT
public:
    lineedit2(enum validator valid, unit _unit);
    lineedit2(enum validator valid, const QVector<unit> &_vUnits);
private:
    void m_setLayout();
    void m_createUnits();
    void m_createLineEdit(validator valid);
    unit m_getUnit();
    void m_setUnit(const unit &_unit);
    void m_setUnit(int i);
private slots:
    void m_slotUnits(QAction *action);
private:
    QLineEdit *m_lineValue;
    QToolButton *m_butUnit;
    QValidator *m_validator = nullptr;
    QTimer *m_redBoxTimer = nullptr;
    QVector<unit> m_vUnits;
    int m_iUnit;
};

class WIDPRETTY_EXPORT checkbox: public QCheckBox
{
public:
  checkbox(const QString &title = "");
  void m_setChecked(bool status);
};

class WIDPRETTY_EXPORT radiobutton: public QRadioButton
{
public:
  radiobutton(const QString &title = "");
  void m_setChecked(bool status);
};

class WIDPRETTY_EXPORT combobox: public QComboBox
{
public:
    combobox(int fixedWidth = 100);
    void m_addItems(const QVector<QString>& items);
    void m_addItems(const QVector<std::tuple<QString, QIcon>>& items, QSize size = QSize(20,20), bool showText = true);
    void m_setCurrentIndex(int index);
};

class WIDPRETTY_EXPORT pushbutton: public QPushButton
{
    Q_OBJECT
public:
    pushbutton(const QString &text = "");
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
signals:
    void m_signalDoubleClicked();
};

class WIDPRETTY_EXPORT spinbox: public QSpinBox
{
    Q_OBJECT
public:
    spinbox(int min = 0, int max = 1000);
    void m_setValue(int value);
};

class WIDPRETTY_EXPORT label: public QLabel
{
public:
    label(const QString &text, int pixelSize = 12, bool bold = false);
    label(const QString &text, const QString &tooltip, int pixelSize = 12, bool bold = false);
private:
    void m_init(const QString &text, const QString &, int pixelSize, bool bold);
};

class treeWidget;
class WIDPRETTY_EXPORT treeItem: public QTreeWidgetItem
{
public:
    treeItem(treeItem *parent);
    treeItem(class treeWidget *parent);
};

class WIDPRETTY_EXPORT treeWidget: public QTreeWidget
{
public:
    treeWidget();
    treeItem* m_addChild(const std::string &text, QWidget *ptr_widget,
        treeItem* ptr_branch = nullptr, bool isExpanded = false, const std::string &tooltip = "");
 //   treeItem* m_addTitleChild(const std::string &text, QWidget *ptr_widget,
 //       treeItem* ptr_branch = nullptr, bool isExpanded = true, const std::string &tooltip = "");
private:
    treeItem * m_createChild(treeItem *ptr_branch);
    QFont m_createFont(bool isBold, int pixelSize = 13);
};

class WIDPRETTY_EXPORT VBoxLayout: public QVBoxLayout
{
public:
    VBoxLayout(QWidget *parent);
};

class WIDPRETTY_EXPORT HBoxLayout: public QHBoxLayout
{
public:
    HBoxLayout(QWidget *parent);
};

class WIDPRETTY_EXPORT dialogs: public QDialog
{
public:
    dialogs();
};

class WIDPRETTY_EXPORT checkEdit: public QWidget
{
    Q_OBJECT
public:
    checkEdit(validator valid);
    void m_setText(bool checked, const std::string &text);
    std::string m_getText()
        {return m_edit->m_text();}
    bool m_getChecked()
        {return m_check->isChecked();}
    void m_setValues(bool checked, double value);
    inline double m_value()
        {return m_edit->m_number();}
    void m_setEnabled(bool enabled);
private slots:
    void m_slotSendToggled();
    void m_slotSendEditingFinished();
signals:
    void m_signalToggled();
    void m_signalEditingFinished();
protected:
    checkbox *m_check;
    lineEdit *m_edit;
};


class WIDPRETTY_EXPORT lineEditEdit: public QWidget
{
    Q_OBJECT
public:
    lineEditEdit(validator valid);
    lineEditEdit(validator valid1, validator valid2);
    void m_setText(const std::string &text1, const std::string &text2);
    std::string m_getText1()
        {return m_edit1->m_text();}
    std::string m_getText2()
        {return m_edit2->m_text();}
    void m_setValues(double value1, double value2);
    double m_value1()
        {return m_edit1->m_number();}
    double m_value2()
        {return m_edit2->m_number();}
    void m_setEnabled(bool enabled);
private:
    void m_createLineEdits(validator valid1, validator valid2);
private slots:
    void m_slotSendEditingFinished();
signals:
    void m_signalEditingFinished();
private:
    lineEdit *m_edit1, *m_edit2;
};

class WIDPRETTY_EXPORT widHorizontal: public QWidget
{
public:
    widHorizontal(std::vector<QWidget *> vWidgets);
};

class WIDPRETTY_EXPORT widVertical: public QWidget
{
public:
    widVertical(std::vector<QWidget *> vWidgets);
};


class WIDPRETTY_EXPORT colorButton: public pushbutton
{
    Q_OBJECT
public:
    colorButton(const QColor &color);
    ~colorButton() = default;
    void m_setColor(const QColor &color);
    inline QColor m_getColor()
    {return m_color;}
signals:
    void m_signalSelected(QColor);
    void m_signalSelectedAndConfirmed(QColor);
protected:
    QColor m_color;
};

class widCustomColor: public QWidget
{
    Q_OBJECT
public:
    widCustomColor();
    void m_setColor(const QColor &color);
protected slots:
    void m_slotLineEditChanged();
signals:
    void m_signalColorChanged(QColor color);
protected:
    spinbox *m_editR, *m_editG, *m_editB, *m_editA;
    QColor m_color;
};

class WIDPRETTY_EXPORT colorPicker: public QWidget
{
    Q_OBJECT
public:
    colorPicker();
    const QColor &m_getColor()
    {return m_color;}
    void m_setColor(const QColor &color);
    inline static const std::vector<QColor> baseColors =
        {Qt::white, Qt::black, Qt::cyan, Qt::darkCyan,
         Qt::red, Qt::darkRed, Qt::magenta, Qt::darkMagenta,
         Qt::green, Qt::darkGreen, Qt::yellow, Qt::darkYellow,
         Qt::blue, Qt::darkBlue, Qt::gray, Qt::darkGray};
    inline static const std::vector<QColor> prettyColors =
        {QColor(0, 114, 190), QColor(218, 83, 25),
         QColor(238, 178, 32), QColor(126, 47, 142),
         QColor(119, 173, 48), QColor(77, 191, 239),
         QColor(163, 20, 47), QColor(17, 119, 51),
         };
signals:
    void m_signalColorChanged();
private:
    QWidget *m_createWidgetWithColorSet(int nColumns, const std::vector<QColor> &vColors, const QString &title);
    void m_createMainButton(HBoxLayout *lay);
    void m_createMenu();
protected slots:
    void m_slotShowMenu();
    void m_slotColorSelected(QColor color);
    void m_slotColorSelectedAndConfirmed(QColor color);
    void m_slotColorConfirmed();
protected:
    colorButton *m_button;
    QMenu *m_menu = nullptr;
    widCustomColor *m_customColor;
    colorButton *m_butPreview;
    pushbutton *m_butOK;
    QColor m_color;
};



#endif // WIDPRETTY_H
