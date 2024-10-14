#include "dialogGraph.h"
#include "dataGraph.h"
#include "widGraph.h"

dialogGraph::dialogGraph(widGraph *graph, std::weak_ptr<dataGraph> data):
    ptr_graph(graph), ptr_data(data)
{
    m_widContent = new graphSettingsWidget(ptr_data);

    m_widFooter = new footerDialogGraph();
    connect(m_widFooter, &footerDialogGraph::m_emitDataChanged,
            this, &dialogGraph::m_slotDataChanged);
    connect(m_widFooter, &footerDialogGraph::m_emitApply,
            this, &dialogGraph::m_slotApply);
    connect(m_widFooter, &footerDialogGraph::m_emitClose,
            this, &dialogGraph::m_slotClose);
    connect(m_widFooter, &footerDialogGraph::m_emitSaveFile,
            this, &dialogGraph::m_slotSaveFile);
    connect(m_widFooter, &footerDialogGraph::m_emitLoadFile,
            this, &dialogGraph::m_slotLoadFile);

    m_layBackground = new VBoxLayout(this);
    m_layBackground->addWidget(m_widContent);
    m_layBackground->addWidget(m_widFooter);

    m_loadValues();
    resize(1024,700);
}

void dialogGraph::m_slotDataChanged()
{
    ptr_graph->m_loadValues();
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
        auto ptr_object = graphObject::m_createGraphObject(type);
        auto ptr_objectData = std::make_shared<dataGraphObject>(loadFileContent);
        ptr_object->m_setData(ptr_objectData);
      //  ptr_graph->m_addObject(ptr_object);
    }
    loadFileContent.close();
}

tabGraph::tabGraph(const QString &title)
{
    m_layBackground = new VBoxLayout(this);
    m_layBackground->setContentsMargins(2,2,2,2);
    m_layBackground->setSpacing(1);
    m_layBackground->addWidget(new label(" " + title, 14, true));
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
    m_editText = new lineedit(validator::NONE);
    m_editFontSize = new lineedit(validator::INT_POS);
    m_tree->m_addChild("Text", m_editText);
    m_tree->m_addChild("Font size", m_editFontSize);
}

void tabGraphSettingsTitle::m_loadValues()
{
    auto s_data = ptr_data.lock();
    m_editText->m_setText(QString::fromStdString(s_data->m_text));
    m_editFontSize->m_setNumber(s_data->m_fontText);
}

void tabGraphSettingsTitle::m_saveValues()
{
    auto s_data = ptr_data.lock();
    s_data->m_text = m_editText->m_text().toStdString();
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
    m_checkShowLegend = new checkbox("Show legend");
    auto *titleDimensions = m_tree->m_addChild("Dimensions", m_checkShowLegend, nullptr, false);
    m_editSize = new checkEdit(validator::INT_POS_0);
    m_tree->m_addChild("Manual dimensions", m_editSize, titleDimensions);
    m_checkShowTopLine = new checkbox();
    m_tree->m_addChild("Show top line", m_checkShowTopLine, titleDimensions);
    m_editFontSizeText = new lineedit(validator::INT_POS);
    m_tree->m_addChild("Text size", m_editFontSizeText, titleDimensions);

    m_checkAlignToAxes = new checkbox("Align to respective axis");
    connect(m_checkAlignToAxes, &QAbstractButton::toggled,
            this, &tabGraphSettingsLegend::m_slotAlignedAxesToggled);
    auto *titleArrange = m_tree->m_addChild("Arrangement", m_checkAlignToAxes, nullptr, true);
    m_editNColumns = new spinbox(0,10);
    m_tree->m_addChild("Number of columns", m_editNColumns, titleArrange);
}

void tabGraphSettingsLegend::m_loadValues()
{
    auto s_data = ptr_data.lock();
    m_editFontSizeText->m_setNumber(s_data->m_fontText);
    m_checkShowLegend->m_setChecked(s_data->m_show);
    m_checkShowTopLine->m_setChecked(s_data->m_showTopLine);
    m_editSize->m_setValues(s_data->m_manualSize, s_data->m_manualSizeValue);
    m_editNColumns->m_setValue(s_data->m_nColumns);
    m_checkAlignToAxes->m_setChecked(s_data->m_arrangeToAxes);
    m_slotAlignedAxesToggled();
}

void tabGraphSettingsLegend::m_saveValues()
{
    auto s_data = ptr_data.lock();
    s_data->m_fontText = m_editFontSizeText->m_number();
    s_data->m_show = m_checkShowLegend->isChecked();
    s_data->m_showTopLine = m_checkShowTopLine->isChecked();
    s_data->m_manualSize = m_editSize->m_getChecked();
    s_data->m_manualSizeValue = m_editSize->m_value();
    s_data->m_nColumns = m_editNColumns->value();
    s_data->m_arrangeToAxes = m_checkAlignToAxes->isChecked();
}

void tabGraphSettingsLegend::m_slotAlignedAxesToggled()
{
    bool aligned = m_checkAlignToAxes->isChecked();
    m_editNColumns->setEnabled(!aligned);
}

graphSettingsWidget::graphSettingsWidget(std::weak_ptr<dataGraph> data):
    ptr_data(data)
{
    m_layBackground = new QVBoxLayout(this);
    m_layBackground->setContentsMargins(2,2,2,2);
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
    m_objects = new tabGraphSettingsObjects(ptr_data.lock());

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
    splitMain->setSizes({300,600,600});
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
    m_butTest = new QPushButton("Test");
    m_butSaveFile = new QPushButton("Save to file");
    m_butLoadFile = new QPushButton("Load from file");
    m_butClose = new QPushButton("Close");
    m_butApply = new QPushButton("Apply");
    connect(m_butTest, &QAbstractButton::clicked,
            this, [this](){emit m_emitDataChanged();});
    connect(m_butSaveFile, &QAbstractButton::clicked, this, &footerDialogGraph::m_slotSaveFile);
    connect(m_butLoadFile, &QAbstractButton::clicked, this, &footerDialogGraph::m_slotLoadFile);
    connect(m_butClose, &QAbstractButton::clicked, this, &footerDialogGraph::m_slotClose);
    connect(m_butApply, &QAbstractButton::clicked, this, &footerDialogGraph::m_slotApply);
    m_layBackground = new HBoxLayout(this);
    m_layBackground->setContentsMargins(4,0,4,4);
    m_layBackground->setSpacing(4);
    m_layBackground->addWidget(m_butSaveFile);
    m_layBackground->addWidget(m_butLoadFile);
    m_layBackground->addWidget(m_butTest);
    m_layBackground->addStretch();
    m_layBackground->addWidget(m_butApply);
    m_layBackground->addWidget(m_butClose);
}

