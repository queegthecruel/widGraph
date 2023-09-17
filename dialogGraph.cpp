#include "dialogGraph.h"
#include "dataGraph.h"
#include "widGraph.h"

dialogs::dialogs()
{

}

dialogGraph::dialogGraph(widGraph *graph, std::weak_ptr<dataGraph> data, int tabIndex):
    ptr_graph(graph), ptr_data(data)
{
    m_tabbed = new graphSettingsTabWidget();
    m_tabbed->m_addTab(new tabGraphSettingsTitle(ptr_data.lock()->m_title), "Title");
    m_tabbed->m_addTab(new tabGraphSettingsXAxis(ptr_data.lock()->m_X), "X-axis");
    m_tabbed->m_addTab(new tabGraphSettingsY1Axis(ptr_data.lock()->m_Y1), "Y1-axis");
    m_tabbed->m_addTab(new tabGraphSettingsY2Axis(ptr_data.lock()->m_Y2), "Y2-axis");
    m_tabbed->m_addTab(new tabGraphSettingsLegend(ptr_data.lock()->m_legend), "Legend");
    m_tabbed->m_addTab(new tabGraphSettingsDrawArea(ptr_data.lock()->m_drawArea), "Draw area");

    m_footer = new footerDialogGraph();
    connect(m_footer, &footerDialogGraph::m_emitApply, this, &dialogGraph::m_slotApply);
    connect(m_footer, &footerDialogGraph::m_emitClose, this, &dialogGraph::m_slotClose);
    connect(m_footer, &footerDialogGraph::m_emitSaveFile, this, &dialogGraph::m_slotSaveFile);
    connect(m_footer, &footerDialogGraph::m_emitLoadFile, this, &dialogGraph::m_slotLoadFile);

    m_layBackground = new VBoxLayout(this);
    m_layBackground->addWidget(m_tabbed);
    m_layBackground->addWidget(m_footer);

    m_loadValues();
    m_setTab(tabIndex);
    resize(800,600);
}

void dialogGraph::m_slotApply()
{
    m_saveValues();
    ptr_graph->m_loadValues();
}

void dialogGraph::m_slotSaveFile()
{
    std::ofstream saveFile("example.txt", std::ios::binary);
    ptr_graph->m_getData().lock()->m_saveToFile(saveFile);
}

void dialogGraph::m_slotLoadFile()
{
    std::ifstream loadFile("example.txt", std::ios::binary);
    auto newData = std::make_shared<dataGraph>(loadFile);
    ptr_graph->m_setData(newData);
}

tabGraphSettings::tabGraphSettings()
{
    m_tree = new treeWidget();
    m_layBackground = new VBoxLayout(this);
    m_layBackground->addWidget(m_tree);
}

tabGraphSettingsXAxis::tabGraphSettingsXAxis(std::weak_ptr<dataAxisX> data):
    ptr_data(data)
{
}

void tabGraphSettingsXAxis::m_loadValues()
{
    auto s_data = ptr_data.lock();
    m_loadGeneralValues(s_data);
}

void tabGraphSettingsXAxis::m_saveValues()
{
    auto s_data = ptr_data.lock();
    m_saveGeneralValues(s_data);
}

tabGraphSettingsY1Axis::tabGraphSettingsY1Axis(std::weak_ptr<dataAxisY1> data):
    ptr_data(data)
{
}

void tabGraphSettingsY1Axis::m_loadValues()
{
    auto s_data = ptr_data.lock();
    m_loadGeneralValues(s_data);
}

void tabGraphSettingsY1Axis::m_saveValues()
{
    auto s_data = ptr_data.lock();
    m_saveGeneralValues(s_data);
}

tabGraphSettingsY2Axis::tabGraphSettingsY2Axis(std::weak_ptr<dataAxisY2> data):
    ptr_data(data)
{
}

void tabGraphSettingsY2Axis::m_loadValues()
{
    auto s_data = ptr_data.lock();
    m_loadGeneralValues(s_data);
}

void tabGraphSettingsY2Axis::m_saveValues()
{
    auto s_data = ptr_data.lock();
    m_saveGeneralValues(s_data);
}

tabGraphSettingsTitle::tabGraphSettingsTitle(std::weak_ptr<dataTitle> data):
    ptr_data(data)
{
    m_editText = new lineEdit(validator::NONE);
    m_editFontSize = new lineEdit(validator::INT_POS);
    m_tree->m_addChild("Text", m_editText);
    m_tree->m_addChild("Font size", m_editFontSize);
}

void tabGraphSettingsTitle::m_loadValues()
{
    auto s_data = ptr_data.lock();
    m_editText->m_setText(s_data->m_text);
    m_editFontSize->m_setNumber(s_data->m_fontText);
}

void tabGraphSettingsTitle::m_saveValues()
{
    auto s_data = ptr_data.lock();
    s_data->m_text = m_editText->m_text();
    s_data->m_fontText = m_editFontSize->m_number();
}

tabGraphSettingsDrawArea::tabGraphSettingsDrawArea(std::weak_ptr<dataDrawArea> data):
    ptr_data(data)
{
    //m_editFontSizeNumbers = new lineEdit();
    //m_tree->m_addChild("Numbers size", m_editFontSizeNumbers);
}

void tabGraphSettingsDrawArea::m_loadValues()
{
    auto s_data = ptr_data.lock();
//    m_editFontSizeNumbers->m_setNumber(s_data->m_fontNumbers);
}

void tabGraphSettingsDrawArea::m_saveValues()
{
    auto s_data = ptr_data.lock();
//    s_data->m_fontNumbers = m_editFontSizeNumbers->m_number();
}

