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
enum class orientation;

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
    checkbox *m_checkAutoAxis;
    checkEdit *m_checkManualStep;
    lineEdit *m_editMin, *m_editMax;
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
    widGraphObjectSetting();
    void m_setVisible(bool status);
protected:
    void m_addEndOfWidget();
    void m_addWidget(QWidget* ptr_widget);
    void m_addSpacing(int nPixels);
    static QVector<std::tuple<QString, QIcon>> m_getIconsForCurve(int iconWidth = 40, int iconHeight = 15);
    static QVector<std::tuple<QString, QIcon>> m_getIconsForPoints(int iconSize = 17);
    static QVector<std::tuple<QString, QIcon>> m_getIconsForArea(int iconWidth = 40, int iconHeight = 15);
    virtual void m_setEnabled(bool enabled) = 0;
    QWidget *m_separator();
signals:
    void m_signalChanged();
protected slots:
    void m_slotSendSignalChanged();
private:
    QWidget *m_widBackground;
    HBoxLayout *m_layBackground;
};

class widGraphObjectSettingWithName: public widGraphObjectSetting
{
    Q_OBJECT
public:
    widGraphObjectSettingWithName(const QString &name);
protected slots:
    void m_slotEnabledToggled();
protected:
    checkbox *m_checkEnable;
};

class butGraphObjectSettingButton: public QPushButton
{
public:
    butGraphObjectSettingButton(QIcon icon, const QString &tooltip);
};

class widGraphObjectSettingMain;
class widGraphObjectSettingOperation: public QWidget
{
    Q_OBJECT
public:
    widGraphObjectSettingOperation(int pos, widGraphObjectSettingMain* ptr_widMain);
    void m_setValues(bool enable);
    std::tuple<bool> m_getValues();
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
    butGraphObjectSettingButton *m_butMoveUp, *m_butMoveDown, *m_butDelete;
};

class widGraphObjectSettingCurve: public widGraphObjectSettingWithName
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

class widGraphObjectSettingPoints: public widGraphObjectSettingWithName
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

class widGraphObjectSettingArea: public widGraphObjectSettingWithName
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

class widGraphObjectSettingColumn: public widGraphObjectSettingWithName
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

class widGraphObjectSettingOrientation: public widGraphObjectSetting
{
    Q_OBJECT
public:
    widGraphObjectSettingOrientation();
    void m_setValues(enum orientation orient);
    std::tuple<enum orientation> m_getValues();
    virtual void m_setEnabled(bool enabled) override;
protected:
    radiobutton *m_radioHorizontal, *m_radioVertical;
};

class widGraphObjectSettingLegend: public widGraphObjectSettingWithName
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

enum class yAxisPosition;
class widGraphObjectSettingAxis: public widGraphObjectSetting
{
    Q_OBJECT
public:
    widGraphObjectSettingAxis();
    void m_setValues(enum yAxisPosition orient);
    std::tuple<enum yAxisPosition> m_getValues();
    virtual void m_setEnabled(bool enabled) override;
private slots:
    void m_slotSendSignal(bool status);
private:
    radiobutton *m_radioLeft, *m_radioRight;
    yAxisPosition m_state;
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
    widGraphObjectSettingOrientation *m_widOrientation;
    widGraphObjectSettingLegend *m_widLegend;
};

#include <QAbstractTableModel>
#include <QTableView>
#include <QStyledItemDelegate>
enum class objectPropertiesColumns {NAME, YAXIS, CURVE, POINTS, AREA, LEGEND};
class objectPropertiesTableModel: public QAbstractTableModel
{
public:
    objectPropertiesTableModel(std::shared_ptr<dataGraph> ptr_dataGraph);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

    static int m_getIColumnFromEnum(enum objectPropertiesColumns column);
    static enum objectPropertiesColumns m_getEnumFromIndex(int index);
private:
    bool m_setName(int role, const QVariant &value, std::shared_ptr<dataGraphObject> ptr_object);
    QVariant m_name(int role, std::shared_ptr<dataGraphObject> ptr_object) const;
    bool m_setYAxis(int role, const QVariant &value, std::shared_ptr<dataGraphObject> ptr_object);
    QVariant m_yAxis(int role, std::shared_ptr<dataGraphObject> ptr_object) const;
    bool m_setCurve(int role, const QVariant &value, std::shared_ptr<dataGraphObject> ptr_object);
    QVariant m_curve(int role, std::shared_ptr<dataGraphObject> ptr_object) const;
    bool m_setPoints(int role, const QVariant &value, std::shared_ptr<dataGraphObject> ptr_object);
    QVariant m_points(int role, std::shared_ptr<dataGraphObject> ptr_object) const;
    bool m_setArea(int role, const QVariant &value, std::shared_ptr<dataGraphObject> ptr_object);
    QVariant m_area(int role, std::shared_ptr<dataGraphObject> ptr_object) const;
    bool m_setLegend(int role, const QVariant &value, std::shared_ptr<dataGraphObject> ptr_object);
    QVariant m_legend(int role, std::shared_ptr<dataGraphObject> ptr_object) const;
private:
    std::weak_ptr<dataGraph> ptr_data;
};

