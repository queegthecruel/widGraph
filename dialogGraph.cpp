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
    resize(1024,600);
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
    m_checkShowGrid = new checkbox();
    m_tree->m_addChild("Show grid", m_checkShowGrid);
}

void tabGraphSettingsDrawArea::m_loadValues()
{
    auto s_data = ptr_data.lock();
    m_checkShowGrid->m_setChecked(s_data->m_showGrid);
}

void tabGraphSettingsDrawArea::m_saveValues()
{
    auto s_data = ptr_data.lock();
    s_data->m_showGrid = m_checkShowGrid->isChecked();
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
    splitMain->setSizes({200,600,400});
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
    // Font
        m_editFontSizeNumbers = new lineEdit(validator::INT_POS);
        m_editFontSizeText = new lineEdit(validator::INT_POS);
        m_tree->m_addChild("Numbers size", m_editFontSizeNumbers);
        m_tree->m_addChild("Text size", m_editFontSizeText);

    // Axis min max
        m_checkAutoAxis = new checkbox();
        connect(m_checkAutoAxis, &QAbstractButton::toggled,
                this, &tabGraphSettingsAxis::m_slotAutoAxisToggled);
        m_checkAutoStep = new checkbox();
        connect(m_checkAutoStep, &QAbstractButton::toggled,
                this, &tabGraphSettingsAxis::m_slotAutoStepToggled);
        m_editMin = new lineEdit(validator::DOUBLE);
        m_editMax = new lineEdit(validator::DOUBLE);
        m_editStep = new lineEdit(validator::DOUBLE);
        auto *titleMinMax = m_tree->m_addChild("Auto axis", m_checkAutoAxis, nullptr, true);
        m_tree->m_addChild("Min", m_editMin, titleMinMax);
        m_tree->m_addChild("Max", m_editMax, titleMinMax);
        m_tree->m_addChild("Auto step", m_checkAutoStep, titleMinMax);
        m_tree->m_addChild("Step", m_editStep, titleMinMax);
    // Text
        m_editText = new lineEdit(validator::NONE);
        m_tree->m_addChild("Text", m_editText);
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
    m_editText->m_setText(s_data->m_text);
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
    s_data->m_text = m_editText->m_text();
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
    vGraphWidgets.clear();
    int nCurves = vGraphObjects.size();
    for (int i = 0; i < nCurves; ++i) {
        auto data = vGraphObjects[i]->m_getData().lock();
        auto *widget = new widGraphObjectSettingMain(data);
        vGraphWidgets.push_back(widget);
        m_tree->m_addChild(data->m_getName(), widget);
        widget->m_loadValues();
    }
}

void tabGraphSettingsObjects::m_saveValues()
{
    int nWidgets = vGraphWidgets.size();
    for (int i = 0; i < nWidgets; ++i) {
        vGraphWidgets[i]->m_saveValues();
    }
}

widGraphObjectSettingMain::widGraphObjectSettingMain(std::weak_ptr<dataGraphObject> data):
    ptr_data(data)
{
    m_widColumn = new widGraphObjectSettingColumn();
    m_widCurve = new widGraphObjectSettingCurve();
    m_widPoints = new widGraphObjectSettingPoints();
    m_widArea = new widGraphObjectSettingArea();
    m_widLegend = new widGraphObjectSettingLegend();
    HBoxLayout *lay = new HBoxLayout(this);
    lay->addWidget(m_widColumn, 1);
    lay->addWidget(m_widCurve, 1);
    lay->addWidget(m_widPoints, 1);
    lay->addWidget(m_widArea, 1);
    lay->addWidget(m_widLegend);
    lay->addStretch(10);
}

