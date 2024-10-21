#include "widPretty.h"
#include <QTimer>

treeWidget::treeWidget()
{
    setColumnCount(2);
    setHeaderLabels({"Item", "Value"});
    header()->setVisible(false);
    setColumnWidth(0, 150);
    setColumnWidth(1, 100);
    setIndentation(8);
    setStyleSheet("QTreeWidget::item {"
                      "padding-top: 1px;"
                      "height: 20px;"
                  "}");
    setSelectionMode(QAbstractItemView::NoSelection);
    setFocusPolicy(Qt::NoFocus);
}

QFont treeWidget::m_createFont(bool isBold, int pixelSize)
{
    QFont font;
    font.setBold(isBold);
    font.setPixelSize(pixelSize);
    return font;
}

treeItem *treeWidget::m_addChild(const std::string &text,
    QWidget *ptr_widget, treeItem *ptr_branch, bool isExpanded, const std::string &tooltip)
{
    // Create item
        treeItem *item = m_createChild(ptr_branch);
    // Set texts, widgets, and properties
        QFont font = m_createFont(false);
        item->setFont(0, font);
        item->setText(0, QString::fromStdString(text));
        setItemWidget(item, 1, ptr_widget);
    // Set tooltip
        std::string tooltipText = tooltip;
        if (tooltipText == "")
            tooltipText = text;
        item->setToolTip(0, QString::fromStdString(tooltipText));
    // Set expanded
        item->setExpanded(isExpanded);
    // Return the new item
        return item;
}
/*
treeItem *treeWidget::m_addTitleChild(const std::string &text,
    QWidget *ptr_widget, treeItem *ptr_branch, bool isExpanded, const std::string &tooltip)
{
    // Create item
        treeItem *item = m_createChild(ptr_branch);
    // Set texts, widgets, and properties
        QFont font = m_createFont(true);
        item->setFont(0, font);
        item->setText(0, QString::fromStdString(text));
        setItemWidget(item, 1, ptr_widget);
    // Set tooltip
        std::string tooltipText = tooltip;
        if (tooltipText == "")
            tooltipText = text;
        item->setToolTip(0, QString::fromStdString(tooltipText));
    // Set expanded
        item->setExpanded(isExpanded);
    // Return the new item
        return item;
}
*/
treeItem * treeWidget::m_createChild(treeItem *ptr_branch)
{
    // If no branch is given, connect new item to the branch,
    // Otherwise, connect to main tree, i.e., this
    treeItem *item;
    if (ptr_branch == nullptr)
        item = new treeItem(this);
    else
        item = new treeItem(ptr_branch);
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

HBoxLayout::HBoxLayout(QWidget *parent):
    QHBoxLayout(parent)
{
    setContentsMargins(0,0,0,0);
    setSpacing(0);
}
/*
lineedit2::lineedit2(enum validator valid)
{
    m_init(valid);
}

lineedit2::lineedit2(const unit &_unit, enum validator valid)
{
    m_init(valid);
}

void lineedit2::m_supValidator(enum validator valid)
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
        m_validator = new QDoubleValidator(-qInf(), qInf(), 3, this);
        break;
    }
}

int lineedit2::m_supWidth(enum validator valid)
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

void lineedit2::m_setText(const std::string &text)
{
    blockSignals(true);
    setText(QString::fromStdString(text));
    blockSignals(false);
}

void lineedit2::m_setNumber(double value)
{
    blockSignals(true);
    setText(QString::number(value));
    blockSignals(false);
}

void lineedit2::m_redBoxAnimation(int ms)
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

void lineedit2::m_slotTextFinished()
{
    if (isModified())
        emit m_signalTextFinished();
}

void lineedit2::m_init(enum validator valid)
{
    // Width
        int maxWidth = m_supWidth(valid);
        setMaximumWidth(maxWidth);
        setMinimumWidth(25);
    // Validator
        m_supValidator(valid);
    // Signals
        connect(this, &Qlineedit2::editingFinished,
                this, &lineedit2::m_slotTextFinished);
    // Style
        setStyleSheet("lineedit2 {"
                        "background: white;"
                        "border: 1px solid gray;"
                      "}"
                      "lineedit2:disabled {"
                        "background: rgb(240,240,240);"
                        "border: 1px solid gray;"
                      "}"
                      "lineedit2:focus {"
                        "background: white;"
                        "border: 1px solid darkblue;"
                      "}"
                      );
}
*/
checkbox::checkbox(const QString &title):
  QCheckBox(title)
{

}

void checkbox::m_setChecked(bool status)
{
    blockSignals(true);
    setChecked(status);
    blockSignals(false);
}

dialogs::dialogs()
{
    setAttribute(Qt::WA_DeleteOnClose);
}

label::label(const QString &text, int pixelSize, bool bold):
    QLabel()
{
    m_init(text, "", pixelSize, bold);
}

label::label(const QString &text, const QString &tooltip, int pixelSize, bool bold):
    QLabel()
{
    m_init(text, tooltip, pixelSize, bold);
}

void label::m_init(const QString &text, const QString &tooltip, int pixelSize, bool bold)
{
    QFont font;
    font.setPixelSize(pixelSize);
    font.setBold(bold);
    setFont(font);
    setText(text);
    setToolTip(tooltip);
}

checkEdit::checkEdit(enum validator valid)
{
    m_edit = new lineedit(valid);
    m_check = new checkbox();
    connect(m_check, &QAbstractButton::toggled,
            m_edit, &QCheckBox::setEnabled);
    connect(m_check, &QAbstractButton::toggled,
            this, &checkEdit::m_slotSendToggled);
    connect(m_edit, &lineedit::m_signalFinished,
            this, &checkEdit::m_slotSendEditingFinished);
    HBoxLayout *layBackground = new HBoxLayout(this);
    layBackground->setSpacing(1);
    layBackground->addWidget(m_check);
    layBackground->addSpacing(1);
    layBackground->addWidget(m_edit);
    layBackground->addStretch();
}

void checkEdit::m_setText(bool checked, const QString &text)
{
    m_check->m_setChecked(checked);
    m_edit->m_setText(text);
    m_edit->setEnabled(checked);
}

QString checkEdit::m_getText()
{return m_edit->m_text();}

bool checkEdit::m_getChecked()
{return m_check->isChecked();}

void checkEdit::m_setValues(bool checked, double value)
{
    m_check->setChecked(checked);
    m_edit->m_setNumber(value);
    m_edit->setEnabled(checked);
}

double checkEdit::m_value()
{return m_edit->m_number();}

void checkEdit::m_setEnabled(bool enabled)
{
    setEnabled(enabled);
}

void checkEdit::m_slotSendToggled()
{
    emit m_signalToggled();
}

void checkEdit::m_slotSendEditingFinished()
{
    emit m_signalEditingFinished();
}

spinbox::spinbox(int min, int max):
    QSpinBox()
{
    setMinimum(min);
    setMaximum(max);
    setMaximumWidth(50);
}
void spinbox::m_setValue(int value)
{
    blockSignals(true);
    setValue(value);
    blockSignals(false);
}

combobox::combobox(int fixedWidth):
    QComboBox()
{
    setFixedWidth(fixedWidth);
}

void combobox::m_addItems(const QVector<QString> &items)
{
    int index = 0;
    for (auto &text:items) {
        addItem(text);
        setItemData(index, text, Qt::ToolTipRole);
        ++index;
    }
}

void combobox::m_addItems(const QVector<std::tuple<QString, QIcon> > &items, QSize size, bool showText)
{
    blockSignals(true);
    setIconSize(size);
    int index = 0;
    for (auto &var:items) {
        auto [text, icon] = var;
        if (showText)
            addItem(text);
        else
            addItem("");
        setItemData(index, text, Qt::ToolTipRole);
        setItemIcon(index, icon);
        ++index;
    }
    blockSignals(false);
}

void combobox::m_setCurrentIndex(int index)
{
    blockSignals(true);
    setCurrentIndex(index);
    blockSignals(false);
}

pushbutton::pushbutton(const QString &text):
    QPushButton(text)
{
    setStyleSheet("border: 2px solid gray;");
}

void pushbutton::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPushButton::mouseDoubleClickEvent(event);
    emit m_signalDoubleClicked();
}