class delegateYAxis: public QStyledItemDelegate
{
    Q_OBJECT
public:
    delegateYAxis();
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
private slots:
    void commitAndCloseEditor();
};

class delegateOperation: public QStyledItemDelegate
{
public:
    delegateOperation();
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

class delegateCurve: public QStyledItemDelegate
{
    Q_OBJECT
public:
    delegateCurve();
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
private slots:
    void commitAndCloseEditor();
};

class delegatePoints: public QStyledItemDelegate
{
    Q_OBJECT
public:
    delegatePoints();
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
private slots:
    void commitAndCloseEditor();
};

class delegateArea: public QStyledItemDelegate
{
    Q_OBJECT
public:
    delegateArea();
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
private slots:
    void commitAndCloseEditor();
};

class delegateLegend: public QStyledItemDelegate
{
    Q_OBJECT
public:
    delegateLegend();
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
private slots:
    void commitAndCloseEditor();
};

class tabGraphSettingsObjects2: public tabGraph
{
    Q_OBJECT
public:
    tabGraphSettingsObjects2(std::weak_ptr<dataGraph> ptr_data);
    void m_loadValues() {}
    void m_saveValues() {}
signals:
    void m_graphDataChanged();
private slots:
    void m_slotDataChanged();
private:
    std::weak_ptr<dataGraph> ptr_graphData;
    objectPropertiesTableModel *m_model;
    QTableView *m_table;
};

class tabGraphSettingsObjects: public tabGraph
{
    Q_OBJECT
public:
    tabGraphSettingsObjects(std::weak_ptr<dataGraph> ptr_data);
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
    std::vector<std::tuple<widGraphObjectSettingOperation*,
                           widGraphObjectSettingAxis*,
                           widGraphObjectSettingMain*
    >> m_vWidgets;
};

class tabGraphSettingsLegend: public tabGraphSettings
{
    Q_OBJECT
public:
    tabGraphSettingsLegend(std::weak_ptr<dataLegend> data);
    ~tabGraphSettingsLegend() = default;
    virtual void m_loadValues() override;
    virtual void m_saveValues() override;
private slots:
    void m_slotAlignedAxesToggled();
protected:
    std::weak_ptr<dataLegend> ptr_data;
    checkbox *m_checkShowLegend, *m_checkShowTopLine;
    checkEdit *m_editSize;
    lineEdit *m_editFontSizeText;
    spinbox *m_editNColumns;
    checkbox *m_checkAlignToAxes;
};

class graphSettingsWidget: public QWidget
{
public:
    graphSettingsWidget(std::weak_ptr<dataGraph> data);
    ~graphSettingsWidget() = default;
    void m_loadValues();
    void m_saveValues();
private:
    std::weak_ptr<dataGraph> ptr_data;
    std::vector<tabGraph*> m_tabs;

    tabGraphSettingsTitle *m_title;
    tabGraphSettingsXAxis *m_xAxis;
    tabGraphSettingsY1Axis *m_yAxis1;
    tabGraphSettingsY2Axis *m_yAxis2;
    tabGraphSettingsLegend *m_legend;
    tabGraphSettingsDrawArea *m_drawArea;
    tabGraphSettingsObjects *m_objects;
    tabGraphSettingsObjects2 *m_objects2;

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
    void m_emitDataChanged();
protected:
    HBoxLayout *m_layBackground;
    QPushButton *m_butClose, *m_butApply;
    QPushButton *m_butLoadFile, *m_butSaveFile;
    QPushButton *m_butTest;
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
    void m_slotDataChanged();
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
    QTreeWidgetItem *m_addChild(const std::string &text,
        QVector<QWidget*> widgets,
        const std::string &tooltip = "");
signals:
    void m_signalMoved(int from, int to);
};

#endif // DIALOGGRAPH_H