void widGraphObjectSettingMain::m_loadValues()
{
    auto data = ptr_data.lock();
    // Curve
        auto [curveColor, curveWidth, curveStyleIndex, curveEnabled]
                = data->m_getStyleOfCurve();
        m_widCurve->m_setValues(curveColor, curveWidth, curveStyleIndex, curveEnabled);
        if (!data->m_getHasCurve())
            m_widCurve->setVisible(false);
    // Points
        auto [pointsColor, pointsWidth, pointsShapeSize, pointsStyleIndex, pointsEnabled]
                = data->m_getStyleOfPoints();
        m_widPoints->m_setValues(pointsColor, pointsWidth, pointsShapeSize, pointsStyleIndex, pointsEnabled);
        if (!data->m_getHasPoints())
            m_widPoints->setVisible(false);
    // Area
        auto [areaColor, areaStyleIndex, areaEnabled]
                = data->m_getStyleOfArea();
        m_widArea->m_setValues(areaColor, areaStyleIndex, areaEnabled);
        if (!data->m_getHasArea())
            m_widArea->setVisible(false);
    // Column
        auto [columnWidth, columnEnabled] = data->m_getStyleOfColumns();
        m_widColumn->m_setValues(columnWidth, columnEnabled);
        if (!data->m_getHasColumn())
            m_widColumn->setVisible(false);
    // Legend
        auto [legendOverwrite, legendText, legendEnabled]
                = data->m_getStyleOfLegend();
        m_widLegend->m_setValues(legendOverwrite, legendText, legendEnabled);
        if (!data->m_getHasLegend())
            m_widLegend->setVisible(false);
 }

void widGraphObjectSettingMain::m_saveValues()
{
    auto data = ptr_data.lock();
    // Curve
        auto [curveColor, curveWidth, curveStyleIndex, curveEnabled] = m_widCurve->m_getValues();
        data->m_setStyleOfCurve(curveColor, curveWidth, curveStyleIndex, curveEnabled);
    // Points
        auto [pointsColor, pointsWidth, pointsShapeSize, pointsStyleIndex, pointsEnabled] = m_widPoints->m_getValues();
        data->m_setStyleOfPoints(pointsColor, pointsWidth, pointsShapeSize, pointsStyleIndex, pointsEnabled);
    // Area
        auto [areaColor, areaStyleIndex, areaEnabled] = m_widArea->m_getValues();
        data->m_setStyleOfArea(areaColor, areaStyleIndex, areaEnabled);
    // Column
        auto [columnWidth, columnEnabled] = m_widColumn->m_getValues();
        data->m_setStyleOfColumn(columnWidth, columnEnabled);
    // Column
        auto [legendOverwrite, legendText, legendEnabled] = m_widLegend->m_getValues();
        data->m_setStyleOfLegend(legendOverwrite, legendText, legendEnabled);
 }

colorPicker::colorPicker()
{
    HBoxLayout *lay = new HBoxLayout(this);
    m_button = new colorButton(Qt::blue);
    lay->addWidget(m_button);

    m_menu = new QMenu(this);
    VBoxLayout *menuLayout = new VBoxLayout(m_menu);
    QGridLayout *layGrid = new QGridLayout();
    layGrid->setContentsMargins(0,0,0,0);
    layGrid->setSpacing(1);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            auto *ptr_button = new colorButton(prettyColors[3*i+j]);
            connect(ptr_button, &colorButton::m_signalSelected,
                    this, &colorPicker::m_slotColorSelected);
            layGrid->addWidget(ptr_button, i, j);
        }
    menuLayout->addLayout(layGrid);
    m_customColor = new widCustomColor();
    connect(m_customColor, &widCustomColor::m_signalColorSelected,
            this, &colorPicker::m_slotColorSelected);
    menuLayout->addWidget(m_customColor);
    connect(m_button, &QAbstractButton::pressed, this,
            &colorPicker::m_slotShowMenu);
}

void colorPicker::m_slotShowMenu()
{
    m_customColor->m_setColor(m_color);
    m_menu->move(mapToGlobal(m_button->pos() + QPoint(0, m_button->height())));
    m_menu->setFixedSize(150,100);
    m_menu->show();
}

