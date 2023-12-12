#ifndef DIALOGGRAPH_H
#define DIALOGGRAPH_H

#include "widPretty.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QMenu>

struct dataGraph;
class widGraph;
struct dataTitle;
struct dataAxis;
struct dataAxisX;
struct dataAxisY1;
struct dataAxisY2;
struct dataDrawArea;
struct dataLegend;
struct graphObjects;

class treeWidgetGraphObjects;
class tabGraph: public QWidget
{
public:
    tabGraph(const QString &title);
    ~tabGraph() = default;
    virtual void m_loadValues() = 0;
    virtual void m_saveValues() = 0;
protected:
    VBoxLayout *m_layBackground;
};

class tabGraphSettings: public tabGraph
{
public:
    tabGraphSettings(const QString &name);
    treeWidget *m_tree;
};

class tabGraphSettingsTitle: public tabGraphSettings
{
public:
    tabGraphSettingsTitle(std::weak_ptr<dataTitle> data);
    ~tabGraphSettingsTitle() = default;
    virtual void m_loadValues() override;
    virtual void m_saveValues() override;
protected:
    std::weak_ptr<dataTitle> ptr_data;
    lineEdit *m_editText;
    lineEdit *m_editFontSize;
};

class tabGraphSettingsAxis: public tabGraphSettings
{
    Q_OBJECT
public:
    tabGraphSettingsAxis(const QString &title);
    ~tabGraphSettingsAxis() = default;
    void m_loadGeneralValues(std::shared_ptr<dataAxis> s_data);
    void m_saveGeneralValues(std::shared_ptr<dataAxis> s_data);
public slots:
    void m_slotAutoAxisToggled();
    void m_slotAutoStepToggled();
protected:
    checkbox *m_checkShowAxis;
    checkEdit *m_editSize;
    lineEdit *m_editFontSizeNumbers, *m_editFontSizeText;
    checkbox *m_checkAutoAxis, *m_checkAutoStep;
    lineEdit *m_editMin, *m_editMax, *m_editStep;
    lineEdit *m_editText;
};

class tabGraphSettingsXAxis: public tabGraphSettingsAxis
{
public:
    tabGraphSettingsXAxis(std::weak_ptr<dataAxisX> data);
    ~tabGraphSettingsXAxis() = default;
    virtual void m_loadValues() override;
    virtual void m_saveValues() override;
protected:
    std::weak_ptr<dataAxisX> ptr_data;
};

class tabGraphSettingsY1Axis: public tabGraphSettingsAxis
{
public:
    tabGraphSettingsY1Axis(std::weak_ptr<dataAxisY1> data);
    ~tabGraphSettingsY1Axis() = default;
    virtual void m_loadValues() override;
    virtual void m_saveValues() override;
protected:
    std::weak_ptr<dataAxisY1> ptr_data;
};

class tabGraphSettingsY2Axis: public tabGraphSettingsAxis
{
public:
    tabGraphSettingsY2Axis(std::weak_ptr<dataAxisY2> data);
    ~tabGraphSettingsY2Axis() = default;
    virtual void m_loadValues() override;
    virtual void m_saveValues() override;
protected:
    std::weak_ptr<dataAxisY2> ptr_data;
};

class tabGraphSettingsDrawArea: public tabGraphSettings
{
public:
    tabGraphSettingsDrawArea(std::weak_ptr<dataDrawArea> data);
    ~tabGraphSettingsDrawArea() = default;
    virtual void m_loadValues() override;
    virtual void m_saveValues() override;
protected:
    std::weak_ptr<dataDrawArea> ptr_data;
    checkbox *m_checkShowGrid;
};

class widGraphObjectSetting: public QWidget
{
    Q_OBJECT
public:
    widGraphObjectSetting(const QString &name);
protected:
    void m_addEndOfWidget();
    static QVector<std::tuple<QString, QIcon>> m_getIconsForCurve(int iconWidth = 40, int iconHeight = 15);
    static QVector<std::tuple<QString, QIcon>> m_getIconsForPoints(int iconSize = 17);
    static QVector<std::tuple<QString, QIcon>> m_getIconsForArea(int iconWidth = 40, int iconHeight = 15);
    virtual void m_setEnabled(bool enabled) = 0;
    QWidget *m_separator();
protected slots:
    void m_slotEnabledToggled();
protected:
    HBoxLayout *m_layBackground;
    checkbox *m_checkEnable;
};