widHorizontal::widHorizontal(std::vector<QWidget *> vWidgets)
{
    HBoxLayout *layBackground = new HBoxLayout(this);
    for (auto &var : vWidgets)
        layBackground->addWidget(var);
}


widVertical::widVertical(std::vector<QWidget *> vWidgets)
{
    VBoxLayout *layBackground = new VBoxLayout(this);
    for (auto &var : vWidgets)
        layBackground->addWidget(var);
}


QWidget* colorPicker::m_createWidgetWithColorSet(int nColumns,
                                                 const std::vector<QColor> &vColors,const QString& title)
{
    QWidget *widget = new QWidget();
    VBoxLayout *layVert = new VBoxLayout(widget);
    layVert->addWidget(new label(" " + title, 12, true));
    QGridLayout *layGridBase = new QGridLayout();
    layGridBase->setContentsMargins(0,0,0,0);
    layGridBase->setSpacing(1);
    int nRows = vColors.size()/nColumns;
    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nColumns; ++j) {
            auto *ptr_button = new colorButton(vColors[nColumns*i+j]);
            connect(ptr_button, &colorButton::m_signalSelected,
                    this, &colorPicker::m_slotColorSelected);
            connect(ptr_button, &colorButton::m_signalSelectedAndConfirmed,
                    this, &colorPicker::m_slotColorSelectedAndConfirmed);
            layGridBase->addWidget(ptr_button, i, j);
        }
    }
    layVert->addLayout(layGridBase);
    return widget;
}