void colorPicker::m_slotColorSelected(QColor color)
{
    m_menu->hide();
    m_color = color;
    m_button->m_setColor(m_color);
}

void colorPicker::m_slotColorConfirmed()
{
}

void colorPicker::m_setColor(const QColor &color)
{
    m_color = color;
    m_button->m_setColor(color);
}

colorButton::colorButton(const QColor &color):
    m_color(color)

{
    setFlat(true);
    m_setColor(m_color);
    connect(this, &QAbstractButton::clicked,
            this, &colorButton::m_slotClicked);
}

void colorButton::m_setColor(const QColor &color)
{
    m_color = color;
    QString tempColor = QString::number(m_color.red()) + "," +
                        QString::number(m_color.green()) + "," +
                        QString::number(m_color.blue());
    setStyleSheet("background: rgb(" + tempColor + ");"
                  "border: 1px solid gray;");
}

void colorButton::m_slotClicked()
{
    emit m_signalSelected(m_color);
}

widCustomColor::widCustomColor()
{
    m_editR = new lineEdit(validator::INT_POS_0);
    m_editG = new lineEdit(validator::INT_POS_0);
    m_editB = new lineEdit(validator::INT_POS_0);
    connect(m_editR, &lineEdit::m_signalTextFinished,
            this, &widCustomColor::m_slotLineEditChanged);
    connect(m_editG, &lineEdit::m_signalTextFinished,
            this, &widCustomColor::m_slotLineEditChanged);
    connect(m_editB, &lineEdit::m_signalTextFinished,
            this, &widCustomColor::m_slotLineEditChanged);

    m_button = new colorButton(Qt::black);
    connect(m_button, &colorButton::m_signalSelected,
            this, &widCustomColor::m_slotColorSelected);

    HBoxLayout *layBackground = new HBoxLayout(this);
    layBackground->addWidget(m_editR);
    layBackground->addWidget(m_editG);
    layBackground->addWidget(m_editB);
    layBackground->addWidget(m_button);
}

void widCustomColor::m_setColor(const QColor &color)
{
    m_color = color;
    m_button->m_setColor(color);
    m_editR->m_setNumber(m_color.red());
    m_editG->m_setNumber(m_color.green());
    m_editB->m_setNumber(m_color.blue());
}

void widCustomColor::m_slotLineEditChanged()
{
    QColor color(m_editR->m_number(),
                 m_editG->m_number(),
                 m_editB->m_number());
    m_color = color;
    m_button->m_setColor(color);
}

void widCustomColor::m_slotColorSelected()
{
    emit m_signalColorSelected(m_color);
}

widGraphObjectSettingCurve::widGraphObjectSettingCurve()
{
    HBoxLayout *lay = new HBoxLayout(this);
    lay->addSpacing(2);
//    m_labTitle = new label("Curve: ", true);
    m_checkEnable = new checkbox("Curve: ");
    connect(m_checkEnable, &QCheckBox::toggled,
            this, &widGraphObjectSettingCurve::m_slotEnabledToggled);
    m_colorPickerCurve = new colorPicker();
    m_editCurveThick = new spinbox();
    m_comboCurveStyle = new combobox(65);

    int iconWidth = 40;
    int iconHeight = 15;
    auto vOptions = m_getIconsForCurve(iconWidth, iconHeight);
    m_comboCurveStyle->m_addItems(vOptions, QSize(iconWidth, iconHeight), false);

    lay->addWidget(m_checkEnable);
    lay->addSpacing(1);
    lay->addWidget(m_colorPickerCurve);
    lay->addWidget(m_editCurveThick);
    lay->addWidget(m_comboCurveStyle);
    lay->addSpacing(3);
    lay->addStretch();
}