class widGraphObjectSettingButton: public QPushButton
{
public:
    widGraphObjectSettingButton(QIcon icon, const QString &tooltip);
};

class widGraphObjectSettingMain;
class widGraphObjectSettingOperation: public QWidget
{
    Q_OBJECT
public:
    widGraphObjectSettingOperation(int pos, widGraphObjectSettingMain* ptr_widMain);
    void m_setValues(bool enable);
    std::tuple<bool> m_getValues();
//    virtual void m_setEnabled(bool enabled) override {};
    void m_loadValues();
    void m_saveValues();
private slots:
    void m_slotDeleteClicked();
    void m_slotMoveUp();
    void m_slotMoveDown();
signals:
    void m_signalMoveUp(int);
    void m_signalMoveDown(int);
protected:
    int m_positionInVector;
    widGraphObjectSettingMain *ptr_widObjectStyle;
    HBoxLayout *m_layBackground;
    widGraphObjectSettingButton *m_butMoveUp, *m_butMoveDown;
    widGraphObjectSettingButton *m_butDelete;
};

class widGraphObjectSettingCurve: public widGraphObjectSetting
{
    Q_OBJECT
public:
    widGraphObjectSettingCurve();
    void m_setValues(QColor color, int width, int styleIndex, bool enable);
    std::tuple<QColor, int, int, bool> m_getValues();
    virtual void m_setEnabled(bool enabled) override;
protected:
    colorPicker *m_colorPickerCurve;
    spinbox *m_editCurveThick;
    combobox *m_comboCurveStyle;
};

class widGraphObjectSettingPoints: public widGraphObjectSetting
{
    Q_OBJECT
public:
    widGraphObjectSettingPoints();
    void m_setValues(QColor color, int width, int shapeSize, int styleIndex, bool enable);
    std::tuple<QColor, int, int, int, bool> m_getValues();
    virtual void m_setEnabled(bool enabled) override;
protected:
    colorPicker *m_colorPickerPoints;
    spinbox *m_editThickness, *m_editShapeSize;
    combobox *m_comboShape;
};

class widGraphObjectSettingArea: public widGraphObjectSetting
{
    Q_OBJECT
public:
    widGraphObjectSettingArea();
    void m_setValues(QColor color, int styleIndex, bool enable);
    std::tuple<QColor, int, bool> m_getValues();
    virtual void m_setEnabled(bool enabled) override;
protected:
    colorPicker *m_colorPickerArea;
    combobox *m_comboAreaStyle;
};

class widGraphObjectSettingColumn: public widGraphObjectSetting
{
    Q_OBJECT
public:
    widGraphObjectSettingColumn();
    void m_setValues(int width, bool enable);
    std::tuple<int, bool> m_getValues();
    virtual void m_setEnabled(bool enabled) override;
protected:
    spinbox *m_editColumnThick;
};

class widGraphObjectSettingLegend: public widGraphObjectSetting
{
    Q_OBJECT
public:
    widGraphObjectSettingLegend();
    void m_setValues(bool overwrite, const std::string &text, bool enable);
    std::tuple<bool, std::string, bool> m_getValues();
    virtual void m_setEnabled(bool enabled) override;
protected:
    checkEdit *m_editText;
};

class dataGraphObject;
class widGraphObjectSettingMain: public QWidget
{
    Q_OBJECT
public:
    widGraphObjectSettingMain(std::weak_ptr<dataGraphObject> data);
    ~widGraphObjectSettingMain() = default;
    void m_loadValues();
    void m_saveValues();
    inline std::weak_ptr<dataGraphObject> m_getData()
            {return ptr_data;}
protected:
    std::weak_ptr<dataGraphObject> ptr_data;
    widGraphObjectSettingCurve *m_widCurve;
    widGraphObjectSettingPoints *m_widPoints;
    widGraphObjectSettingArea *m_widArea;
    widGraphObjectSettingColumn *m_widColumn;
    widGraphObjectSettingLegend *m_widLegend;
};