tabGraphSettingsAxis::tabGraphSettingsAxis(const QString &title):
    tabGraphSettings(title)
{
    // Dimensions
        m_checkShowAxis = new checkbox("Show axis");
        auto *titleDimensions = m_tree->m_addChild("Dimensions", m_checkShowAxis, nullptr, false);
        m_editSize = new checkEdit(validator::INT_POS_0);
        m_tree->m_addChild("Manual dimensions", m_editSize, titleDimensions);
        m_editFontSizeNumbers = new lineedit(validator::INT_POS);
        m_editFontSizeText = new lineedit(validator::INT_POS);
        m_tree->m_addChild("Numbers size", m_editFontSizeNumbers, titleDimensions);
        m_tree->m_addChild("Text size", m_editFontSizeText, titleDimensions);
    // Text
        m_editText = new lineedit(validator::NONE);
        m_tree->m_addChild("Text", m_editText);
    // Axis min max
        m_checkAutoAxis = new checkbox("Auto axis");
        connect(m_checkAutoAxis, &QAbstractButton::toggled,
                this, &tabGraphSettingsAxis::m_slotAutoAxisToggled);
        m_checkManualStep = new checkEdit(validator::DOUBLE);
        connect(m_checkManualStep, &checkEdit::m_signalToggled,
                this, &tabGraphSettingsAxis::m_slotAutoStepToggled);
        m_editMinMax = new lineEditedit(validator::DOUBLE);
        auto *titleMinMax = m_tree->m_addChild("Range", m_checkAutoAxis, nullptr, true);
        m_tree->m_addChild("Min/max", m_editMinMax, titleMinMax);
        m_tree->m_addChild("Manual step", m_checkManualStep, titleMinMax);
}

void tabGraphSettingsAxis::m_loadGeneralValues(std::shared_ptr<dataAxis> s_data)
{
    m_checkShowAxis->m_setChecked(s_data->m_show);
    m_editSize->m_setValues(s_data->m_manualSize, s_data->m_manualSizeValue);
    m_editFontSizeNumbers->m_setNumber(s_data->m_fontNumbers);
    m_editFontSizeText->m_setNumber(s_data->m_fontText);
    m_checkAutoAxis->m_setChecked(s_data->m_autoAxis);
    m_checkManualStep->m_setValues(s_data->m_manualStep, s_data->m_step);
    m_editMinMax->m_setValues(s_data->m_min, s_data->m_max);
    m_editText->m_setText(QString::fromStdString(s_data->m_text));
    m_slotAutoAxisToggled();
}

void tabGraphSettingsAxis::m_saveGeneralValues(std::shared_ptr<dataAxis> s_data)
{
    s_data->m_show = m_checkShowAxis->isChecked();
    s_data->m_manualSize = m_editSize->m_getChecked();
    s_data->m_manualSizeValue = m_editSize->m_value();
    s_data->m_fontNumbers = m_editFontSizeNumbers->m_number();
    s_data->m_fontText = m_editFontSizeText->m_number();
    s_data->m_autoAxis = m_checkAutoAxis->isChecked();
    s_data->m_manualStep = m_checkManualStep->m_getChecked();
    s_data->m_min = m_editMinMax->m_value1();
    s_data->m_max = m_editMinMax->m_value2();
    s_data->m_step = m_checkManualStep->m_value();
    s_data->m_text = m_editText->m_text().toStdString();
}

void tabGraphSettingsAxis::m_slotAutoAxisToggled()
{
    bool autoAxis = m_checkAutoAxis->isChecked();
    m_editMinMax->m_setEnabled(!autoAxis);
    m_checkManualStep->setEnabled(!autoAxis);
    m_slotAutoStepToggled();
}

void tabGraphSettingsAxis::m_slotAutoStepToggled()
{

}



widGraphObjectSettingCurve::widGraphObjectSettingCurve():
    widGraphObjectSettingWithCheck("Curve")
{
    m_colorPickerCurve = new colorPicker();
    connect(m_colorPickerCurve, &colorPicker::m_signalColorChanged,
            this, &widGraphObjectSettingCurve::m_slotSendSignalChanged);
    m_colorPickerCurve->setToolTip("Select curve color");
    m_editCurveThick = new spinbox();
    connect(m_editCurveThick, &spinbox::valueChanged,
            this, &widGraphObjectSettingCurve::m_slotSendSignalChanged);
    m_editCurveThick->setToolTip("Curve thickness");
    m_comboCurveStyle = new combobox(65);
    connect(m_comboCurveStyle, &combobox::currentIndexChanged,
            this, &widGraphObjectSettingCurve::m_slotSendSignalChanged);
    m_comboCurveStyle->setToolTip("Curve style");

    int iconWidth = 40;
    int iconHeight = 15;
    auto vOptions = m_getIconsForCurve(iconWidth, iconHeight);
    m_comboCurveStyle->m_addItems(vOptions, QSize(iconWidth, iconHeight), false);

    m_addWidget(m_checkEnable);
    m_addSpacing(1);
    m_addWidget(m_colorPickerCurve);
    m_addWidget(m_editCurveThick);
    m_addWidget(m_comboCurveStyle);
    m_addEndOfWidget();
}

