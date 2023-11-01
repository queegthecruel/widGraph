#include "dialogGraph.h"
#include "dataGraph.h"
#include "widGraph.h"

dialogGraph::dialogGraph(widGraph *graph, std::weak_ptr<dataGraph> data):
    ptr_graph(graph), ptr_data(data)
{
    m_content = new graphSettingsWidget(ptr_data);

    m_footer = new footerDialogGraph();
    connect(m_footer, &footerDialogGraph::m_emitApply, this, &dialogGraph::m_slotApply);
    connect(m_footer, &footerDialogGraph::m_emitClose, this, &dialogGraph::m_slotClose);
    connect(m_footer, &footerDialogGraph::m_emitSaveFile, this, &dialogGraph::m_slotSaveFile);
    connect(m_footer, &footerDialogGraph::m_emitLoadFile, this, &dialogGraph::m_slotLoadFile);

    m_layBackground = new VBoxLayout(this);
    m_layBackground->addWidget(m_content);
    m_layBackground->addWidget(m_footer);

    m_loadValues();
    resize(800,600);
}

void dialogGraph::m_slotClose()
{
    close();
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
    saveFile.close();

    std::ofstream saveFileContent("exampleContent.txt", std::ios::binary);
    const auto &vObjects = ptr_graph->m_getData().lock()->m_vectorOfObjects;
    writeInt(saveFileContent, vObjects.size());
    for (const auto& var: vObjects)
        var->m_getData().lock()->m_saveToFile(saveFileContent);
    saveFileContent.close();
}

void dialogGraph::m_slotLoadFile()
{
    std::ifstream loadFile("example.txt", std::ios::binary);
    auto newData = std::make_shared<dataGraph>(loadFile);
    ptr_graph->m_setData(newData);
    loadFile.close();

    std::ifstream loadFileContent("exampleContent.txt", std::ios::binary);
    int nGraphObjects = readInt(loadFileContent);
    for (int i = 0; i < nGraphObjects; ++i) {
        int type = readInt(loadFileContent);
        auto ptr_object = graphObjects::m_createGraphObject(type);
        auto ptr_objectData = std::make_shared<dataGraphObject>(loadFileContent);
        ptr_object->m_setData(ptr_objectData);
        ptr_graph->m_addObject(ptr_object);
    }
    loadFileContent.close();
}

tabGraphSettings::tabGraphSettings(const QString &title)
{
    m_tree = new treeWidget();
    m_layBackground = new VBoxLayout(this);
    m_layBackground->setContentsMargins(2,2,2,2);
    m_layBackground->setSpacing(1);
    m_layBackground->addWidget(new label(title, true));
    m_layBackground->addWidget(m_tree);
    setStyleSheet(".tabGraphSettings "
                  "{background:black;}");
}

tabGraphSettingsXAxis::tabGraphSettingsXAxis(std::weak_ptr<dataAxisX> data):
    tabGraphSettingsAxis("X axis"),
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
    tabGraphSettingsAxis("Y axis left"),
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
    tabGraphSettingsAxis("Y axis right"),
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
    tabGraphSettings("Title"),
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
    tabGraphSettings("Draw area"),
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
    tabGraphSettings("Legend"),
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