class tabGraphSettingsObjects: public tabGraph
{
    Q_OBJECT
public:
    tabGraphSettingsObjects(std::weak_ptr<dataGraph> ptr_data);
    ~tabGraphSettingsObjects() = default;
    virtual void m_loadValues() override;
    virtual void m_saveValues() override;
private:
    void m_createCopyOfGraphData();
    void m_saveDataInWidgets();
    void m_reorderObjectsInGraph();
    void m_deleteDeletedItemsInGraph();
private slots:
    void m_slotMoved(int from, int to);
    void m_slotMoveUp(int from);
    void m_slotMoveDown(int from);
protected:
    treeWidgetGraphObjects *m_tree;
    std::weak_ptr<dataGraph> ptr_graphData;
    std::vector<std::shared_ptr<dataGraphObject>> m_vDataCopy;
    std::vector<int> m_vOrder;
    std::vector<std::tuple<widGraphObjectSettingOperation*, widGraphObjectSettingMain*>> m_vWidgets;
};

class tabGraphSettingsLegend: public tabGraphSettings
{
public:
    tabGraphSettingsLegend(std::weak_ptr<dataLegend> data);
    ~tabGraphSettingsLegend() = default;
    virtual void m_loadValues() override;
    virtual void m_saveValues() override;
protected:
    std::weak_ptr<dataLegend> ptr_data;
    checkbox *m_checkShowLegend;
    checkEdit *m_editSize;
    lineEdit *m_editFontSizeText;
    lineEdit *m_editNColumns;
    checkbox *m_checkArrangeToAxes;
};

class graphSettingsWidget: public QWidget
{
public:
    graphSettingsWidget(std::weak_ptr<dataGraph> data);
    ~graphSettingsWidget() = default;
    void m_loadValues();
    void m_saveValues();
protected:
    std::weak_ptr<dataGraph> ptr_data;
    std::vector<tabGraph*> m_tabs;

    tabGraphSettingsTitle *m_title;
    tabGraphSettingsXAxis *m_xAxis;
    tabGraphSettingsY1Axis *m_yAxis1;
    tabGraphSettingsY2Axis *m_yAxis2;
    tabGraphSettingsLegend *m_legend;
    tabGraphSettingsDrawArea *m_drawArea;
    tabGraphSettingsObjects *m_objects;

    QVBoxLayout *m_layBackground;
};

class footerDialogGraph: public QWidget
{
    Q_OBJECT
public:
    footerDialogGraph();
    ~footerDialogGraph() = default;
public slots:
    void m_slotApply()
        {emit m_emitApply();}
    inline void m_slotClose()
        {emit m_emitClose();}
    inline void m_slotLoadFile()
        {emit m_emitLoadFile();}
    inline void m_slotSaveFile()
        {emit m_emitSaveFile();}
signals:
    void m_emitApply();
    void m_emitClose();
    void m_emitLoadFile();
    void m_emitSaveFile();
protected:
    HBoxLayout *m_layBackground;
    QPushButton *m_butClose, *m_butApply;
    QPushButton *m_butLoadFile, *m_butSaveFile;
};

class dialogGraph: public dialogs
{
    Q_OBJECT
public:
    dialogGraph(widGraph *graph, std::weak_ptr<dataGraph> data);
    ~dialogGraph() = default;
    inline void m_loadValues()
        {m_widContent->m_loadValues();}
    inline void m_saveValues()
        {m_widContent->m_saveValues();}
public slots:
    void m_slotClose();
    void m_slotApply();
    void m_slotSaveFile();
    void m_slotLoadFile();
protected:
    widGraph *ptr_graph;
    std::weak_ptr<dataGraph> ptr_data;
    VBoxLayout *m_layBackground;
    graphSettingsWidget *m_widContent;
    footerDialogGraph *m_widFooter;
};

class treeWidgetGraphObjects: public QTreeWidget
{
    Q_OBJECT
public:
    treeWidgetGraphObjects();
    void dropEvent(QDropEvent *event) override;
    QTreeWidgetItem *m_addChild(const std::string &text, QWidget *ptr_widget, QWidget *ptr_widget2, const std::string &tooltip = "");
signals:
    void m_signalMoved(int from, int to);
};

#endif // DIALOGGRAPH_H
