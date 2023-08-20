#include "dialogs.h"
#include "dataGraph.h"
#include "widGraph.h"

dialogs::dialogs()
{

}

dialogAxis::dialogAxis(widGraph *graph, dataAxis &data):
    ptr_graph(graph), ptr_data(data)
{
    m_editFontSizeNumbers = new lineEdit();

    m_butLoad = new QPushButton("Load values");
    m_butSave = new QPushButton("Save values");
    connect(m_butLoad, &QAbstractButton::clicked, this, &dialogAxis::m_slotLoad);
    connect(m_butSave, &QAbstractButton::clicked, this, &dialogAxis::m_slotSave);
    m_tree = new treeWidget;
    m_tree->m_addChild("Numbers size", m_editFontSizeNumbers);
    m_tree->m_addChild("Load", m_butLoad);
    m_tree->m_addChild("Save", m_butSave);
    m_layBackground = new QVBoxLayout(this);
    m_layBackground->addWidget(m_tree);
    m_loadValues();
}

void dialogAxis::m_loadValues()
{
    const auto &ref_data = ptr_data;
    m_editFontSizeNumbers->m_setNumber(ref_data.m_fontNumbers);
}

void dialogAxis::m_saveValues()
{
    auto &ref_data = ptr_data;
    ref_data.m_fontNumbers = m_editFontSizeNumbers->m_number();
}

void dialogAxis::m_slotLoad()
{
    m_loadValues();
}

void dialogAxis::m_slotSave()
{
    m_saveValues();
    ptr_graph->m_loadValues();
}