void colorPicker::m_createMenu()
{
    m_menu = new QMenu(this);
    VBoxLayout *menuLayout = new VBoxLayout(m_menu);
    int nColumns = 4;
    menuLayout->addWidget(m_createWidgetWithColorSet(
        nColumns, prettyColors, "Pretty colors"));
    menuLayout->addWidget(m_createWidgetWithColorSet(
        nColumns, baseColors, "Base colors"));
    menuLayout->addWidget(new label(" Custom color (RGBA)", 12, true));
    m_customColor = new widCustomColor();
    connect(m_customColor, &widCustomColor::m_signalColorChanged,
            this, &colorPicker::m_slotColorSelected);
    menuLayout->addWidget(m_customColor);

    menuLayout->addWidget(new label(" Preview", 12, true));
    m_butPreview = new colorButton(QColor());
    m_butPreview->setFixedHeight(21);
    m_butOK = new pushbutton("OK");
    m_butOK->setFixedWidth(50);
    connect(m_butOK, &pushbutton::clicked,
            this, &colorPicker::m_slotColorConfirmed);
    widHorizontal *widHor = new widHorizontal({m_butPreview, m_butOK});
    menuLayout->addWidget(widHor);
}

void colorPicker::m_createMainButton(HBoxLayout *lay)
{
    m_button = new colorButton(Qt::blue);
  //  m_button->setStyleSheet("border: 2px solid green;");
    connect(m_button, &QAbstractButton::pressed,
            this, &colorPicker::m_slotShowMenu);
    lay->addWidget(m_button);
}

colorPicker::colorPicker()
{
    HBoxLayout *lay = new HBoxLayout(this);
    m_createMainButton(lay);
}

void colorPicker::m_slotShowMenu()
{
    if (m_menu == nullptr)
        m_createMenu();
    m_customColor->m_setColor(m_color);
    m_butPreview->m_setColor(m_color);
    m_menu->move(mapToGlobal(m_button->pos() + QPoint(0, m_button->height())));
    m_menu->setFixedWidth(200);
    m_menu->show();
}

void colorPicker::m_slotColorSelected(QColor color)
{
    m_customColor->m_setColor(color);
    m_butPreview->m_setColor(color);
}

void colorPicker::m_slotColorSelectedAndConfirmed(QColor color)
{
    m_slotColorSelected(color);
    m_slotColorConfirmed();
}

void colorPicker::m_slotColorConfirmed()
{
    m_setColor(m_butPreview->m_getColor());
//    m_color = m_butPreview->m_getColor();
//    m_button->m_setColor(m_color);
    m_menu->hide();
    emit m_signalColorChanged();
}

void colorPicker::m_setColor(const QColor &color)
{
    m_color = color;
    m_button->m_setColor(color);
}

colorButton::colorButton(const QColor &color):
    pushbutton(),
    m_color(color)
{
    setFlat(true);
    m_setColor(m_color);
    connect(this, &QAbstractButton::clicked,
            this, [this](){emit m_signalSelected(m_color);});
    connect(this, &pushbutton::m_signalDoubleClicked,
            this, [this](){emit m_signalSelectedAndConfirmed(m_color);});
}

