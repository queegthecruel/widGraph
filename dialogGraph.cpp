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
    m_editFontSizeNumbers = new lineEdit(maxWidthNumbers);
    m_editFontSizeText = new lineEdit(maxWidthNumbers);
    m_tree->m_addChild("Numbers size", m_editFontSizeNumbers);
    m_tree->m_addChild("Text size", m_editFontSizeText);
}

void tabGraphSettingsXAxis::m_loadValues()
{
    auto s_data = ptr_data.lock();
    m_editFontSizeNumbers->m_setNumber(s_data->m_fontNumbers);
    m_editFontSizeText->m_setNumber(s_data->m_fontText);
}

void tabGraphSettingsXAxis::m_saveValues()
{
    auto s_data = ptr_data.lock();
    s_data->m_fontNumbers = m_editFontSizeNumbers->m_number();
    s_data->m_fontText = m_editFontSizeText->m_number();
}

tabGraphSettingsY1Axis::tabGraphSettingsY1Axis(std::weak_ptr<dataAxisY1> data):
    ptr_data(data)
{
    m_editFontSizeNumbers = new lineEdit(maxWidthNumbers);
    m_editFontSizeText = new lineEdit(maxWidthNumbers);
    m_tree->m_addChild("Numbers size", m_editFontSizeNumbers);
    m_tree->m_addChild("Text size", m_editFontSizeText);
}

void tabGraphSettingsY1Axis::m_loadValues()
{
    auto s_data = ptr_data.lock();
    m_editFontSizeNumbers->m_setNumber(s_data->m_fontNumbers);
    m_editFontSizeText->m_setNumber(s_data->m_fontText);
}

void tabGraphSettingsY1Axis::m_saveValues()
{
    auto s_data = ptr_data.lock();
    s_data->m_fontNumbers = m_editFontSizeNumbers->m_number();
    s_data->m_fontText= m_editFontSizeText->m_number();
}

tabGraphSettingsY2Axis::tabGraphSettingsY2Axis(std::weak_ptr<dataAxisY2> data):
    ptr_data(data)
{
    m_editFontSizeNumbers = new lineEdit(maxWidthNumbers);
    m_editFontSizeText = new lineEdit(maxWidthNumbers);
    m_tree->m_addChild("Numbers size", m_editFontSizeNumbers);
    m_tree->m_addChild("Text size", m_editFontSizeText);
}

void tabGraphSettingsY2Axis::m_loadValues()
{
    auto s_data = ptr_data.lock();
    m_editFontSizeNumbers->m_setNumber(s_data->m_fontNumbers);
    m_editFontSizeText->m_setNumber(s_data->m_fontText);
}

void tabGraphSettingsY2Axis::m_saveValues()
{
    auto s_data = ptr_data.lock();
    s_data->m_fontNumbers = m_editFontSizeNumbers->m_number();
    s_data->m_fontText = m_editFontSizeText->m_number();
}

tabGraphSettingsTitle::tabGraphSettingsTitle(std::weak_ptr<dataTitle> data):
    ptr_data(data)
{
    m_editText = new lineEdit(maxWidthText);
    m_editFontSize = new lineEdit(maxWidthNumbers);
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
    m_editFontSizeText = new lineEdit(maxWidthNumbers);
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
    connect(m_butLoadFile, &QAbstractButton::clicked, this, &footerDialogGraph::m_slotLoadFIle);
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