void widGraphObjectSettingCurve::m_setValues(QColor color, int width, int styleIndex, bool enable)
{
    m_colorPickerCurve->m_setColor(color);
    m_editCurveThick->m_setValue(width);
    m_comboCurveStyle->m_setCurrentIndex(styleIndex);
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

void widGraphObjectSettingCurve::m_setEnabled(bool enabled)
{
    m_colorPickerCurve->setEnabled(enabled);
    m_editCurveThick->setEnabled(enabled);
    m_comboCurveStyle->setEnabled(enabled);
}

widGraphObjectSettingWithCheck::widGraphObjectSettingWithCheck(const QString &name)
{
    m_checkEnable = new checkbox("");
    m_checkEnable->setToolTip("Show " + name.toLower());
    connect(m_checkEnable, &QCheckBox::toggled,
            this, &widGraphObjectSettingWithCheck::m_slotEnabledToggled);
    connect(m_checkEnable, &QCheckBox::toggled,
            this, &widGraphObjectSettingWithCheck::m_slotSendSignalChanged);
    m_addWidget(m_checkEnable);
    m_addSpacing(1);
}

void widGraphObjectSetting::m_addEndOfWidget()
{
    m_layBackground->addSpacing(2);
    m_layBackground->addStretch();
}

void widGraphObjectSetting::m_addWidget(QWidget *ptr_widget)
{
    m_layBackground->addWidget(ptr_widget);
}

void widGraphObjectSetting::m_addSpacing(int nPixels)
{
    m_layBackground->addSpacing(nPixels);
}

widGraphObjectSetting::widGraphObjectSetting()
{
    QHBoxLayout *lay = new QHBoxLayout();
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    setLayout(lay);
    m_widBackground = new QWidget();
    lay->addWidget(m_widBackground);
    m_layBackground = new HBoxLayout(m_widBackground);
    m_layBackground->setSpacing(1);
  //  m_layBackground->addWidget(m_separator());
    m_layBackground->addSpacing(1);
}

void widGraphObjectSetting::m_setVisible(bool status)
{
    m_widBackground->setVisible(status);
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

widGraphObjectSettingPoints::widGraphObjectSettingPoints():
    widGraphObjectSettingWithCheck("Points")
{
    m_colorPickerPoints = new colorPicker();
    connect(m_colorPickerPoints, &colorPicker::m_signalColorChanged,
            this, &widGraphObjectSettingPoints::m_slotSendSignalChanged);
    m_colorPickerPoints->setToolTip("Select points color");
    m_editThickness = new spinbox();
    connect(m_editThickness, &spinbox::valueChanged,
            this, &widGraphObjectSettingPoints::m_slotSendSignalChanged);
    m_editThickness->setToolTip("Set pen thickness");
    m_editShapeSize = new spinbox();
    connect(m_editShapeSize, &spinbox::valueChanged,
            this, &widGraphObjectSettingPoints::m_slotSendSignalChanged);
    m_editShapeSize->setToolTip("Set size of point");
    m_comboShape = new combobox(50);
    connect(m_comboShape, &combobox::currentIndexChanged,
            this, &widGraphObjectSettingPoints::m_slotSendSignalChanged);
    m_comboShape->setToolTip("Select shape of points");

    int iconSize = 17;
    auto vOptions = m_getIconsForPoints(iconSize);
    m_comboShape->m_addItems(vOptions, QSize(iconSize,iconSize), false);

    m_addWidget(m_colorPickerPoints);
    m_addWidget(m_editThickness);
    m_addWidget(m_editShapeSize);
    m_addWidget(m_comboShape);
    m_addEndOfWidget();
}

void widGraphObjectSettingPoints::m_setValues(QColor color, int width, int shapeSize, int styleIndex, bool enable)
{
    m_colorPickerPoints->m_setColor(color);
    m_editThickness->m_setValue(width);
    m_editShapeSize->m_setValue(shapeSize);
    m_comboShape->m_setCurrentIndex(styleIndex);
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

void widGraphObjectSettingPoints::m_setEnabled(bool enabled)
{
    m_colorPickerPoints->setEnabled(enabled);
    m_editThickness->setEnabled(enabled);
    m_editShapeSize->setEnabled(enabled);
    m_comboShape->setEnabled(enabled);
}
void widGraphObjectSettingWithCheck::m_slotEnabledToggled()
{
    m_checkEnable->setEnabled(true);
    bool enabled = m_checkEnable->isChecked();
    m_setEnabled(enabled);
}

QVector<std::tuple<QString, QIcon> > widGraphObjectSetting::m_getIconsForPoints(int iconSize)
{
    QRect rect(0,0,iconSize,iconSize);
    QPoint mid = rect.center();
    QPixmap pixmap(rect.size());
    painterAntiAl painter(&pixmap);
    painter.setPen(QPen(Qt::black, 2));

    painter.fillRect(pixmap.rect(), Qt::white);
    painter.drawPath(graphObject::m_createPoint(mid, iconSize - 3, pointsShapes::NONE));
    QIcon iconNone(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.drawPath(graphObject::m_createPoint(mid, iconSize - 3, pointsShapes::POINT));
    QIcon iconPoint(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.drawPath(graphObject::m_createPoint(mid, iconSize - 3, pointsShapes::CROSS));
    QIcon iconCross(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.drawPath(graphObject::m_createPoint(mid, iconSize - 3, pointsShapes::SQUARE));
    QIcon iconRect(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.drawPath(graphObject::m_createPoint(mid, iconSize - 3, pointsShapes::CIRCLE));
    QIcon iconCircle(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.drawPath(graphObject::m_createPoint(mid, iconSize - 3, pointsShapes::TRIANGLE));
    QIcon iconTriangle(pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.drawPath(graphObject::m_createPoint(mid, iconSize - 3, pointsShapes::TRIANGLE_REV));
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

widGraphObjectSettingArea::widGraphObjectSettingArea():
    widGraphObjectSettingWithCheck("Area")
{
    m_colorPickerArea = new colorPicker();
    connect(m_colorPickerArea, &colorPicker::m_signalColorChanged,
            this, &widGraphObjectSettingArea::m_slotSendSignalChanged);
    m_colorPickerArea->setToolTip("Select color of area below curve");
    m_comboAreaStyle = new combobox(65);
    connect(m_comboAreaStyle, &combobox::currentIndexChanged,
            this, &widGraphObjectSettingArea::m_slotSendSignalChanged);
    m_comboAreaStyle->setToolTip("Select style of area filling");

    int iconWidth = 40;
    int iconHeight = 15;
    auto vOptions = m_getIconsForArea(iconWidth, iconHeight);
    m_comboAreaStyle->m_addItems(vOptions, QSize(iconWidth, iconHeight), false);

    m_addWidget(m_colorPickerArea);
    m_addWidget(m_comboAreaStyle);
    m_addEndOfWidget();
}

void widGraphObjectSettingArea::m_setValues(QColor color, int styleIndex, bool enable)
{
    m_colorPickerArea->m_setColor(color);
    m_comboAreaStyle->m_setCurrentIndex(styleIndex);
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

void widGraphObjectSettingArea::m_setEnabled(bool enabled)
{
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

QWidget *widGraphObjectSetting::m_separator()
{
    QWidget *separator = new QWidget();
    separator->setStyleSheet("background:black;");
    separator->setFixedWidth(1);
    return separator;
}

void widGraphObjectSetting::m_slotSendSignalChanged()
{
    emit m_signalChanged();
}

widGraphObjectSettingColumn::widGraphObjectSettingColumn():
    widGraphObjectSettingWithCheck("Column")
{
    m_editColumnThick = new spinbox();
    m_editColumnThick->setToolTip("Set column thickness");
    connect(m_editColumnThick, &spinbox::valueChanged,
            this, &widGraphObjectSettingColumn::m_slotSendSignalChanged);
    m_addWidget(m_editColumnThick);
    m_addEndOfWidget();
}

void widGraphObjectSettingColumn::m_setValues(int width, bool enable)
{
    m_editColumnThick->m_setValue(width);
    m_checkEnable->m_setChecked(enable);
    m_slotEnabledToggled();
}

std::tuple<int, bool> widGraphObjectSettingColumn::m_getValues()
{
    int width = m_editColumnThick->value();
    bool enable = m_checkEnable->isChecked();
    return {width, enable};
}

void widGraphObjectSettingColumn::m_setEnabled(bool enabled)
{
    m_editColumnThick->setEnabled(enabled);
}

widGraphObjectSettingLegend::widGraphObjectSettingLegend():
    widGraphObjectSettingWithCheck("Legend")
{
    m_editText = new checkEdit(validator::NONE);
    m_editText->setToolTip("Overwrite text in legend");
    connect(m_editText, &checkEdit::m_signalToggled,
            this, &widGraphObjectSettingLegend::m_slotSendSignalChanged);
    connect(m_editText, &checkEdit::m_signalEditingFinished,
            this, &widGraphObjectSettingLegend::m_slotSendSignalChanged);
    m_addWidget(m_editText);
    m_addEndOfWidget();
}

void widGraphObjectSettingLegend::m_setValues(bool overwrite, const std::string &text, bool enable)
{
    m_editText->m_setText(overwrite, QString::fromStdString(text));
    m_checkEnable->m_setChecked(enable);
    m_slotEnabledToggled();
}

std::tuple<bool, std::string, bool> widGraphObjectSettingLegend::m_getValues()
{
    bool overwrite = m_editText->m_getChecked();
    std::string text = m_editText->m_getText().toStdString();
    bool enable = m_checkEnable->isChecked();
    return {overwrite, text, enable};
}

void widGraphObjectSettingLegend::m_setEnabled(bool enabled)
{
    m_editText->setEnabled(enabled);
}

treeWidgetGraphObjects::treeWidgetGraphObjects()
{
    setColumnCount(9);
    setHeaderLabels({"Object name", "Operations", "Y Axis", "Type",  "Curve", "Points", "Area", "Legend", "Style"});
    QFont font;
    font.setBold(true);
    font.setPixelSize(12);
    header()->setFont(font);
    header()->setStyleSheet("border:none; border-bottom:1px solid black;");
    setColumnWidth(0, 150);
    setColumnWidth(1, 75);
    setColumnWidth(2, 60);
    setIndentation(8);
    setStyleSheet("QTreeWidget::item {"
                      "padding-top: 1px 0;"
                      "height: 25px;"
                  "}");
    setFocusPolicy(Qt::NoFocus);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(QAbstractItemView::InternalMove);
}

void treeWidgetGraphObjects::dropEvent(QDropEvent *event)
{
    if (event->source() == this) {
        auto *itemFrom = selectedItems()[0];
        int indexFrom = indexOfTopLevelItem(itemFrom);
        auto *itemTo = itemAt(event->position().toPoint());
        int indexTo = indexOfTopLevelItem(itemTo);
        emit m_signalMoved(indexFrom, indexTo);
    }
    event->accept();
}

QTreeWidgetItem *treeWidgetGraphObjects::m_addChild(const std::string &text,
  QVector<QWidget *> widgets,
  const std::string &tooltip)
{
    // Create item
        QTreeWidgetItem *item = new QTreeWidgetItem(this);
    // First
        QFont font;
        font.setBold(false);
        font.setPixelSize(12);
        item->setFont(0, font);
        item->setText(0, QString::fromStdString(text));
        std::string tooltipText = tooltip;
        if (tooltipText == "")
            tooltipText = text;
        item->setToolTip(0, QString::fromStdString(tooltipText));

    // Operation
    setItemWidget(item, 1, widgets[0]);
    // Axis
    setItemWidget(item, 2, widgets[1]);
    // Axis
    setItemWidget(item, 3, widgets[2]);
    // Axis
    setItemWidget(item, 4, widgets[3]);
    // Axis
    setItemWidget(item, 5, widgets[4]);
    // Axis
    setItemWidget(item, 6, widgets[5]);
    // Axis
    setItemWidget(item, 7, widgets[6]);
    // Fourth
        setItemWidget(item, 8, widgets[7]);
    return item;
}

tabGraphSettings::tabGraphSettings(const QString &name):
    tabGraph(name)
{
    m_tree = new treeWidget();
    m_layBackground->addWidget(m_tree);
}

widGraphObjectSettingOperation::widGraphObjectSettingOperation() //:
//     m_positionInVector(pos), ptr_widObjectStyle(ptr_widMain)
{
    m_layBackground = new HBoxLayout(this);
    m_butDelete = new butGraphObjectSettingButton(QIcon(":/images/redCross.png"), "Delete this object");
    m_butDelete->setCheckable(true);
    connect(m_butDelete, &QAbstractButton::pressed,
            this, &widGraphObjectSettingOperation::m_slotDeleteClicked);
    m_butMoveUp = new butGraphObjectSettingButton(QIcon(":/images/moveUp.png"), "Move object up");
    connect(m_butMoveUp, &QAbstractButton::pressed,
            this, &widGraphObjectSettingOperation::m_slotMoveUp);
    m_butMoveDown = new butGraphObjectSettingButton(QIcon(":/images/moveDown.png"), "Move object down");
    connect(m_butMoveDown, &QAbstractButton::pressed,
            this, &widGraphObjectSettingOperation::m_slotMoveDown);
    m_layBackground->addWidget(m_butMoveUp);
    m_layBackground->addWidget(m_butMoveDown);
    m_layBackground->addWidget(m_butDelete);
    m_layBackground->addStretch();
}

void widGraphObjectSettingOperation::m_setValues(bool enable)
{
    m_butDelete->setChecked(enable);
//    ptr_widObjectStyle->setEnabled(!enable);
}

std::tuple<bool> widGraphObjectSettingOperation::m_getValues()
{
    return {m_butDelete->isChecked()};
}

void widGraphObjectSettingOperation::m_loadValues()
{
    /*auto ptr_data = ptr_widObjectStyle->m_getData().lock();
    auto [toBeDeleted] = ptr_data->m_getOperation();
    m_setValues(toBeDeleted);*/
}

void widGraphObjectSettingOperation::m_saveValues()
{
    /*auto ptr_data = ptr_widObjectStyle->m_getData().lock();
    auto [toBeDeleted] = m_getValues();
    ptr_data->m_setOperation(toBeDeleted);*/
}

void widGraphObjectSettingOperation::m_setEnabled(bool /*enabled*/)
{

}

void widGraphObjectSettingOperation::m_slotDeleteClicked()
{
    m_saveValues();
/*    if (ptr_widObjectStyle)
        ptr_widObjectStyle->setEnabled(m_butDelete->isChecked());
    else
        qDebug() << "No widget";*/
}

void widGraphObjectSettingOperation::m_slotMoveUp()
{
    emit m_signalMoveUp(m_positionInVector);
}

void widGraphObjectSettingOperation::m_slotMoveDown()
{
    emit m_signalMoveDown(m_positionInVector);
}

butGraphObjectSettingButton::butGraphObjectSettingButton(QIcon icon, const QString &tooltip)
{
    setIcon(icon);
    setFixedSize(QSize(25,25));
    setIconSize(QSize(20,20));
    setToolTip(tooltip);
}

widGraphObjectSettingAxis::widGraphObjectSettingAxis()
{
    m_state = yAxisPosition::LEFT;
    m_radioLeft = new radiobutton("L");
    connect(m_radioLeft, &radiobutton::toggled,
            this, &widGraphObjectSettingAxis::m_slotSendSignal);
    m_radioLeft->setToolTip("Left y axis");
    m_radioRight = new radiobutton("R");
    connect(m_radioRight, &radiobutton::toggled,
            this, &widGraphObjectSettingAxis::m_slotSendSignal);
    m_radioRight->setToolTip("Right y axis");
    m_addWidget(m_radioLeft);
    m_addWidget(m_radioRight);
    m_addEndOfWidget();
}

void widGraphObjectSettingAxis::m_setValues(yAxisPosition orient)
{
    m_state = orient;
    m_radioLeft->m_setChecked(m_state == yAxisPosition::LEFT);
    m_radioRight->m_setChecked(m_state == yAxisPosition::RIGHT);
}

std::tuple<yAxisPosition> widGraphObjectSettingAxis::m_getValues()
{
    return {m_state};
}

void widGraphObjectSettingAxis::m_setEnabled(bool enabled)
{
    m_radioLeft->setEnabled(enabled);
    m_radioRight->setEnabled(enabled);
}

void widGraphObjectSettingAxis::m_slotSendSignal(bool status)
{
    if (status) {
        switch (m_state) {
            case yAxisPosition::LEFT:
                m_state = yAxisPosition::RIGHT;
                break;
            case yAxisPosition::RIGHT:
                m_state = yAxisPosition::LEFT;
                break;
        }
        emit m_signalChanged();
    }
}

objectPropertiesTableModel::objectPropertiesTableModel(std::shared_ptr<dataGraph> ptr_dataGraph):
    ptr_data(ptr_dataGraph)
{

}

int objectPropertiesTableModel::rowCount(const QModelIndex &/*parent*/) const
{return ptr_data.lock()->m_vectorOfObjects.size();}

int objectPropertiesTableModel::columnCount(const QModelIndex &/*parent*/) const
{return 7;}

QVariant objectPropertiesTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    auto ptr_objectData = ptr_data.lock()->m_vectorOfObjects[row]->m_getData().lock();
    auto columnType = m_getEnumFromIndex(col);
    switch (columnType) {
      case objectPropertiesColumns::NAME:
        return m_name(role, ptr_objectData);
      case objectPropertiesColumns::YAXIS:
        return m_yAxis(role, ptr_objectData);
      case objectPropertiesColumns::CURVE:
        return m_curve(role, ptr_objectData);
      case objectPropertiesColumns::POINTS:
        return m_points(role, ptr_objectData);
      case objectPropertiesColumns::AREA:
        return m_area(role, ptr_objectData);
      case objectPropertiesColumns::LEGEND:
        return m_legend(role, ptr_objectData);
      case objectPropertiesColumns::COLUMN:
        return m_column(role, ptr_objectData);
    }
    return QVariant();
}

bool objectPropertiesTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    int col = index.column();
    auto ptr_objectData = ptr_data.lock()->m_vectorOfObjects[row]->m_getData().lock();
    auto columnType = m_getEnumFromIndex(col);
    switch (columnType) {
      case objectPropertiesColumns::NAME:
        return m_setName(role, value, ptr_objectData);
      case objectPropertiesColumns::YAXIS:
        return m_setYAxis(role, value, ptr_objectData);
      case objectPropertiesColumns::CURVE:
        return m_setCurve(role, value, ptr_objectData);
      case objectPropertiesColumns::POINTS:
        return m_setPoints(role, value, ptr_objectData);
      case objectPropertiesColumns::AREA:
        return m_setArea(role, value, ptr_objectData);
      case objectPropertiesColumns::LEGEND:
        return m_setLegend(role, value, ptr_objectData);
      case objectPropertiesColumns::COLUMN:
        return m_setColumn(role, value, ptr_objectData);
    }
    return true;
}

QVariant objectPropertiesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (orientation) {
      case Qt::Horizontal:
        switch (role) {
          case Qt::DisplayRole:
            auto columnType = m_getEnumFromIndex(section);
              switch (columnType) {
                case objectPropertiesColumns::NAME: return "Object name";
                case objectPropertiesColumns::YAXIS: return "Y axis";
                case objectPropertiesColumns::CURVE: return "Curve";
                case objectPropertiesColumns::POINTS: return "Points";
                case objectPropertiesColumns::AREA: return "Area";
                case objectPropertiesColumns::LEGEND: return "Legend";
                case objectPropertiesColumns::COLUMN: return "Column";
              }
            break;
        }
        break;

    case Qt::Vertical:
        break;
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags objectPropertiesTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags;
    auto columnType = m_getEnumFromIndex(index.column());
    switch (columnType) {
      case objectPropertiesColumns::NAME:
        flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        break;
      case objectPropertiesColumns::YAXIS:
      case objectPropertiesColumns::CURVE:
      case objectPropertiesColumns::POINTS:
      case objectPropertiesColumns::AREA:
      case objectPropertiesColumns::LEGEND:
      case objectPropertiesColumns::COLUMN:
        flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
        break;
    }
    return flags;
}

int objectPropertiesTableModel::m_getIColumnFromEnum(objectPropertiesColumns column)
{
  switch (column){
    case objectPropertiesColumns::NAME:
      return 0;
    case objectPropertiesColumns::YAXIS:
      return 1;
    case objectPropertiesColumns::CURVE:
      return 2;
    case objectPropertiesColumns::POINTS:
      return 3;
    case objectPropertiesColumns::AREA:
      return 4;
    case objectPropertiesColumns::LEGEND:
      return 5;
    case objectPropertiesColumns::COLUMN:
      return 6;
    default:
    return 0;
  }
}

objectPropertiesColumns objectPropertiesTableModel::m_getEnumFromIndex(int index)
{
  switch (index) {
    case 0: return objectPropertiesColumns::NAME;
    case 1: return objectPropertiesColumns::YAXIS;
    case 2: return objectPropertiesColumns::CURVE;
    case 3: return objectPropertiesColumns::POINTS;
    case 4: return objectPropertiesColumns::AREA;
    case 5: return objectPropertiesColumns::LEGEND;
    case 6: return objectPropertiesColumns::COLUMN;
  default:
      qDebug() << "Unknown enum objectPropertiesColumns";
      return objectPropertiesColumns::NAME;
  }
}

bool objectPropertiesTableModel::m_setName(int /*role*/, const QVariant &/*value*/, std::shared_ptr<dataGraphObject> /*ptr_object*/)
{
    return true;
}

QVariant objectPropertiesTableModel::m_name(int role, std::shared_ptr<dataGraphObject> ptr_object) const
{
    auto text = ptr_object->m_getName();
    switch (role) {
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        return QString::fromStdString(text);
    case Qt::FontRole:
        {QFont font;
        font.setBold(true);
        return font;}
      default:
        return QVariant();
    }
}

bool objectPropertiesTableModel::m_setYAxis(int role, const QVariant &value, std::shared_ptr<dataGraphObject> ptr_object)
{
    enum yAxisPosition yAxis; // = ptr_object->m_getPrefferedYAxis();
    switch (role){
      case Qt::UserRole:
         yAxis = static_cast<enum yAxisPosition>(value.value<int>());
         break;
      default:
         return false;
    }
    ptr_object->m_setYAxis(yAxis);
    return true;
}

QVariant objectPropertiesTableModel::m_yAxis(int role, std::shared_ptr<dataGraphObject> ptr_object) const
{
    auto yAxis = ptr_object->m_getPrefferedYAxis();
    switch (role){
      case Qt::UserRole + 10:
        return true;
      case Qt::UserRole:
          return static_cast<int>(yAxis);
      default:
        return QVariant();
    }
}

bool objectPropertiesTableModel::m_setCurve(int role, const QVariant &value, std::shared_ptr<dataGraphObject> ptr_object)
{
    auto [color, width, style, show] = ptr_object->m_getStyleOfCurve();
    switch (role){
      case Qt::UserRole:
         color = value.value<QColor>();
         break;
      case Qt::UserRole + 1:
         width = value.value<int>();
         break;
      case Qt::UserRole + 2:
         style = value.value<int>();
         break;
      case Qt::UserRole + 3:
         show = value.value<bool>();
         break;
      default:
         return false;
    }
    ptr_object->m_setStyleOfCurve(color, width, style, show);
    return true;
}

QVariant objectPropertiesTableModel::m_curve(int role, std::shared_ptr<dataGraphObject> ptr_object) const
{
    auto [color, width, style, show] = ptr_object->m_getStyleOfCurve();
    switch (role){
      case Qt::UserRole + 10:
        return ptr_object->m_getHasCurve();
      case Qt::UserRole:
          return color;
      case Qt::UserRole + 1:
          return width;
      case Qt::UserRole + 2:
          return style;
      case Qt::UserRole + 3:
          return show;
      default:
        return QVariant();
    }
}

bool objectPropertiesTableModel::m_setPoints(int role, const QVariant &value, std::shared_ptr<dataGraphObject> ptr_object)
{
    auto [color, width, shape, style, show] = ptr_object->m_getStyleOfPoints();
    switch (role){
    case Qt::UserRole:
         color = value.value<QColor>();
         break;
      case Qt::UserRole + 1:
         width = value.value<int>();
         break;
      case Qt::UserRole + 2:
         shape = value.value<int>();
         break;
      case Qt::UserRole + 3:
       style = value.value<int>();
       break;
      case Qt::UserRole + 4:
       show = value.value<bool>();
       break;
      default:
         return false;
      break;
    }
    ptr_object->m_setStyleOfPoints(color, width, shape, style, show);
    return true;
}

QVariant objectPropertiesTableModel::m_points(int role, std::shared_ptr<dataGraphObject> ptr_object) const
{
    auto [color, width, shape, style, show] = ptr_object->m_getStyleOfPoints();
    switch (role){
    case Qt::UserRole + 10:
        return ptr_object->m_getHasPoints();
      case Qt::UserRole:
          return color;
      case Qt::UserRole + 1:
          return width;
      case Qt::UserRole + 2:
          return shape;
      case Qt::UserRole + 3:
          return style;
      case Qt::UserRole + 4:
          return show;
      default:
        return QVariant();
    }
}

bool objectPropertiesTableModel::m_setLegend(int role, const QVariant &value, std::shared_ptr<dataGraphObject> ptr_object)
{
    auto [overwrite, text, show] = ptr_object->m_getStyleOfLegend();
    QString textQ = QString::fromStdString(text);
    switch (role){
      case Qt::UserRole:
         overwrite = value.value<bool>();
         break;
      case Qt::UserRole + 1:
         textQ = value.value<QString>();
         break;
      case Qt::UserRole + 2:
         show = value.value<bool>();
         break;
      default:
         return false;
    }
    ptr_object->m_setStyleOfLegend(overwrite, textQ.toStdString(), show);
    return true;
}

QVariant objectPropertiesTableModel::m_legend(int role, std::shared_ptr<dataGraphObject> ptr_object) const
{
    auto [overwrite, text, show] = ptr_object->m_getStyleOfLegend();
    switch (role){
    case Qt::UserRole + 10:
        return ptr_object->m_getHasLegend();
      case Qt::UserRole:
          return overwrite;
      case Qt::UserRole + 1:
          return QString::fromStdString(text);
      case Qt::UserRole + 2:
          return show;
      default:
        return QVariant();
    }
}

bool objectPropertiesTableModel::m_setColumn(int role, const QVariant &value, std::shared_ptr<dataGraphObject> ptr_object)
{
    auto [width, show] = ptr_object->m_getStyleOfColumns();
    switch (role){
      case Qt::UserRole:
         width = value.value<int>();
         break;
      case Qt::UserRole + 1:
         show = value.value<bool>();
         break;
      default:
         return false;
    }
    ptr_object->m_setStyleOfColumn(width, show);
    return true;
}

QVariant objectPropertiesTableModel::m_column(int role, std::shared_ptr<dataGraphObject> ptr_object) const
{
    auto [width, show] = ptr_object->m_getStyleOfColumns();
    switch (role){
    case Qt::UserRole + 10:
        return ptr_object->m_getHasColumn();
      case Qt::UserRole:
          return width;
      case Qt::UserRole + 1:
          return show;
      default:
        return QVariant();
    }
}

bool objectPropertiesTableModel::m_setArea(int role, const QVariant &value, std::shared_ptr<dataGraphObject> ptr_object)
{
    auto [color, style, show] = ptr_object->m_getStyleOfArea();
    switch (role){
      case Qt::UserRole:
         color = value.value<QColor>();
         break;
      case Qt::UserRole + 1:
         style = value.value<int>();
         break;
      case Qt::UserRole + 2:
         show = value.value<bool>();
         break;
      default:
         return false;
      break;
    }
    ptr_object->m_setStyleOfArea(color, style, show);
    return true;
}

QVariant objectPropertiesTableModel::m_area(int role, std::shared_ptr<dataGraphObject> ptr_object) const
{
    auto [color, style, show] = ptr_object->m_getStyleOfArea();
    switch (role){
      case Qt::UserRole + 10:
        return ptr_object->m_getHasArea();
      case Qt::UserRole:
          return color;
      case Qt::UserRole + 1:
          return style;
      case Qt::UserRole + 2:
          return show;
      default:
        return QVariant();
    }
}

tabGraphSettingsObjects::tabGraphSettingsObjects(std::weak_ptr<dataGraph> ptr_data):
    tabGraph("Curves"),
    ptr_graphData(ptr_data)
{
    m_createTableAndModel();
    m_layBackground->addWidget(m_table);
}

void tabGraphSettingsObjects::m_createTableAndModel()
{
    m_model = new objectPropertiesTableModel(ptr_graphData.lock());
    m_table = new QTableView();
    m_table->setModel(m_model);
    connect(m_model, &objectPropertiesTableModel::dataChanged,
            this, &tabGraphSettingsObjects::m_slotDataChanged);
    m_setDelegatesToTableColumns();
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setAlternatingRowColors(true);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    m_table->setColumnWidth(0, 200);
    for (int row = 0; row < m_model->rowCount(); ++row)
        for (int col = 0; col < m_model->columnCount(); ++col)
            if (col != 0)
                m_table->openPersistentEditor(m_model->index(row, col));
}

void tabGraphSettingsObjects::m_setDelegatesToTableColumns()
{
    delegateYAxis* delYAxis = new delegateYAxis();
    m_table->setItemDelegateForColumn(
                objectPropertiesTableModel::m_getIColumnFromEnum(objectPropertiesColumns::YAXIS),
                delYAxis);
    delegateCurve* delCurve = new delegateCurve();
    m_table->setItemDelegateForColumn(
                objectPropertiesTableModel::m_getIColumnFromEnum(objectPropertiesColumns::CURVE),
                delCurve);
    delegatePoints* delPoints = new delegatePoints();
    m_table->setItemDelegateForColumn(
                objectPropertiesTableModel::m_getIColumnFromEnum(objectPropertiesColumns::POINTS),
                delPoints);
    delegateArea* delArea = new delegateArea();
    m_table->setItemDelegateForColumn(
                objectPropertiesTableModel::m_getIColumnFromEnum(objectPropertiesColumns::AREA),
                delArea);
    delegateLegend* delLegend = new delegateLegend();
    m_table->setItemDelegateForColumn(
                objectPropertiesTableModel::m_getIColumnFromEnum(objectPropertiesColumns::LEGEND),
                delLegend);
    delegateColumn* delColumn = new delegateColumn();
    m_table->setItemDelegateForColumn(
                objectPropertiesTableModel::m_getIColumnFromEnum(objectPropertiesColumns::COLUMN),
                delColumn);
}

void tabGraphSettingsObjects::m_slotDataChanged()
{

}

delegateYAxis::delegateYAxis()
{
}

QWidget *delegateYAxis::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    widGraphObjectSettingAxis *wid = new widGraphObjectSettingAxis();
    wid->setParent(parent);
    connect(wid, &widGraphObjectSettingAxis::m_signalChanged,
            this, &delegateYAxis::commitAndCloseEditor);
    return wid;
}

void delegateYAxis::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto *wid = qobject_cast<widGraphObjectSettingAxis *> (editor);
    auto yAxis = index.data(Qt::UserRole).value<int>();
    yAxisPosition value = yAxisPosition::LEFT;
    switch (yAxis) {
        case 0:
            value = yAxisPosition::LEFT;
            break;
        case 1:
            value = yAxisPosition::RIGHT;
            break;
    }
    wid->m_setValues(value);
}

void delegateYAxis::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto *wid = qobject_cast<widGraphObjectSettingAxis *> (editor);
    auto [yAxis] = wid->m_getValues();
    int value = 0;
    switch (yAxis) {
        case yAxisPosition::LEFT:
            value = 0;
            break;
        case yAxisPosition::RIGHT:
            value = 1;
            break;
    }
    model->setData(index, value, Qt::UserRole);
    emit model->dataChanged(index, index, {Qt::UserRole});
}

void delegateYAxis::commitAndCloseEditor()
{
    auto *wid = qobject_cast<widGraphObjectSettingAxis *> (sender());
    emit commitData(wid);
}

delegateCurve::delegateCurve()
{
}

QWidget *delegateCurve::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    widGraphObjectSettingCurve *wid = new widGraphObjectSettingCurve();
    wid->setParent(parent);
    connect(wid, &widGraphObjectSettingCurve::m_signalChanged,
            this, &delegateCurve::commitAndCloseEditor);
    return wid;
}

void delegateCurve::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto *wid = qobject_cast<widGraphObjectSettingCurve *> (editor);
    bool isShown = index.data(Qt::UserRole + 10).value<bool>();
    QColor color = index.data(Qt::UserRole).value<QColor>();
    int width = index.data(Qt::UserRole + 1).value<int>();
    int style = index.data(Qt::UserRole + 2).value<int>();
    bool show = index.data(Qt::UserRole + 3).value<bool>();
    wid->m_setValues(color, width, style, show);
    wid->m_setVisible(isShown);
}

void delegateCurve::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto *wid = qobject_cast<widGraphObjectSettingCurve *> (editor);
    auto [color, width, style, show] = wid->m_getValues();
    model->setData(index, color, Qt::UserRole);
    model->setData(index, width, Qt::UserRole + 1);
    model->setData(index, style, Qt::UserRole + 2);
    model->setData(index, show, Qt::UserRole + 3);
    emit model->dataChanged(index, index, {Qt::UserRole, Qt::UserRole+1, Qt::UserRole+2, Qt::UserRole+3});
}

void delegateCurve::commitAndCloseEditor()
{
    auto *wid = qobject_cast<widGraphObjectSettingCurve *> (sender());
    emit commitData(wid);
  //  emit closeEditor(wid);
}

delegatePoints::delegatePoints()
{
}

QWidget *delegatePoints::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    widGraphObjectSettingPoints *wid = new widGraphObjectSettingPoints();
    wid->setParent(parent);
    connect(wid, &widGraphObjectSettingPoints::m_signalChanged,
            this, &delegatePoints::commitAndCloseEditor);
    return wid;
}

void delegatePoints::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto *wid = qobject_cast<widGraphObjectSettingPoints *> (editor);
    bool isShown = index.data(Qt::UserRole + 10).value<bool>();
    QColor color = index.data(Qt::UserRole).value<QColor>();
    int width = index.data(Qt::UserRole + 1).value<int>();
    int shape = index.data(Qt::UserRole + 2).value<int>();
    int style = index.data(Qt::UserRole + 3).value<int>();
    bool show = index.data(Qt::UserRole + 4).value<bool>();
    wid->m_setValues(color, width, shape, style, show);
    wid->m_setVisible(isShown);
}

void delegatePoints::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto *wid = qobject_cast<widGraphObjectSettingPoints *> (editor);
    auto [color, width, shape, style, show] = wid->m_getValues();
    model->setData(index, color, Qt::UserRole);
    model->setData(index, width, Qt::UserRole + 1);
    model->setData(index, shape, Qt::UserRole + 2);
    model->setData(index, style, Qt::UserRole + 3);
    model->setData(index, show, Qt::UserRole + 4);
    emit model->dataChanged(index, index, {Qt::UserRole, Qt::UserRole+1, Qt::UserRole+2, Qt::UserRole+3, Qt::UserRole+4});
}

void delegatePoints::commitAndCloseEditor()
{
    auto *wid = qobject_cast<widGraphObjectSettingPoints *> (sender());
    emit commitData(wid);
}

delegateArea::delegateArea()
{
}

QWidget *delegateArea::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    widGraphObjectSettingArea *wid = new widGraphObjectSettingArea();
    wid->setParent(parent);
    connect(wid, &widGraphObjectSettingArea::m_signalChanged,
            this, &delegateArea::commitAndCloseEditor);
    return wid;
}