void colorButton::m_setColor(const QColor &color)
{
    m_color = color;
    QString tempColor = QString::number(m_color.red()) + "," +
                        QString::number(m_color.green()) + "," +
                        QString::number(m_color.blue()) + "," +
                        QString::number(m_color.alpha());
    setStyleSheet("colorButton {background: rgba(" + tempColor + ");"
                               "border: 1px solid gray;}"
                  "colorButton::disabled {background: lightgray;"
                                         "border: 1px solid gray;}");
}

widCustomColor::widCustomColor()
{
    m_editR = new spinbox(0,255);
    m_editG = new spinbox(0,255);
    m_editB = new spinbox(0,255);
    m_editA = new spinbox(0,255);
    connect(m_editR, &spinbox::valueChanged,
            this, &widCustomColor::m_slotLineeditChanged);
    connect(m_editG, &spinbox::valueChanged,
            this, &widCustomColor::m_slotLineeditChanged);
    connect(m_editB, &spinbox::valueChanged,
            this, &widCustomColor::m_slotLineeditChanged);
    connect(m_editA, &spinbox::valueChanged,
            this, &widCustomColor::m_slotLineeditChanged);

    HBoxLayout *layBackground = new HBoxLayout(this);
    layBackground->addWidget(m_editR);
    layBackground->addWidget(m_editG);
    layBackground->addWidget(m_editB);
    layBackground->addWidget(m_editA);
}

void widCustomColor::m_setColor(const QColor &color)
{
    m_color = color;
    m_editR->m_setValue(m_color.red());
    m_editG->m_setValue(m_color.green());
    m_editB->m_setValue(m_color.blue());
    m_editA->m_setValue(m_color.alpha());
}

void widCustomColor::m_slotLineeditChanged()
{
    QColor color(m_editR->value(),
                 m_editG->value(),
                 m_editB->value(),
                 m_editA->value());
    m_color = color;
    emit m_signalColorChanged(m_color);
}

radiobutton::radiobutton(const QString &title):
    QRadioButton(title)
{

}

void radiobutton::m_setChecked(bool status)
{
    blockSignals(true);
    setChecked(status);
    blockSignals(false);
}

lineEditedit::lineEditedit(validator valid)
{
    createLineedits(valid, valid);
}

lineEditedit::lineEditedit(validator valid1, validator valid2)
{
    createLineedits(valid1, valid2);
}

void lineEditedit::m_setText(const QString &text1, const QString &text2)
{
    m_edit1->m_setText(text1);
    m_edit2->m_setText(text2);
}

QString lineEditedit::m_getText1()
{return m_edit1->m_text();}

QString lineEditedit::m_getText2()
{return m_edit2->m_text();}

void lineEditedit::m_setValues(double value1, double value2)
{
    m_edit1->m_setNumber(value1);
    m_edit2->m_setNumber(value2);
}

double lineEditedit::m_value1()
{return m_edit1->m_number();}

double lineEditedit::m_value2()
{return m_edit2->m_number();}

void lineEditedit::m_setEnabled(bool enabled)
{
    m_edit1->setEnabled(enabled);
    m_edit2->setEnabled(enabled);
}

void lineEditedit::createLineedits(validator valid1, validator valid2)
{
    m_edit1 = new lineedit(valid1);
    m_edit2 = new lineedit(valid2);

    connect(m_edit1, &lineedit::m_signalFinished,
            this, &lineEditedit::m_slotSendEditingFinished);
    connect(m_edit2, &lineedit::m_signalFinished,
            this, &lineEditedit::m_slotSendEditingFinished);
    HBoxLayout *layBackground = new HBoxLayout(this);
    layBackground->setSpacing(1);
    layBackground->addWidget(m_edit1);
    layBackground->addSpacing(1);
    layBackground->addWidget(m_edit2);
    layBackground->addStretch();
}

void lineEditedit::m_slotSendEditingFinished()
{
    emit m_signalEditingFinished();
}

void lineedit::m_setUnit(const unit& _unit)
{
    int i = m_vUnits.indexOf(_unit);
    m_setUnit(i);
}

void lineedit::m_setUnit(int i)
{
    int nActions = m_butUnit->actions().size();
    if (i != -1 && i < nActions) {
        m_iUnit = i;
        auto vActions = m_butUnit->actions();
        auto *ptr_selectedAction = vActions[m_iUnit];
        m_butUnit->setDefaultAction(ptr_selectedAction);
    }
}