void widGraphObjectSettingCurve::m_setValues(QColor color, int width, int styleIndex, bool enable)
{
    m_colorPickerCurve->m_setColor(color);
    m_editCurveThick->setValue(width);
    m_comboCurveStyle->setCurrentIndex(styleIndex);
    m_checkEnable->m_setChecked(enable);
    m_slotEnabledToggled();
}

std::tuple<QColor, int, int, bool> widGraphObjectSettingCurve::m_getValues()
{
    QColor color = m_colorPickerCurve->m_getColor();
    int width = m_editCurveThick->value();
    int styleIndex = m_comboCurveStyle->currentIndex();
    bool enable = m_checkEnable->isChecked();
    return {color, width, styleIndex, enable};
}

void widGraphObjectSettingCurve::m_slotEnabledToggled()
{
    bool enabled = m_checkEnable->isChecked();
    m_colorPickerCurve->setEnabled(enabled);
    m_editCurveThick->setEnabled(enabled);
    m_comboCurveStyle->setEnabled(enabled);
}

QVector<std::tuple<QString, QIcon> > widGraphObjectSetting::m_getIconsForCurve(int iconWidth, int iconHeight)
{
    QRect rect(0,0,iconWidth,iconHeight);
    QPixmap pixmap(rect.size());
    painterAntiAl painter(&pixmap);
    painter.setPen(QPen(Qt::black, 2));

    QLine line(0, iconHeight/2, iconWidth, iconHeight/2);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.setPen(QPen(Qt::black, 2, Qt::NoPen));
    painter.drawLine(line);
    QIcon iconNone(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter.drawLine(line);
    QIcon iconSolid(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.setPen(QPen(Qt::black, 2, Qt::DashLine));
    painter.drawLine(line);
    QIcon iconDash(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.setPen(QPen(Qt::black, 2, Qt::DotLine));
    painter.drawLine(line);
    QIcon iconDot(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.setPen(QPen(Qt::black, 2, Qt::DashDotLine));
    painter.drawLine(line);
    QIcon iconDashDot(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.setPen(QPen(Qt::black, 2, Qt::DashDotDotLine));
    painter.drawLine(line);
    QIcon iconDashDotDot(pixmap);

    auto none = std::tuple<QString, QIcon>("None",iconNone);
    auto solid = std::tuple<QString, QIcon>("Solid",iconSolid);
    auto dash = std::tuple<QString, QIcon>("Dash",iconDash);
    auto dot = std::tuple<QString, QIcon>("Dot",iconDot);
    auto dashDot = std::tuple<QString, QIcon>("Dash dot",iconDashDot);
    auto dashDotDot = std::tuple<QString, QIcon>("Dash dot dot",iconDashDotDot);
    return {none, solid, dash, dot, dashDot, dashDotDot};
}

widGraphObjectSettingPoints::widGraphObjectSettingPoints()
{
    HBoxLayout *lay = new HBoxLayout(this);
    lay->addSpacing(2);
    m_checkEnable = new checkbox("Points: ");
    connect(m_checkEnable, &QCheckBox::toggled,
            this, &widGraphObjectSettingPoints::m_slotEnabledToggled);
    m_colorPickerPoints = new colorPicker();
    m_editThickness = new spinbox();
    m_editShapeSize = new spinbox();
    m_comboShape = new combobox(50);

    int iconSize = 17;
    auto vOptions = m_getIconsForPoints(iconSize);
    m_comboShape->m_addItems(vOptions, QSize(iconSize,iconSize), false);

    lay->addWidget(m_checkEnable);
    lay->addSpacing(1);
    lay->addWidget(m_colorPickerPoints);
    lay->addWidget(m_editThickness);
    lay->addWidget(m_editShapeSize);
    lay->addWidget(m_comboShape);
    lay->addSpacing(3);
    lay->addStretch();
}

void widGraphObjectSettingPoints::m_setValues(QColor color, int width, int shapeSize, int styleIndex, bool enable)
{
    m_colorPickerPoints->m_setColor(color);
    m_editThickness->setValue(width);
    m_editShapeSize->setValue(shapeSize);
    m_comboShape->setCurrentIndex(styleIndex);
    m_checkEnable->m_setChecked(enable);
    m_slotEnabledToggled();
}

std::tuple<QColor, int, int, int, bool> widGraphObjectSettingPoints::m_getValues()
{
    QColor color = m_colorPickerPoints->m_getColor();
    int width = m_editThickness->value();
    int shapeSize = m_editShapeSize->value();
    int styleIndex = m_comboShape->currentIndex();
    bool enable = m_checkEnable->isChecked();
    return {color, width, shapeSize, styleIndex, enable};
}

void widGraphObjectSettingPoints::m_slotEnabledToggled()
{
    bool enabled = m_checkEnable->isChecked();
    m_colorPickerPoints->setEnabled(enabled);
    m_editThickness->setEnabled(enabled);
    m_editShapeSize->setEnabled(enabled);
    m_comboShape->setEnabled(enabled);
}

QVector<std::tuple<QString, QIcon> > widGraphObjectSetting::m_getIconsForPoints(int iconSize)
{
    QRect rect(0,0,iconSize,iconSize);
    QPoint mid = rect.center();
    QPixmap pixmap(rect.size());
    painterAntiAl painter(&pixmap);
    painter.setPen(QPen(Qt::black, 2));

    painter.fillRect(pixmap.rect(), Qt::white);
    painter.drawPath(graphObjects::m_createPoint(mid, iconSize - 3, pointsShapes::NONE));
    QIcon iconNone(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.drawPath(graphObjects::m_createPoint(mid, iconSize - 3, pointsShapes::POINT));
    QIcon iconPoint(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.drawPath(graphObjects::m_createPoint(mid, iconSize - 3, pointsShapes::CROSS));
    QIcon iconCross(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.drawPath(graphObjects::m_createPoint(mid, iconSize - 3, pointsShapes::SQUARE));
    QIcon iconRect(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.drawPath(graphObjects::m_createPoint(mid, iconSize - 3, pointsShapes::CIRCLE));
    QIcon iconCircle(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.drawPath(graphObjects::m_createPoint(mid, iconSize - 3, pointsShapes::TRIANGLE));
    QIcon iconTriangle(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.drawPath(graphObjects::m_createPoint(mid, iconSize - 3, pointsShapes::TRIANGLE_REV));
    QIcon iconRevTriangle(pixmap);

    auto none = std::tuple<QString, QIcon>("None",iconNone);
    auto point = std::tuple<QString, QIcon>("Point",iconPoint);
    auto cross = std::tuple<QString, QIcon>("Cross",iconCross);
    auto square = std::tuple<QString, QIcon>("Square",iconRect);
    auto circle = std::tuple<QString, QIcon>("Circle",iconCircle);
    auto triangle = std::tuple<QString, QIcon>("Triangle",iconTriangle);
    auto revTriangle = std::tuple<QString, QIcon>("Rev. triangle",iconRevTriangle);

    return {none, point, cross, square, circle, triangle, revTriangle};
}

widGraphObjectSettingArea::widGraphObjectSettingArea()
{
    HBoxLayout *lay = new HBoxLayout(this);
    lay->addSpacing(2);
    m_checkEnable = new checkbox("Area: ");
    connect(m_checkEnable, &QCheckBox::toggled,
            this, &widGraphObjectSettingArea::m_slotEnabledToggled);
    m_colorPickerArea = new colorPicker();
    m_comboAreaStyle = new combobox(65);

    int iconWidth = 40;
    int iconHeight = 15;
    auto vOptions = m_getIconsForArea(iconWidth, iconHeight);
    m_comboAreaStyle->m_addItems(vOptions, QSize(iconWidth, iconHeight), false);

    lay->addWidget(m_checkEnable);
    lay->addSpacing(1);
    lay->addWidget(m_colorPickerArea);
    lay->addWidget(m_comboAreaStyle);
    lay->addSpacing(3);
    lay->addStretch();
}

void widGraphObjectSettingArea::m_setValues(QColor color, int styleIndex, bool enable)
{
    m_colorPickerArea->m_setColor(color);
    m_comboAreaStyle->setCurrentIndex(styleIndex);
    m_checkEnable->m_setChecked(enable);
    m_slotEnabledToggled();
}

std::tuple<QColor, int, bool> widGraphObjectSettingArea::m_getValues()
{
    QColor color = m_colorPickerArea->m_getColor();
    int styleIndex = m_comboAreaStyle->currentIndex();
    bool enable = m_checkEnable->isChecked();
    return {color, styleIndex, enable};
}

void widGraphObjectSettingArea::m_slotEnabledToggled()
{
    bool enabled = m_checkEnable->isChecked();
    m_colorPickerArea->setEnabled(enabled);
    m_comboAreaStyle->setEnabled(enabled);
}

QVector<std::tuple<QString, QIcon> > widGraphObjectSetting::m_getIconsForArea(int iconWidth, int iconHeight)
{
    QRect rect(0,0,iconWidth,iconHeight);
    QRect rect2(1,1,iconWidth-2,iconHeight-2);
    QPixmap pixmap(rect.size());
    painterAntiAl painter(&pixmap);
    painter.setPen(QPen(Qt::black, 1));

    painter.fillRect(pixmap.rect(), Qt::white);
    painter.fillRect(rect, QBrush(Qt::black, Qt::NoBrush));
    painter.drawRect(rect2);
    QIcon iconNone(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.fillRect(rect, QBrush(Qt::black, Qt::SolidPattern));
    painter.drawRect(rect2);
    QIcon iconSolid(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.fillRect(rect, QBrush(Qt::black, Qt::Dense1Pattern));
    painter.drawRect(rect2);
    QIcon iconDense1(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.fillRect(rect, QBrush(Qt::black, Qt::Dense2Pattern));
    painter.drawRect(rect2);
    QIcon iconDense2(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.fillRect(rect, QBrush(Qt::black, Qt::Dense3Pattern));
    painter.drawRect(rect2);
    QIcon iconDense3(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.fillRect(rect, QBrush(Qt::black, Qt::Dense4Pattern));
    painter.drawRect(rect2);
    QIcon iconDense4(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.fillRect(rect, QBrush(Qt::black, Qt::Dense5Pattern));
    painter.drawRect(rect2);
    QIcon iconDense5(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.fillRect(rect, QBrush(Qt::black, Qt::Dense6Pattern));
    painter.drawRect(rect2);
    QIcon iconDense6(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.fillRect(rect, QBrush(Qt::black, Qt::Dense7Pattern));
    painter.drawRect(rect2);
    QIcon iconDense7(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.fillRect(rect, QBrush(Qt::black, Qt::VerPattern));
    painter.drawRect(rect2);
    QIcon iconHorizontal(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.fillRect(rect, QBrush(Qt::black, Qt::HorPattern));
    painter.drawRect(rect2);
    QIcon iconVertical(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.fillRect(rect, QBrush(Qt::black, Qt::CrossPattern));
    painter.drawRect(rect2);
    QIcon iconCross(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.fillRect(rect, QBrush(Qt::black, Qt::BDiagPattern));
    painter.drawRect(rect2);
    QIcon iconBackDiagonal(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.fillRect(rect, QBrush(Qt::black, Qt::FDiagPattern));
    painter.drawRect(rect2);
    QIcon iconForwardDiagonal(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.fillRect(rect, QBrush(Qt::black, Qt::DiagCrossPattern));
    painter.drawRect(rect2);
    QIcon iconCrossDiagonal(pixmap);

    auto none = std::tuple<QString, QIcon>("None",iconNone);
    auto solid = std::tuple<QString, QIcon>("Solid",iconSolid);
    auto dense1 = std::tuple<QString, QIcon>("Dense 1",iconDense1);
    auto dense2 = std::tuple<QString, QIcon>("Dense 2",iconDense2);
    auto dense3 = std::tuple<QString, QIcon>("Dense 3",iconDense3);
    auto dense4 = std::tuple<QString, QIcon>("Dense 4",iconDense4);
    auto dense5 = std::tuple<QString, QIcon>("Dense 5",iconDense5);
    auto dense6 = std::tuple<QString, QIcon>("Dense 6",iconDense6);
    auto dense7 = std::tuple<QString, QIcon>("Dense 7",iconDense7);
    auto horizontal = std::tuple<QString, QIcon>("Horizontal",iconHorizontal);
    auto vertical = std::tuple<QString, QIcon>("Vertical",iconVertical);
    auto cross = std::tuple<QString, QIcon>("Cross",iconCross);
    auto bdiagonal = std::tuple<QString, QIcon>("Back diagonal",iconBackDiagonal);
    auto fDiagonal = std::tuple<QString, QIcon>("Forward diagonal",iconForwardDiagonal);
    auto crossDiagonal = std::tuple<QString, QIcon>("Cross diagonal",iconCrossDiagonal);

    return {none, solid, dense1, dense2, dense3, dense4, dense5, dense6, dense7,
            horizontal, vertical, cross, bdiagonal, fDiagonal, crossDiagonal};
}


widGraphObjectSettingColumn::widGraphObjectSettingColumn()
{
    HBoxLayout *lay = new HBoxLayout(this);
    lay->addSpacing(2);
    m_checkEnable = new checkbox("Column: ");
    connect(m_checkEnable, &QCheckBox::toggled,
            this, &widGraphObjectSettingColumn::m_slotEnabledToggled);
    m_editColumnThick = new spinbox();

    lay->addWidget(m_checkEnable);
    lay->addSpacing(1);
    lay->addWidget(m_editColumnThick);
    lay->addSpacing(3);
    lay->addStretch();
}

void widGraphObjectSettingColumn::m_setValues(int width, bool enable)
{
    m_editColumnThick->setValue(width);
    m_checkEnable->m_setChecked(enable);
    m_slotEnabledToggled();
}

std::tuple<int, bool> widGraphObjectSettingColumn::m_getValues()
{
    int width = m_editColumnThick->value();
    bool enable = m_checkEnable->isChecked();
    return {width, enable};
}

void widGraphObjectSettingColumn::m_slotEnabledToggled()
{
    bool enabled = m_checkEnable->isChecked();
    m_editColumnThick->setEnabled(enabled);
}

widGraphObjectSettingLegend::widGraphObjectSettingLegend()
{
    HBoxLayout *lay = new HBoxLayout(this);
    lay->addSpacing(2);
    m_checkEnable = new checkbox("Legend: ");
    connect(m_checkEnable, &QCheckBox::toggled,
            this, &widGraphObjectSettingLegend::m_slotEnabledToggled);
    m_editText = new checkEdit(validator::NONE);

    lay->addWidget(m_checkEnable);
    lay->addSpacing(1);
    lay->addWidget(m_editText);
    lay->addSpacing(3);
    lay->addStretch();
}

void widGraphObjectSettingLegend::m_setValues(bool overwrite, const std::string &text, bool enable)
{
    m_editText->m_setText(overwrite, text);
    m_checkEnable->m_setChecked(enable);
    m_slotEnabledToggled();
}

std::tuple<bool, std::string, bool> widGraphObjectSettingLegend::m_getValues()
{
    bool overwrite = m_editText->m_getChecked();
    std::string text = m_editText->m_getText();
    bool enable = m_checkEnable->isChecked();
    return {overwrite, text, enable};
}

void widGraphObjectSettingLegend::m_slotEnabledToggled()
{
    bool enabled = m_checkEnable->isChecked();
    m_editText->setEnabled(enabled);
}