void delegateArea::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto *wid = qobject_cast<widGraphObjectSettingArea *> (editor);
    bool isShown = index.data(Qt::UserRole + 10).value<bool>();
    QColor color = index.data(Qt::UserRole).value<QColor>();
    int style = index.data(Qt::UserRole + 1).value<int>();
    bool show = index.data(Qt::UserRole + 2).value<bool>();
    wid->m_setValues(color, style, show);
    wid->m_setVisible(isShown);
}

void delegateArea::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto *wid = qobject_cast<widGraphObjectSettingArea *> (editor);
    auto [color, style, show] = wid->m_getValues();
    model->setData(index, color, Qt::UserRole);
    model->setData(index, style, Qt::UserRole + 1);
    model->setData(index, show, Qt::UserRole + 2);
    emit model->dataChanged(index, index, {Qt::UserRole, Qt::UserRole+1, Qt::UserRole+2});
}

void delegateArea::commitAndCloseEditor()
{
    auto *wid = qobject_cast<widGraphObjectSettingArea *> (sender());
    emit commitData(wid);
}

delegateLegend::delegateLegend()
{
}

QWidget *delegateLegend::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    widGraphObjectSettingLegend *wid = new widGraphObjectSettingLegend();
    wid->setParent(parent);
    connect(wid, &widGraphObjectSettingLegend::m_signalChanged,
            this, &delegateLegend::commitAndCloseEditor);
    return wid;
}

