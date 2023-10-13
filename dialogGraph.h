#ifndef DIALOGGRAPH_H
#define DIALOGGRAPH_H

#include "widPretty.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>

struct dataGraph;
class widGraph;
struct dataTitle;
struct dataAxis;
struct dataAxisX;
struct dataAxisY1;
struct dataAxisY2;
struct dataDrawArea;
struct dataLegend;

class tabGraphSettings: public QWidget
{
public:
    tabGraphSettings();
    ~tabGraphSettings() = default;
    virtual void m_loadValues() = 0;
    virtual void m_saveValues() = 0;
protected:
    VBoxLayout *m_layBackground;
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
    tabGraphSettingsAxis();
    ~tabGraphSettingsAxis() = default;
    void m_loadGeneralValues(std::shared_ptr<dataAxis> s_data);
    void m_saveGeneralValues(std::shared_ptr<dataAxis> s_data);
public slots:
    void m_slotAutoAxisToggled();
    void m_slotAutoStepToggled();
protected:
    lineEdit *m_editFontSizeNumbers, *m_editFontSizeText;
    checkbox *m_checkAutoAxis, *m_checkAutoStep;
    lineEdit *m_editMin, *m_editMax, *m_editStep;
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
    lineEdit *m_editFontSizeText;
};

class graphSettingsTabWidget: public QTabWidget
{
public:
    graphSettingsTabWidget();
    ~graphSettingsTabWidget() = default;
    void m_addTab(tabGraphSettings *tab, const std::string &title);
    void m_loadValues();
    void m_saveValues();
protected:
    std::vector<tabGraphSettings*> m_tabs;
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
    dialogGraph(widGraph *graph, std::weak_ptr<dataGraph> data, int tabIndex = 0);
    ~dialogGraph() = default;
    inline void m_setTab(int index)
        {m_tabbed->setCurrentIndex(index);}
    inline void m_loadValues()
        {m_tabbed->m_loadValues();}
    inline void m_saveValues()
        {m_tabbed->m_saveValues();}
public slots:
    void m_slotClose();
    void m_slotApply();
    void m_slotSaveFile();
    void m_slotLoadFile();
protected:
    widGraph *ptr_graph;
    std::weak_ptr<dataGraph> ptr_data;
    VBoxLayout *m_layBackground;
    graphSettingsTabWidget *m_tabbed;
    footerDialogGraph *m_footer;
};


#endif // DIALOGGRAPH_H