tabGraphSettingsLegend::tabGraphSettingsLegend(std::weak_ptr<dataLegend> data):
    ptr_data(data)
{
    m_editFontSizeText = new lineEdit(validator::INT_POS);
    m_tree->m_addChild("Text size", m_editFontSizeText);
}

void tabGraphSettingsLegend::m_loadValues()
{
    auto s_data = ptr_data.lock();
    m_editFontSizeText->m_setNumber(s_data->m_fontText);
}

void tabGraphSettingsLegend::m_saveValues()
{
    auto s_data = ptr_data.lock();
    s_data->m_fontText = m_editFontSizeText->m_number();
}

graphSettingsTabWidget::graphSettingsTabWidget()
{
  //  tabBar()->setExpanding(true);
}

void graphSettingsTabWidget::m_addTab(tabGraphSettings *tab,
                                      const std::string &title)
{
    addTab(tab, QString::fromStdString(title));
    m_tabs.push_back(tab);
}

void graphSettingsTabWidget::m_loadValues()
{
    for (auto &var: m_tabs)
        var->m_loadValues();
}

void graphSettingsTabWidget::m_saveValues()
{
    for (auto &var: m_tabs)
        var->m_saveValues();
}

footerDialogGraph::footerDialogGraph()
{
    m_butSaveFile = new QPushButton("Save to file");
    m_butLoadFile = new QPushButton("Load from file");
    m_butClose = new QPushButton("Close");
    m_butApply = new QPushButton("Apply");
    connect(m_butSaveFile, &QAbstractButton::clicked, this, &footerDialogGraph::m_slotSaveFile);
    connect(m_butLoadFile, &QAbstractButton::clicked, this, &footerDialogGraph::m_slotLoadFile);
    connect(m_butClose, &QAbstractButton::clicked, this, &footerDialogGraph::m_slotClose);
    connect(m_butApply, &QAbstractButton::clicked, this, &footerDialogGraph::m_slotApply);
    m_layBackground = new HBoxLayout(this);
    m_layBackground->setContentsMargins(4,0,4,4);
    m_layBackground->setSpacing(4);
    m_layBackground->addWidget(m_butSaveFile);
    m_layBackground->addWidget(m_butLoadFile);
    m_layBackground->addStretch();
    m_layBackground->addWidget(m_butApply);
    m_layBackground->addWidget(m_butClose);
}

tabGraphSettingsAxis::tabGraphSettingsAxis()
{
    m_editFontSizeNumbers = new lineEdit(validator::INT_POS);
    m_editFontSizeText = new lineEdit(validator::INT_POS);
    m_checkAutoAxis = new checkbox();
    connect(m_checkAutoAxis, &QAbstractButton::toggled, this, &tabGraphSettingsAxis::m_slotAutoAxisToggled);
    m_checkAutoStep = new checkbox();
    connect(m_checkAutoStep, &QAbstractButton::toggled, this, &tabGraphSettingsAxis::m_slotAutoStepToggled);
    m_editMin = new lineEdit(validator::DOUBLE);
    m_editMax = new lineEdit(validator::DOUBLE);
    m_editStep = new lineEdit(validator::DOUBLE);
    m_tree->m_addChild("Numbers size", m_editFontSizeNumbers);
    m_tree->m_addChild("Text size", m_editFontSizeText);
    auto *titleMinMax = m_tree->m_addChild("Auto axis", m_checkAutoAxis);
    m_tree->m_addChild("Min", m_editMin, titleMinMax);
    m_tree->m_addChild("Max", m_editMax, titleMinMax);
    m_tree->m_addChild("Auto step", m_checkAutoStep, titleMinMax);
    m_tree->m_addChild("Step", m_editStep, titleMinMax);
}

void tabGraphSettingsAxis::m_loadGeneralValues(std::shared_ptr<dataAxis> s_data)
{
    m_editFontSizeNumbers->m_setNumber(s_data->m_fontNumbers);
    m_editFontSizeText->m_setNumber(s_data->m_fontText);
    m_checkAutoAxis->m_setChecked(s_data->m_autoAxis);
    m_checkAutoStep->m_setChecked(s_data->m_autoStep);
    m_editMin->m_setNumber(s_data->m_min);
    m_editMax->m_setNumber(s_data->m_max);
    m_editStep->m_setNumber(s_data->m_step);
    m_slotAutoAxisToggled();
}

void tabGraphSettingsAxis::m_saveGeneralValues(std::shared_ptr<dataAxis> s_data)
{
    s_data->m_fontNumbers = m_editFontSizeNumbers->m_number();
    s_data->m_fontText = m_editFontSizeText->m_number();
    s_data->m_autoAxis = m_checkAutoAxis->isChecked();
    s_data->m_autoStep = m_checkAutoStep->isChecked();
    s_data->m_min = m_editMin->m_number();
    s_data->m_max = m_editMax->m_number();
    s_data->m_step = m_editStep->m_number();
}

void tabGraphSettingsAxis::m_slotAutoAxisToggled()
{
    bool autoAxis = m_checkAutoAxis->isChecked();
    m_editMin->setEnabled(!autoAxis);
    m_editMax->setEnabled(!autoAxis);
    m_checkAutoStep->setEnabled(!autoAxis);
    m_editStep->setEnabled(!autoAxis);
}

void tabGraphSettingsAxis::m_slotAutoStepToggled()
{
    bool autoStep = m_checkAutoStep->isChecked();
 //   bool autoAxis = m_checkAutoAxis->isChecked();
    m_editStep->setEnabled(!autoStep);
}