void delegateLegend::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto *wid = qobject_cast<widGraphObjectSettingLegend *> (editor);
    bool isShown = index.data(Qt::UserRole + 10).value<bool>();
    bool overwrite = index.data(Qt::UserRole).value<bool>();
    std::string text = index.data(Qt::UserRole + 1).value<QString>().toStdString();
    bool show = index.data(Qt::UserRole + 2).value<bool>();
    wid->m_setValues(overwrite, text, show);
    wid->m_setVisible(isShown);
}

void delegateLegend::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto *wid = qobject_cast<widGraphObjectSettingLegend *> (editor);
    auto [overwrite, text, show] = wid->m_getValues();
    model->setData(index, overwrite, Qt::UserRole);
    model->setData(index, QString::fromStdString(text), Qt::UserRole + 1);
    model->setData(index, show, Qt::UserRole + 2);
    emit model->dataChanged(index, index, {Qt::UserRole, Qt::UserRole+1, Qt::UserRole+2});
}

void delegateLegend::commitAndCloseEditor()
{
    auto *wid = qobject_cast<widGraphObjectSettingLegend *> (sender());
    emit commitData(wid);
}

delegateColumn::delegateColumn()
{

}

QWidget *delegateColumn::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    widGraphObjectSettingColumn *wid = new widGraphObjectSettingColumn();
    wid->setParent(parent);
    connect(wid, &widGraphObjectSettingColumn::m_signalChanged,
            this, &delegateColumn::commitAndCloseEditor);
    return wid;
}

void delegateColumn::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto *wid = qobject_cast<widGraphObjectSettingColumn*> (editor);
    bool isShown = index.data(Qt::UserRole + 10).value<bool>();
    int width = index.data(Qt::UserRole).value<int>();
    bool show = index.data(Qt::UserRole + 1).value<bool>();
    wid->m_setValues(width, show);
    wid->m_setVisible(isShown);
}

void delegateColumn::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto *wid = qobject_cast<widGraphObjectSettingColumn*> (editor);
    auto [width, show] = wid->m_getValues();
    model->setData(index, width, Qt::UserRole);
    model->setData(index, show, Qt::UserRole + 1);
    emit model->dataChanged(index, index, {Qt::UserRole, Qt::UserRole+1});
}

void delegateColumn::commitAndCloseEditor()
{
    auto *wid = qobject_cast<widGraphObjectSettingColumn*> (sender());
    emit commitData(wid);
}