graphSettingsWidget::graphSettingsWidget(std::weak_ptr<dataGraph> data):
    ptr_data(data)
{
    m_layBackground = new QVBoxLayout(this);
    m_layBackground->setContentsMargins(2,2,2,2);
//    m_layBackground->setSpacing(0);
    setStyleSheet(""
                  ".QSplitter"
                        "{background: gray;}"
                  ".QSplitter::handle"
                        "{width: 5px;}");

    m_title = new tabGraphSettingsTitle(ptr_data.lock()->m_title);
    m_xAxis = new tabGraphSettingsXAxis(ptr_data.lock()->m_X);
    m_yAxis1 = new tabGraphSettingsY1Axis(ptr_data.lock()->m_Y1);
    m_yAxis2 = new tabGraphSettingsY2Axis(ptr_data.lock()->m_Y2);
    m_legend = new tabGraphSettingsLegend(ptr_data.lock()->m_legend);
    m_drawArea = new tabGraphSettingsDrawArea(ptr_data.lock()->m_drawArea);
    m_objects = new tabGraphSettingsObjects(ptr_data.lock()->m_vectorOfObjects);

    m_tabs.push_back(m_title);
    m_tabs.push_back(m_xAxis);
    m_tabs.push_back(m_yAxis1);
    m_tabs.push_back(m_yAxis2);
    m_tabs.push_back(m_legend);
    m_tabs.push_back(m_drawArea);
    m_tabs.push_back(m_objects);


    QSplitter *splitGeneral = new QSplitter(Qt::Horizontal);
    splitGeneral->addWidget(m_title);
    splitGeneral->addWidget(m_drawArea);
    splitGeneral->addWidget(m_legend);
    QSplitter *splitAxes = new QSplitter(Qt::Horizontal);
    splitAxes->addWidget(m_yAxis1);
    splitAxes->addWidget(m_xAxis);
    splitAxes->addWidget(m_yAxis2);

    QSplitter *splitMain = new QSplitter(Qt::Vertical);
    splitMain->addWidget(splitGeneral);
    splitMain->addWidget(splitAxes);
    splitMain->addWidget(m_objects);

    m_layBackground->addWidget(splitMain);
    splitMain->setSizes({200,600,200});
}

void graphSettingsWidget::m_loadValues()
{
    for (auto &var: m_tabs)
        var->m_loadValues();
}

void graphSettingsWidget::m_saveValues()
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

tabGraphSettingsAxis::tabGraphSettingsAxis(const QString &title):
    tabGraphSettings(title)
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
    auto *titleMinMax = m_tree->m_addChild("Auto axis", m_checkAutoAxis, nullptr, true);
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

tabGraphSettingsObjects::tabGraphSettingsObjects(const std::vector<std::shared_ptr<graphObjects> > &vObjects):
    tabGraphSettings("Curves"),
    vGraphObjects(vObjects)
{

}

void tabGraphSettingsObjects::m_loadValues()
{
    m_tree->clear();
    int nCurves = vGraphObjects.size();
    for (int i = 0; i < nCurves; ++i)
        m_tree->m_addChild(vGraphObjects[i]->m_getData().lock()->m_getName(), new widGraphObjectSetting());
}

void tabGraphSettingsObjects::m_saveValues()
{

}

widGraphObjectSetting::widGraphObjectSetting()
{
    HBoxLayout *lay = new HBoxLayout(this);
    colorPicker *a = new colorPicker();
    lay->addWidget(a);
    lay->addStretch();
}

colorPicker::colorPicker()
{
    HBoxLayout *lay = new HBoxLayout(this);
    m_button = new QPushButton("AAA");
    lay->addWidget(m_button);

    m_menu = new QMenu(this);
   // m_menu->addAction(new QAction("Action"));
    QGridLayout *layGrid = new QGridLayout(m_menu);
    layGrid->setContentsMargins(0,0,0,0);
    layGrid->setSpacing(1);
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            layGrid->addWidget(new colorButton(Qt::red),i, j);

    connect(m_button, &QAbstractButton::pressed, this, &colorPicker::m_slotShowMenu);
 //   setEditable(true);
 //   setFrame(false);
 /*   HBoxLayout *lay = new HBoxLayout(this);
    m_combo = new QComboBox();
    m_combo->setFixedWidth(25+10);
    m_combo->addItem();
    lay->addWidget(m_combo);*/
}

void colorPicker::m_slotShowMenu()
{
    m_menu->move(mapToGlobal(m_button->pos() + QPoint(0, m_button->height())));
    m_menu->setFixedSize(150,100);
    m_menu->show();
}

colorButton::colorButton(const QColor &color):
    m_color(color)

{
    setFlat(true);
    QString tempColor = QString::number(m_color.red()) + "," +
                        QString::number(m_color.green()) + "," +
                        QString::number(m_color.blue());
    setStyleSheet("background: rgb(" + tempColor + ");"
                  "border: 1px solid gray;");
}