void lineedit::m_setTooltip()
{
    QString tooltip;
    switch (m_valid) {
        case validator::NONE:
            tooltip = m_text();
        break;
        default:
            double valueInSI = m_value();
            const unit &_unit = m_getUnit();
            double valueInUnit = _unit.m_toUnitFromSI(valueInSI);
            tooltip = QString::number(valueInUnit) + " " +_unit.m_getUnit() +
                              " = " +
                      QString::number(valueInSI) + " " +_unit.m_getSIUnit().m_getUnit();
        break;
    }
    m_lineValue->setToolTip(tooltip);
}

void lineedit::m_createUnits()
{
    m_butUnit = new QToolButton();
    m_butUnit->setContentsMargins(0,0,0,0);
    m_butUnit->setFixedWidth(40);
    if (m_vUnits.size() > 1)
        m_butUnit->setPopupMode(QToolButton::MenuButtonPopup);
    for(const auto &var: m_vUnits)
        m_butUnit->addAction(new QAction(var.m_getUnit(), this));
    connect(m_butUnit, &QToolButton::triggered,
            this, &lineedit::m_slotUnits);
    const unit &_unit = m_getUnit();
    m_setUnit(_unit);
}

void lineedit::m_createValidator()
{
    switch (m_valid) {
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
        m_validator = new QDoubleValidator(-qInf(), qInf(), 3, this);
        break;
    }
}

lineedit::lineedit(validator valid):
    m_valid(valid), m_vUnits({}), m_iUnit(-1)
{
    m_init();
}

lineedit::lineedit(validator valid, unit _unit):
    m_valid(valid), m_vUnits({_unit}), m_iUnit(0)
{
    m_init();
}

lineedit::lineedit(validator valid, const QVector<unit> &_vUnits):
    m_valid(valid), m_vUnits(_vUnits), m_iUnit(0)
{
    m_init();
}

void lineedit::m_setText(const QString &text)
{
    blockSignals(true);
    m_lineValue->setText(text);
    m_setTooltip();
    blockSignals(false);
}

QString lineedit::m_text() const
{
    return m_lineValue->text();
}

void lineedit::m_setValue(double valueInSI)
{
    blockSignals(true);
    const unit &_unit = m_getUnit();
    double valueInUnit = _unit.m_toUnitFromSI(valueInSI);
    m_lineValue->setText(QString::number(valueInUnit));
    m_setTooltip();
    blockSignals(false);
}

double lineedit::m_value() const
{
    double valueInUnit = m_lineValue->text().toDouble();
    double valueInSI = m_getUnit().m_toSIFromUnit(valueInUnit);
    return valueInSI;
}

void lineedit::m_setNumber(double value)
{
    blockSignals(true);
    m_lineValue->setText(QString::number(value));
    m_setTooltip();
    blockSignals(true);
}

double lineedit::m_number() const
{
    double value = m_lineValue->text().toDouble();
    return value;
}

void lineedit::m_setEnabled(bool enabled)
{
    setEnabled(enabled);
}

void lineedit::m_redBoxAnimation(int ms)
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

void lineedit::m_init()
{
    m_createlineedit();
    m_createValidator();
    m_createUnits();
    m_setLayout();
}

void lineedit::m_createlineedit()
{
    m_lineValue = new QLineEdit();
    connect(m_lineValue, &QLineEdit::editingFinished,
            this, &lineedit::m_slotEditingFinished);
    connect(this, &lineedit::m_signalFinished,
            this, &lineedit::m_slotValueChanged);
}

const unit &lineedit::m_getUnit() const
{
    if (m_iUnit > 0 && m_iUnit < m_vUnits.size())
        return m_vUnits[m_iUnit];
    else
        return units::none;
}

void lineedit::m_slotUnits(QAction *action)
{
    const auto &vActions = m_butUnit->actions();
    int i = vActions.indexOf(action);
    m_setUnit(i);
    m_slotEditingFinished();
}

void lineedit::m_slotValueChanged()
{
    m_setTooltip();
}

void lineedit::m_slotEditingFinished()
{
//    if (m_lineValue->textChanged())
    emit m_signalFinished();
}

void lineedit::m_setLayout()
{
    auto *vWids = new widHorizontal({m_lineValue, m_butUnit});
    HBoxLayout *layBackground = new HBoxLayout(this);
    layBackground->setContentsMargins(0,0,0,0);
    layBackground->setSpacing(1);
    layBackground->addWidget(vWids);
}

