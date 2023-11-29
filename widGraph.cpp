#include "widGraph.h"
#include "dialogGraph.h"
#include <QApplication>
#include <QClipboard>

widGraph::widGraph()
{
    // Create data
        m_data = std::make_shared<dataGraph>();
    // Create graph elements
        m_widArea = new widGraphDrawArea(this);
        m_widTitle = new widGraphTitle(this);
        m_widX = new widGraphXAxis(this);
        m_widY1 = new widGraphY1Axis(this);
        m_widY2 = new widGraphY2Axis(this);
        m_widLegend = new widGraphLegend(this);
    // Add elements to the layout
        graphLayout *layBackground = new graphLayout();
        layBackground->addWidget(m_widTitle, 0,0, 1,3);
        layBackground->addWidget(m_widX, 2,0, 1,3);
        layBackground->addWidget(m_widY1, 0,0, 3,1);
        layBackground->addWidget(m_widY2, 0,2, 3,1);
        layBackground->addWidget(m_widArea, 1,1, 1,1);
        layBackground->addWidget(m_widLegend, 3,0, 1,3);
        setLayout(layBackground);
     // Stylesheet
        setStyleSheet(".widGraph {background: white;}"
                      ".QWidget {background: transparent;}");
        setFocusPolicy(Qt::StrongFocus);
  //      setWindowFlags(Qt::SubWindow);
        setAttribute(Qt::WA_DeleteOnClose);
}

widGraph::~widGraph()
{
    emit m_signalClose();
}

void widGraph::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        // Cancel zoom or drag operation
            bool a = m_widArea->m_cancelOperation();
            bool b = m_widX->m_cancelOperation();
            bool c = m_widY1->m_cancelOperation();
            bool d = m_widY2->m_cancelOperation();
        // If nothing was done, cancel moving or dragging mode
            if (!(a || b || c || d)) {
                m_data->m_control->m_zoom = false;
                m_data->m_control->m_move = false;
            }
        m_loadValues();
    }
}

void widGraph::hideEvent(QHideEvent */*event*/)
{
    emit m_signalHide();
}

void widGraph::m_addObject(std::shared_ptr<graphObjects> ptr_object)
{
    m_data->m_vectorOfObjects.push_back(ptr_object);
}

void widGraph::m_loadValues()
{
    setUpdatesEnabled(false);
    // Set dimensions
        m_widLegend->m_setDimensions();
        m_widX->m_setDimensions();
        m_widY1->m_setDimensions();
        m_widY2->m_setDimensions();
        m_widTitle->m_setDimensions();

    // Set axis
        m_widX->m_setAxis();
        m_widY1->m_setAxis();
        m_widY2->m_setAxis();

    // Set mouse cursor
        if (m_data->m_control->m_zoom)
            setCursor(Qt::CrossCursor);
        else if (m_data->m_control->m_move)
            setCursor(Qt::SizeAllCursor);
        else
            setCursor(Qt::ArrowCursor);
    setUpdatesEnabled(true);
}

void widGraph::m_setCurveStyle(int curveIndex, QColor color, int curveWidth, int curveStyleIndex, bool showCurve)
{
    auto ptr_objectData = m_getObjectFromIndex(curveIndex);
    ptr_objectData->m_setStyleOfCurve(color, curveWidth, curveStyleIndex, showCurve);
}

void widGraph::m_setPointsStyle(int curveIndex, QColor color, int penPointsWidth, int pointsShapeSize, int pointsStyleIndex, bool showPoints)
{
    auto ptr_objectData = m_getObjectFromIndex(curveIndex);
    ptr_objectData->m_setStyleOfPoints(color, penPointsWidth, pointsShapeSize, pointsStyleIndex, showPoints);
}

void widGraph::m_setAreaStyle(int curveIndex, QColor color, int areaStyleIndex, bool showArea)
{
    auto ptr_objectData = m_getObjectFromIndex(curveIndex);
    ptr_objectData->m_setStyleOfArea(color, areaStyleIndex, showArea);
}

void widGraph::m_setColumnsStyle(int curveIndex, int columnWidth, bool showColumn)
{
    auto ptr_objectData = m_getObjectFromIndex(curveIndex);
    ptr_objectData->m_setStyleOfColumn(columnWidth, showColumn);
}

void widGraph::m_setCurveAxis(int curveIndex, int axis)
{
    auto ptr_objectData = m_getObjectFromIndex(curveIndex);
    ptr_objectData->m_setPrefferedAxis(axis);
}

void widGraph::m_setCurveName(int curveIndex, const std::string &name)
{
    auto ptr_objectData = m_getObjectFromIndex(curveIndex);
    ptr_objectData->m_setName(name);
}

void widGraph::m_openDialog()
{
    if (m_dialog == nullptr) {
        m_dialog = new dialogGraph(this, m_data);
  //      connect(this, &widGraph::m_signalHide, this, [](){qDebug() << "signalHide";});
        connect(this, &widGraph::m_signalHide, m_dialog, &dialogGraph::m_slotClose);
        connect(m_dialog, &QDialog::finished, this, &widGraph::m_slotDialogClosed);
    }
    m_dialog->show();
}

void widGraph::m_takeScreenshot()
{
    m_widTitle->m_hideButtons();
    int ratio = 5;
    QSize imageSize = size()*ratio;
    QImage image = QImage(imageSize, QImage::Format_RGB32);
    image.setDevicePixelRatio(ratio);
    render(&image);
    QApplication::clipboard()->setImage(image);
    m_widTitle->m_showButtons();
}

void widGraph::m_zoomOut()
{
    auto [minX, maxX, stepX] = m_data->m_X->m_getMinMaxStep();
    auto [minY1, maxY1, stepY1] = m_data->m_Y1->m_getMinMaxStep();
    auto [minY2, maxY2, stepY2] = m_data->m_Y2->m_getMinMaxStep();

    double zoomFactor = 0.25;
    double d_x = zoomFactor*(maxX - minX);
    double d_y1 = zoomFactor*(maxY1 - minY1);
    double d_y2 = zoomFactor*(maxY2 - minY2);

    m_widArea->m_setAxisMinMax(minX - d_x, maxX + d_x, minY1 - d_y1, maxY1 + d_y1, minY2 - d_y2, maxY2 + d_y2);
    m_loadValues();
}

QString widGraph::removeTrailingZeros(double number)
{
    double a = number;
    QString textNumber;
    textNumber = QString::number(a, 'f', 2);

    return textNumber;
}

void widGraph::m_slotDialogClosed(int /*status*/)
{
    m_dialog = nullptr;
}

std::shared_ptr<dataGraphObject> widGraph::m_getObjectFromIndex(int curveIndex)
{
    if (curveIndex == -1)
        curveIndex = m_data->m_vectorOfObjects.size() - 1;

    auto ptr_objectData = m_data->m_vectorOfObjects[curveIndex]->m_getData().lock();
    return ptr_objectData;
}

void widGraphDrawArea::m_drawHorGrid(painterAntiAl &painter)
{
    auto ptr_y = ptr_graph->m_getY1Axis();
    auto ptr_data = ptr_graph->m_getData().lock()->m_Y1;
    auto [min, max, step] = ptr_data->m_getMinMaxStep();
    auto [niceMin, niceMax, c] = widGraphAxis::m_calculateNiceMaxMin(min, max);
    std::vector<double> vTicks = widGraphAxis::m_getTicksPosition(niceMin, max, step);
    for (const auto &var: vTicks) {
        double posY = ptr_y->m_getDrawAreaPositionFromValue(var);
        painter.drawLine(0,posY,width(),posY);
    }
}

void widGraphDrawArea::m_drawVertGrid(painterAntiAl &painter)
{
    auto ptr_x = ptr_graph->m_getXAxis();
    auto ptr_dataX = ptr_graph->m_getData().lock()->m_X;
    auto [min, max, step] = ptr_dataX->m_getMinMaxStep();
    auto [niceMin, niceMax, c] = widGraphAxis::m_calculateNiceMaxMin(min, max);
    std::vector<double> vTicks = widGraphAxis::m_getTicksPosition(niceMin, max, step);
    for (const auto &var: vTicks) {
        double posX = ptr_x->m_getDrawAreaPositionFromValue(var);
        painter.drawLine(posX,0,posX,width());
    }
}

void widGraphDrawArea::m_setAxisMinMax(double startX, double endX, double startY1, double endY1, double startY2, double endY2)
{
    ptr_graph->m_getXAxis()->m_setAxisMinMax(startX, endX);
    ptr_graph->m_getY1Axis()->m_setAxisMinMax(startY1, endY1);
    ptr_graph->m_getY2Axis()->m_setAxisMinMax(startY2, endY2);
}

bool widGraphDrawArea::m_cancelOperation()
{
    bool res = m_isMouseZooming  || m_isMouseMoving;
    m_isMouseZooming = false;
    m_isMouseMoving = false;
    return res;
}

widGraphDrawArea::widGraphDrawArea(widGraph *graph):
    widGraphElement(graph)
{
}

void widGraphDrawArea::mousePressEvent(QMouseEvent *event)
{
    m_startingPoint = event->pos();
    m_isMouseZooming = false;
    m_isMouseMoving = false;
    auto ptr_control = ptr_graph->m_getData().lock()->m_control;
    if (ptr_control->m_zoom)
        m_isMouseZooming = true;
    else if (ptr_control->m_move)
        m_isMouseMoving = true;
}

void widGraphDrawArea::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isMouseMoving) {
        m_moveByMouse();
        m_startingPoint = event->pos();
    }
    update();
}

void widGraphDrawArea::m_moveByMouse()
{
  //  m_isMouseMoving = false;
    QPoint currentPoint = mapFromGlobal(QCursor::pos());
    double startX = ptr_graph->m_getXAxis()->m_getValueFromDrawAreaPosition(m_startingPoint.x());
    double startY1 = ptr_graph->m_getY1Axis()->m_getValueFromDrawAreaPosition(m_startingPoint.y());
    double startY2 = ptr_graph->m_getY2Axis()->m_getValueFromDrawAreaPosition(m_startingPoint.y());
    double endX = ptr_graph->m_getXAxis()->m_getValueFromDrawAreaPosition(currentPoint.x());
    double endY1 = ptr_graph->m_getY1Axis()->m_getValueFromDrawAreaPosition(currentPoint.y());
    double endY2 = ptr_graph->m_getY2Axis()->m_getValueFromDrawAreaPosition(currentPoint.y());

    double currStartX = ptr_graph->m_getData().lock()->m_X->m_min;
    double currEndX = ptr_graph->m_getData().lock()->m_X->m_max;
    double currStartY1 = ptr_graph->m_getData().lock()->m_Y1->m_min;
    double currEndY1 = ptr_graph->m_getData().lock()->m_Y1->m_max;
    double currStartY2 = ptr_graph->m_getData().lock()->m_Y2->m_min;
    double currEndY2 = ptr_graph->m_getData().lock()->m_Y2->m_max;

    m_setAxisMinMax(currStartX + (startX - endX), currEndX + (startX - endX),
                    currStartY1 + (startY1 - endY1), currEndY1 + (startY1 - endY1),
                    currStartY2 + (startY2 - endY2), currEndY2 + (startY2 - endY2));
}

void widGraphDrawArea::m_zoomByMouse()
{
    QPoint currentPoint = mapFromGlobal(QCursor::pos());

    if (widGraphAxis::m_supDistanceForActionIsSufficient(m_startingPoint, currentPoint)) {
        double startX = ptr_graph->m_getXAxis()->m_getValueFromDrawAreaPosition(m_startingPoint.x());
        double startY1 = ptr_graph->m_getY1Axis()->m_getValueFromDrawAreaPosition(m_startingPoint.y());
        double startY2 = ptr_graph->m_getY2Axis()->m_getValueFromDrawAreaPosition(m_startingPoint.y());

        double endX = ptr_graph->m_getXAxis()->m_getValueFromDrawAreaPosition(currentPoint.x());
        double endY1 = ptr_graph->m_getY1Axis()->m_getValueFromDrawAreaPosition(currentPoint.y());
        double endY2 = ptr_graph->m_getY2Axis()->m_getValueFromDrawAreaPosition(currentPoint.y());

        m_setAxisMinMax(startX, endX, startY1, endY1, startY2, endY2);
    }
}

void widGraphDrawArea::mouseReleaseEvent(QMouseEvent */*event*/)
{
    if (m_isMouseZooming)
        m_zoomByMouse();

    m_isMouseZooming = false;
    m_isMouseMoving = false;
    ptr_graph->m_loadValues();
}

void widGraphDrawArea::m_drawMove(painterAntiAl &painter)
{
    painter.save();
    QPoint currentPoint = mapFromGlobal(QCursor::pos());
    if (m_isMouseMoving) {
        QPen penRect(Qt::black, 1, Qt::DashLine);
        painter.setPen(penRect);
        QLine line(m_startingPoint, currentPoint);
        painter.setBrush(Qt::red);
        painter.drawEllipse(m_startingPoint, 2, 2);
        painter.drawLine(line);
    }
    painter.restore();
}

void widGraphDrawArea::m_drawSelectionRectangle(painterAntiAl &painter)
{
    painter.save();
    QPoint currentPoint = mapFromGlobal(QCursor::pos());
    if (m_isMouseZooming
        && widGraphAxis::m_supDistanceForActionIsSufficient(m_startingPoint, currentPoint)) {
        QPen penRect(Qt::black, 1, Qt::DashLine);
        painter.setPen(penRect);
        QRect rect(m_startingPoint, currentPoint);
        painter.drawRect(rect);
    }
    painter.restore();
}

void widGraphDrawArea::m_drawBorder(painterAntiAl &painter)
{
    painter.save();
    QPen penBorder(Qt::black, 2);
    painter.setPen(penBorder);
    QRect rectBorder(0, 0, width()-1, height()-1);
    painter.drawRect(rectBorder);
    painter.restore();
}

void widGraphDrawArea::m_drawGrid(painterAntiAl &painter)
{
    painter.save();
    auto ptr_data = ptr_graph->m_getData().lock()->m_drawArea;
    if (ptr_data->m_showGrid) {
        QPen penGrid(Qt::gray, 1, Qt::DashLine);
        painter.setPen(penGrid);
        m_drawHorGrid(painter);
        m_drawVertGrid(painter);
    }
    painter.restore();
}

void widGraphDrawArea::m_drawGraphObjects(painterAntiAl &painter)
{
    painter.save();
    auto vectorObjects = ptr_graph->m_getData().lock()->m_vectorOfObjects;
    for(auto &var: vectorObjects)
        var->m_drawItself(&painter, ptr_graph);
    painter.restore();
}

void widGraphDrawArea::paintEvent(QPaintEvent */*event*/)
{
    // Painter
        painterAntiAl painter(this);
    // Curves
        m_drawGraphObjects(painter);
    // Grid
        m_drawGrid(painter);
    // Border
        m_drawBorder(painter);
    // Selection rectangle
        m_drawSelectionRectangle(painter);
    // Move trajectory
        m_drawMove(painter);
}

widGraphTitle::widGraphTitle(widGraph *graph):
    widGraphElement(graph)
{
    m_text = new widGraphTitleText(ptr_graph);
    m_butAuto = new widGraphButtonAutoAxes(ptr_graph);
    m_butZoomIn = new widGraphButtonZoomIn(ptr_graph);
    m_butZoomOut = new widGraphButtonZoomOut(ptr_graph);
    m_butMove = new widGraphButtonMove(ptr_graph);
    m_butScreenshot = new widGraphButtonScreenshot(ptr_graph);
    m_layBackground = new QHBoxLayout(this);
    m_layBackground->setSpacing(2);
    m_layBackground->addWidget(m_text);
    m_layBackground->addWidget(m_butZoomIn);
    m_layBackground->addWidget(m_butZoomOut);
    m_layBackground->addWidget(m_butMove);
    m_layBackground->addWidget(m_butAuto);
    m_layBackground->addWidget(m_butScreenshot);
}

void widGraphTitle::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    // Painter
        painterAntiAl painter(this);
    // Draw line
        m_drawLine(painter);
}

void widGraphTitle::enterEvent(QEnterEvent *event)
{
    QWidget::enterEvent(event);
    m_showButtons();
    update();
}

void widGraphTitle::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    m_hideButtons();
}

void widGraphTitle::m_setDimensions()
{
    auto ptr_dataTitle = ptr_graph->m_getData().lock()->m_title;
    ptr_dataTitle->m_height = m_spaceAbove +
            m_rowSpacing*ptr_dataTitle->m_fontText + m_spaceBelow;
    setFixedHeight(ptr_dataTitle->m_height);

    auto ptr_data = ptr_graph->m_getData().lock();

    m_layBackground->setContentsMargins(ptr_data->m_Y1->m_width,0,ptr_data->m_Y2->m_width,0);
    m_text->m_setDimensions();
    m_setButtonsDimensions();
    m_loadButtonsValues();
}

void widGraphTitle::m_drawLine(painterAntiAl &painter)
{
    painter.save();
    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenX = ptr_data->m_Y1->m_width;
    double endScreenX = width() - ptr_data->m_Y2->m_width;
    double bottom = height();
    painter.drawLine(startScreenX,bottom,endScreenX,bottom);
    painter.restore();
}

void widGraphTitle::m_showButtons()
{
    m_butZoomIn->m_show();
    m_butZoomOut->m_show();
    m_butMove->m_show();
    m_butAuto->m_show();
    m_butScreenshot->m_show();
}

void widGraphTitle::m_hideButtons()
{
    m_butZoomIn->m_hide();
    m_butZoomOut->m_hide();
    m_butMove->m_hide();
    m_butAuto->m_hide();
    m_butScreenshot->m_hide();
    update();
}

void widGraphTitle::m_setButtonsDimensions()
{
    m_butZoomIn->m_setDimensions();
    m_butZoomOut->m_setDimensions();
    m_butMove->m_setDimensions();
    m_butAuto->m_setDimensions();
    m_butScreenshot->m_setDimensions();
}

void widGraphTitle::m_loadButtonsValues()
{
    m_butZoomIn->m_loadValues();
    m_butZoomOut->m_loadValues();
    m_butMove->m_loadValues();
    m_butAuto->m_loadValues();
    m_butScreenshot->m_loadValues();
}

widGraphXAxis::widGraphXAxis(widGraph *graph):
    widGraphXAxes(graph)
{

}

double widGraphXAxis::m_getDrawAreaPositionFromValue(double value)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenX = ptr_data->m_Y1->m_width;
    double endScreenX = width() - ptr_data->m_Y2->m_width;

    auto [min, max, step] = ptr_data->m_X->m_getMinMaxStep();
    return (endScreenX-startScreenX)*(value-min)/(max-min);
}

double widGraphXAxis::m_getValueFromDrawAreaPosition(double position)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenX = ptr_data->m_Y1->m_width;
    double y2axisWidth = ptr_data->m_Y2->m_width;
    double endScreenX = width() - y2axisWidth;

    auto [min, max, step] = ptr_data->m_X->m_getMinMaxStep();
    double ret = position*(max-min)/(endScreenX-startScreenX) + min;
    return ret;
}

std::tuple<double, double> widGraphXAxis::m_getMinAndMaxOfObjects()
{
    // Graph data
        auto ptr_graphData = ptr_graph->m_getData().lock();
    // Min and max values
        double minX = 0;
        double maxX = 0;
        for (auto &var:ptr_graphData->m_vectorOfObjects) {
            minX = std::min(minX, var->m_getMinX());
            maxX = std::max(maxX, var->m_getMaxX());
        }
    // Return
        return {minX, maxX};
}

void widGraphXAxis::m_setDimensions()
{
    auto ptr_data = ptr_graph->m_getData().lock()->m_X;
    if (ptr_data->m_manualSize)
        ptr_data->m_height = ptr_data->m_manualSizeValue;
    else
        ptr_data->m_height = m_getEndFromTop();
    setFixedHeight(ptr_data->m_height);
}

std::weak_ptr<dataAxis> widGraphXAxis::m_getData()
{
    return ptr_graph->m_getData().lock()->m_X;
}

double widGraphXAxes::m_getPositionFromValue(double value)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenX = ptr_data->m_Y1->m_width;
    return startScreenX + m_getDrawAreaPositionFromValue(value);
}

double widGraphXAxis::m_getValueFromPosition(double position)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenX = ptr_data->m_Y1->m_width;
    double ret = m_getValueFromDrawAreaPosition(position - startScreenX);
    return ret;
}

std::tuple<double, double> widGraphXAxis::m_getStartAndEndFromMouse(QPointF start, QPointF end)
{
    double startX = m_getValueFromPosition(start.x());
    double endX = m_getValueFromPosition(end.x());
    return {startX, endX};
}

void widGraphXAxis::m_drawLine(painterAntiAl &painter)
{
    painter.save();
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenX = ptr_data->m_Y1->m_width;
    double endScreenX = width() - ptr_data->m_Y2->m_width;
    painter.drawLine(startScreenX,0,endScreenX,0);
    painter.restore();
}

void widGraphXAxis::m_drawTicks(painterAntiAl &painter)
{
    painter.save();
    auto ptr_data = ptr_graph->m_getData().lock()->m_X;

    auto [min, max, step] = ptr_data->m_getMinMaxStep();
    auto [niceMin, niceMax, c] = m_calculateNiceMaxMin(min, max);
    m_supDrawTick(painter, min);
    m_supDrawTick(painter, max);
    std::vector<double> vTicks = m_getTicksPosition(niceMin, max, step);
    for (const auto &var: vTicks)
        m_supDrawTick(painter, var);
    painter.restore();
}

void widGraphXAxis::m_drawNumbers(painterAntiAl &painter)
{
    painter.save();
    auto ptr_data = ptr_graph->m_getData().lock()->m_X;
    QFont font;
    font.setPixelSize(ptr_data->m_fontNumbers);
    painter.setFont(font);

    auto [min, max, step] = ptr_data->m_getMinMaxStep();
    auto [niceMin, niceMax, c] = m_calculateNiceMaxMin(min, max);
    int nSteps = (max - min)/step;
    double numberLength = width()/nSteps/2;

    m_supDrawNumber(painter, min, numberLength);
    m_supDrawNumber(painter, max, numberLength);

    std::vector<double> vTicks = m_getTicksPosition(niceMin, max, step);
    for (const auto &var: vTicks)
        m_supDrawNumber(painter, var, numberLength);

    painter.restore();
}

void widGraphXAxis::m_drawText(painterAntiAl &painter)
{
    painter.save();
    auto ptr_dataX = ptr_graph->m_getData().lock()->m_X;
    QFont font;
    font.setPixelSize(ptr_dataX->m_fontText);
    painter.setFont(font);

    auto ptr_dataY2 = ptr_graph->m_getData().lock()->m_Y2;
    double y2width = ptr_dataY2->m_width;
    QRect rect(QPoint(0, m_getTextStart()),
               QPoint(width() - y2width, m_getTextEnds()));
    std::string text = ptr_dataX->m_text;
    if (ptr_dataX->m_unit != "")
        text = text + " [" + ptr_dataX->m_unit + "]";
    painter.drawText(rect, QString::fromStdString(text),
                     QTextOption(Qt::AlignRight | Qt::AlignVCenter));
    painter.restore();
}

void widGraphXAxis::m_drawZoomCursor(painterAntiAl &painter)
{
    painter.save();
    if (m_isMouseZooming) {
        QPoint currentPoint = mapFromGlobal(QCursor::pos());
        if (m_supDistanceForActionIsSufficient(m_startingPoint, currentPoint)) {
            int top = m_getTicksEnd();
            int bottom = top + m_zoomCursorWidth;
            // Start line
                QPoint startTop(m_startingPoint.x(), top);
                QPoint startBottom(m_startingPoint.x(), bottom);
                QLine leftLine(startTop, startBottom);
                painter.drawLine(leftLine);
            // End line
                QPoint endTop(currentPoint.x(), top);
                QPoint endBottom(currentPoint.x(), bottom);
                QLine rightLine(endTop, endBottom);
                painter.drawLine(rightLine);
            // Long line
                painter.drawLine(leftLine.center(), rightLine.center());
        }
    }
    painter.restore();
}

void widGraphXAxis::m_drawMoveCursor(painterAntiAl &painter)
{
    painter.save();
    QPoint currentPoint = mapFromGlobal(QCursor::pos());
    if (m_isMouseMoving) {
        if (m_supDistanceForActionIsSufficient(m_startingPoint, currentPoint)) {
            QPen penRect(Qt::black, 1, Qt::DashLine);
            painter.setPen(penRect);
            int y = (m_getNumbersStart() + m_getNumbersEnd())/2;
            QPoint start(m_startingPoint.x(), y);
            QPoint end(currentPoint.x(), y);
            QLine line(start, end);
            painter.setBrush(Qt::red);
            painter.drawEllipse(QPoint(m_startingPoint.x(), y), 2, 2);
            painter.drawLine(line);
        }
    }
    painter.restore();
}

double widGraphXAxis::m_getTicksStart()
{
    return 0;
}

double widGraphXAxis::m_getTicksEnd()
{
    return m_tickLength;
}

double widGraphXAxis::m_getNumbersStart()
{
    return m_getTicksEnd() + m_spaceTicksToNumbers;
}

double widGraphXAxis::m_getNumbersEnd()
{
    auto ptr_dataX = ptr_graph->m_getData().lock()->m_X;
    double rowHeight = m_rowSpacing*ptr_dataX->m_fontNumbers;
    return m_getNumbersStart() + rowHeight;
}

double widGraphXAxis::m_getTextStart()
{
    return m_getNumbersEnd() + m_spaceNumbersToText;
}

double widGraphXAxis::m_getTextEnds()
{
    auto ptr_dataX = ptr_graph->m_getData().lock()->m_X;
    double rowHeight = m_rowSpacing*ptr_dataX->m_fontText;
    return m_getTextStart() + rowHeight;
}

double widGraphXAxis::m_getEndFromTop()
{
    return m_getTextEnds() + m_spaceBorder;
}

void widGraphXAxes::m_supDrawNumber(painterAntiAl &painter, double location, double length)
{
    double posX = m_getPositionFromValue(location);
    QRect rect(QPoint(posX - length, m_getNumbersStart()),
               QPoint(posX + length, m_getNumbersEnd()));
    QString number = widGraph::removeTrailingZeros(location);
    painter.drawText(rect, number, QTextOption(Qt::AlignCenter));
}

void widGraphXAxes::m_supDrawTick(painterAntiAl &painter, double location)
{
    double posX = m_getPositionFromValue(location);
    painter.drawLine(QPoint(posX,m_getTicksStart()),
                     QPoint(posX,m_getTicksEnd()));
}

widGraphY1Axis::widGraphY1Axis(widGraph *graph):
    widGraphYAxes(graph, 0)
{

}

std::tuple<double, double> widGraphYAxes::m_getMinAndMaxOfObjects()
{
    auto ptr_graphData = ptr_graph->m_getData().lock();
    double minY = 0;
    double maxY = 0;
    for (auto &var:ptr_graphData->m_vectorOfObjects) {
        if(var->m_getPrefferedYAxis() == m_axisPosition) {
            minY = std::min(minY, var->m_getMinY());
            maxY = std::max(maxY, var->m_getMaxY());
        }
    }
    return {minY, maxY};
}

void widGraphY1Axis::m_setDimensions()
{
    auto ptr_data = m_getData().lock();
    if (ptr_data->m_manualSize)
        ptr_data->m_width = ptr_data->m_manualSizeValue;
    else
        ptr_data->m_width = m_getTicksStart();
    setFixedWidth(ptr_data->m_width);
}

std::weak_ptr<dataAxis> widGraphY1Axis::m_getData()
{
    return ptr_graph->m_getData().lock()->m_Y1;
}

std::tuple<double, double> widGraphYAxes::m_getStartAndEndFromMouse(QPointF start, QPointF end)
{
    double startY = m_getValueFromPosition(start.y());
    double endY = m_getValueFromPosition(end.y());
    return {startY, endY};
}

void widGraphYAxes::m_supDrawNumber(painterAntiAl &painter, double location, double length)
{
    double posY = m_getPositionFromValue(location);
    QPoint leftTop(std::min(m_getNumbersStart(), m_getNumbersEnd()), posY - length/2);
    QPoint rightBottom(std::max(m_getNumbersStart(), m_getNumbersEnd()), posY + length/2);
    QRect rect(leftTop, rightBottom);
    QString number = widGraph::removeTrailingZeros(location);
    painter.drawText(rect, number, QTextOption(Qt::AlignRight | Qt::AlignVCenter));
}

void widGraphYAxes::m_supDrawTick(painterAntiAl &painter, double location)
{
    double pos = m_getPositionFromValue(location);
    painter.drawLine(QPoint(m_getTicksStart(),pos),
                     QPoint(m_getTicksEnd(),pos));
}

void widGraphY1Axis::m_drawLine(painterAntiAl &painter)
{
    painter.save();
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenY = ptr_data->m_title->m_height;
    double endScreenY = height() - ptr_data->m_X->m_height;
    double right = m_getTicksStart() - 1;
    painter.drawLine(right,startScreenY,right,endScreenY);
    painter.restore();
}

void widGraphYAxes::m_drawTicks(painterAntiAl &painter)
{
    painter.save();
    auto ptr_data = m_getData().lock();
    auto [min, max, step] = ptr_data->m_getMinMaxStep();
     auto [niceMin, niceMax, c] = m_calculateNiceMaxMin(min, max);
     m_supDrawTick(painter, min);
     m_supDrawTick(painter, max);
     std::vector<double> vTicks = m_getTicksPosition(niceMin, max, step);
     for (const auto &var: vTicks)
         m_supDrawTick(painter, var);
     painter.restore();
}

void widGraphYAxes::m_drawNumbers(painterAntiAl &painter)
{
    painter.save();
    auto ptr_data = m_getData().lock();
    QFont font;
    font.setPixelSize(ptr_data->m_fontNumbers);
    painter.setFont(font);

    auto [min, max, step] = ptr_data->m_getMinMaxStep();
    auto [niceMin, niceMax, c] = m_calculateNiceMaxMin(min, max);
    m_supDrawNumber(painter, min, ptr_data->m_fontNumbers);
    m_supDrawNumber(painter, max, ptr_data->m_fontNumbers);
    std::vector<double> vTicks = m_getTicksPosition(niceMin, max, step);
    for (const auto &var: vTicks)
        m_supDrawNumber(painter, var, ptr_data->m_fontNumbers);
    painter.restore();
}

void widGraphYAxes::m_drawText(painterAntiAl &painter)
{
    painter.save();
    auto ptr_dataY = m_getData().lock();
    QFont font;
    font.setPixelSize(ptr_dataY->m_fontText);
    painter.setFont(font);

    std::string text = ptr_dataY->m_text;
    if (ptr_dataY->m_unit != "")
        text = text + " [" + ptr_dataY->m_unit + "]";
    QRect rect(QPoint(0,m_getTextEnds()),
               QPoint(-height(), m_getTextStart()));
    painter.rotate(270);
    painter.drawText(rect, Qt::AlignCenter, QString::fromStdString(text));
    painter.restore();
}

double widGraphY1Axis::m_getTicksStart()
{
    return m_getTicksEnd() + m_tickLength - 1;
}

double widGraphY1Axis::m_getTicksEnd()
{
    return m_getNumbersStart() + m_spaceTicksToNumbers;
}

double widGraphY1Axis::m_getNumbersStart()
{
    auto ptr_dataY1 = ptr_graph->m_getData().lock()->m_Y1;
    double numberWidth = 3*m_rowSpacing*ptr_dataY1->m_fontNumbers;
    return m_getNumbersEnd() + numberWidth;
}

double widGraphY1Axis::m_getNumbersEnd()
{
    return m_getTextStart() + m_spaceNumbersToText;
}

double widGraphY1Axis::m_getTextStart()
{
    auto ptr_dataY1 = ptr_graph->m_getData().lock()->m_Y1;
    double rowHeight = m_rowSpacing*ptr_dataY1->m_fontText;
    return m_getTextEnds() + rowHeight;
}

double widGraphY1Axis::m_getTextEnds()
{
    return 0 + m_spaceBorder;
}

widGraphAxis::widGraphAxis(widGraph *graph):
    widGraphElement(graph)
{}

void widGraphAxis::mousePressEvent(QMouseEvent *event)
{
    m_startingPoint = event->pos();
    m_isMouseZooming = false;
    m_isMouseMoving = false;
    auto ptr_control = ptr_graph->m_getData().lock()->m_control;
    if (ptr_control->m_zoom)
        m_isMouseZooming = true;
    else if (ptr_control->m_move)
        m_isMouseMoving = true;
}

void widGraphAxis::mouseMoveEvent(QMouseEvent */*event*/)
{
    update();
}


void widGraphAxis::mouseReleaseEvent(QMouseEvent */*event*/)
{
    if (m_isMouseZooming)
        m_zoomByMouse();
    if (m_isMouseMoving)
        m_moveByMouse();
    ptr_graph->m_loadValues();
}

void widGraphAxis::m_zoomByMouse()
{
    m_isMouseZooming = false;
    QPoint currentPoint = mapFromGlobal(QCursor::pos());
    if (m_supDistanceForActionIsSufficient(m_startingPoint, currentPoint)) {
        auto [start, end] = m_getStartAndEndFromMouse(m_startingPoint, currentPoint);
        m_setAxisMinMax(start, end);
    }
}

void widGraphAxis::m_moveByMouse()
{
    m_isMouseMoving = false;
    QPoint currentPoint = mapFromGlobal(QCursor::pos());
    if (m_supDistanceForActionIsSufficient(m_startingPoint, currentPoint)) {
        auto ptr_data = m_getData().lock();
        auto [min, max, step] = ptr_data->m_getMinMaxStep();
        auto [start, end] = m_getStartAndEndFromMouse(m_startingPoint, currentPoint);
        double diff = end - start;
        m_setAxisMinMax(min - diff, max - diff);
    }
}

void widGraphAxis::paintEvent(QPaintEvent *)
{
    if (m_getData().lock()->m_show) {
        // Painter
        painterAntiAl painter(this);
        // Pen
            QPen pen(Qt::black, 2);
            painter.setPen(pen);
        // Draw line
            m_drawLine(painter);
        // Draw ticks
            m_drawTicks(painter);
        // Draw numbers
            m_drawNumbers(painter);
        // Draw text
            m_drawText(painter);
        // Draw zoom cursor
            m_drawZoomCursor(painter);
        // Draw move cursor
            m_drawMoveCursor(painter);
    }
}

double widGraphAxis::m_supCalculateNiceNumbers(float range, bool round)
{
    double exponent;
    double fraction;
    double niceFraction;

    exponent = floor(log10(range));
    fraction = range / pow(10.f, exponent);

    if (round)
    {   if (fraction < 1.5)
            niceFraction = 1;
        else if (fraction < 3)
            niceFraction = 2;
        else if (fraction < 7)
            niceFraction = 5;
        else
            niceFraction = 10;
    }
    else
    {   if (fraction <= 1)
            niceFraction = 1;
        else if (fraction <= 2)
            niceFraction = 2;
        else if (fraction <= 5)
            niceFraction = 5;
        else
            niceFraction = 10;
    }

    return niceFraction * pow(10, exponent);
}

std::tuple<double, double, double> widGraphAxis::m_calculateNiceMaxMin(double min, double max)
{
    int maxTicks = 8;
    double range = m_supCalculateNiceNumbers(max - min, false);
    double tickSpacing = m_supCalculateNiceNumbers(range / (maxTicks - 1), true);
    double niceMin = floor(min / tickSpacing) * tickSpacing;
    double niceMax = ceil(max / tickSpacing) * tickSpacing;
    return {niceMin, niceMax, tickSpacing};
}

void widGraphAxis::m_setAxis()
{
    auto ptr_axisData = m_getData().lock();
    if (ptr_axisData->m_autoAxis) {
        // Min and max values
            auto [min, max] = m_getMinAndMaxOfObjects();
        // Calculate nice numbers
            auto [niceMin, niceMax, niceStep] = m_calculateNiceMaxMin(min, max);
        // Save them
            ptr_axisData->m_setMinMaxStep(niceMin, niceMax, niceStep);
    }
    else if (ptr_axisData->m_autoStep) {
        // Min and max values
            auto [min, max, step] = ptr_axisData->m_getMinMaxStep();
        // Calculate nice numbers
            auto [niceMin, niceMax, niceStep] = m_calculateNiceMaxMin(min, max);
        // Save them
            ptr_axisData->m_setStep(niceStep);
    }
}

void widGraphAxis::m_setAxisMinMax(double start, double end)
{
    auto ptr_data = m_getData().lock();
    ptr_data->m_autoAxis = false;
    ptr_data->m_autoStep = false;
    double min = std::min(start, end);
    double max = std::max(start, end);
    auto [niceMin, niceMax, niceStep] = m_calculateNiceMaxMin(min, max);
    ptr_data->m_setMinMaxStep(min, max, niceStep);
}

bool widGraphAxis::m_cancelOperation()
{
    bool res = m_isMouseZooming  || m_isMouseMoving;
    m_isMouseZooming = false;
    m_isMouseMoving = false;
    return res;
}

std::vector<double> widGraphAxis::m_getTicksPosition(double min, double max, double step)
{
    std::vector<double> vTicks;
    double numberX = min + step;
    while (numberX < max)
    {
        vTicks.push_back(numberX);
        numberX = numberX + step;
    }
    return vTicks;
}

bool widGraphAxis::m_supDistanceForActionIsSufficient(const QPoint &x1, const QPoint &x2)
{
    int dist_x = x1.x() - x2.x();
    int dist_y = x1.y() - x2.y();
    int pointsDistance = sqrt(dist_x*dist_x + dist_y*dist_y);
    if (pointsDistance > 20) return true;
    else return false;
}

widGraphY2Axis::widGraphY2Axis(widGraph *graph):
    widGraphYAxes(graph, 1)
{
}

double widGraphYAxes::m_getDrawAreaPositionFromValue(double value)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double titleHeight = ptr_data->m_title->m_height;
    double xAXisHeight = ptr_data->m_X->m_height;
    double yStart = height() - xAXisHeight;
    auto [min, max, step] = m_getData().lock()->m_getMinMaxStep();
    return - (yStart-titleHeight)*(value-max)/(max-min);
}

double widGraphYAxes::m_getValueFromDrawAreaPosition(double position)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double titleHeight = ptr_data->m_title->m_height;
    double xAXisHeight = ptr_data->m_X->m_height;
    double yStart = height() - xAXisHeight;
    auto [min, max, step] = m_getData().lock()->m_getMinMaxStep();
    return - position*(max-min)/(yStart-titleHeight) + max;
}

void widGraphY2Axis::m_setDimensions()
{
    auto ptr_data = m_getData().lock();
    if (ptr_data->m_manualSize)
        ptr_data->m_width = ptr_data->m_manualSizeValue;
    else
        ptr_data->m_width = m_getTextEnds() + m_spaceBorder;
    setFixedWidth(ptr_data->m_width);
}

std::weak_ptr<dataAxis> widGraphY2Axis::m_getData()
{
    return ptr_graph->m_getData().lock()->m_Y2;
}

double widGraphYAxes::m_getPositionFromValue(double value)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenY = ptr_data->m_title->m_height;
    return m_getDrawAreaPositionFromValue(value) + startScreenY;
}

double widGraphYAxes::m_getValueFromPosition(double position)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenY = ptr_data->m_title->m_height;
    return m_getValueFromDrawAreaPosition(position - startScreenY);
}

void widGraphY2Axis::m_drawLine(painterAntiAl &painter)
{
    painter.save();
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenY = ptr_data->m_title->m_height;
    double endScreenY = height() - ptr_data->m_X->m_height;
    double right = m_getTicksStart();
    painter.drawLine(right,startScreenY,right,endScreenY);
    painter.restore();
}

double widGraphY2Axis::m_getTicksStart()
{
    return 0;
}

double widGraphY2Axis::m_getTicksEnd()
{
    return m_getTicksStart() + m_tickLength;
}

double widGraphY2Axis::m_getNumbersStart()
{
    return m_getTicksEnd() + m_spaceTicksToNumbers;
}

double widGraphY2Axis::m_getNumbersEnd()
{
    const auto ptr_dataY2 = m_getData().lock();
    double rowHeight = 3*m_rowSpacing*ptr_dataY2->m_fontNumbers;
    return m_getNumbersStart() + rowHeight;
}

double widGraphY2Axis::m_getTextStart()
{
    return m_getNumbersEnd() + m_spaceNumbersToText;
}

double widGraphY2Axis::m_getTextEnds()
{
    auto ptr_dataY2 = m_getData().lock();
    double rowHeight = m_rowSpacing*ptr_dataY2->m_fontText;
    return m_getTextStart() + rowHeight;
}

widGraphLegend::widGraphLegend(widGraph *graph):
    widGraphElement(graph)
{
}

void widGraphLegend::m_drawTopLine(painterAntiAl &painter)
{
    painter.drawLine(0,0, width(),0);
}

void widGraphLegend::m_drawTexts(painterAntiAl &painter)
{
    auto ptr_legendData = ptr_graph->m_getData().lock()->m_legend;
    const auto &listCurves = ptr_graph->m_getData().lock()->m_vectorOfObjects;
    int i = 0;
    double rowHeight = 1.2*ptr_legendData->m_fontText;
    QFont font;
    font.setPixelSize(ptr_legendData->m_fontText);
    painter.setFont(font);
    double startLeft = 10;
    for (const auto &var: listCurves) {
        auto ptr_curveData = var->m_getData().lock();
        auto [overwrite, text, showLegend] = ptr_curveData->m_getStyleOfLegend();
        if (showLegend) {
            std::string legendText;
            std::string axis = ptr_curveData->m_getPrefferedYAxis() == 0 ? "L" : "R";
            if (overwrite)
                legendText = text;
            else
                legendText = ptr_curveData->m_getName() + " [" + axis + "]" ;
            QRect rect(startLeft, i*rowHeight, width() - startLeft, rowHeight);
            painter.drawText(rect, QString::fromStdString(legendText), QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
            ++i;
        }
    }
}

void widGraphLegend::paintEvent(QPaintEvent */*event*/)
{
    painterAntiAl painter(this);
    m_drawTopLine(painter);
    m_drawTexts(painter);
}

void widGraphLegend::m_setDimensions()
{
    int numberCurves = m_getNCurvesWithLegend();
    auto ptr_legendData = ptr_graph->m_getData().lock()->m_legend;
    double rowHeight = 1.2*ptr_legendData->m_fontText;
    double height = numberCurves * rowHeight;
    setFixedHeight(height);
}

int widGraphLegend::m_getNCurvesWithLegend()
{
    int nCurvesWithLegend = 0;
    const auto &listCurves = ptr_graph->m_getData().lock()->m_vectorOfObjects;
    for (auto &var:listCurves) {
        auto [overwrite, text, show] = var->m_getData().lock()->m_getStyleOfLegend();
        if (show)
            ++nCurvesWithLegend;
    }
    return nCurvesWithLegend;
}


void widGraphElement::mouseDoubleClickEvent(QMouseEvent *event)
{
    ptr_graph->m_openDialog();
    event->accept();
}

widGraphButton::widGraphButton(widGraph *graph,
    const QImage &icon, const QImage &iconActive,
    const QString &tooltip):
    widGraphElement(graph),
    m_icon(icon), m_iconActive(iconActive)
{
    setToolTip(tooltip);
    setCursor(Qt::ArrowCursor);
    m_timerAnimation = new QTimer(this);
}

void widGraphButton::paintEvent(QPaintEvent */*event*/)
{
    if (m_isVisible) {
        // Painter
        painterAntiAl painter(this);
        // Pen
            m_drawBorder(painter);
        // Draw inside
            m_drawInside(painter);
    }
}

void widGraphButton::m_drawBorder(painterAntiAl &painter)
{
    painter.save();
    // Pen
        QPen pen;
        if ((m_isCheckable && m_isChecked)
            || (!m_isCheckable && m_isAnimation))
            pen = QPen(Qt::blue, 2);
        else
            pen = QPen(Qt::black, 1);
        painter.setPen(pen);
    // Draw border
        QRectF rectButton = QRectF(QPointF(0,0), m_size);
        painter.drawRect(rectButton);

    painter.restore();
}

void widGraphButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
    event->accept();
}

void widGraphButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isChecked = !m_isChecked;
        m_onClick();
        m_doAnimation();
        ptr_graph->m_loadValues();
    }
}

void widGraphButton::m_setDimensions()
{
    setFixedSize(m_size.toSize());
}

void widGraphButton::m_slotAnimationTimer()
{
    m_isAnimation = false;
    m_timerAnimation->stop();
    update();
}

void widGraphButton::m_doAnimation()
{
    int ms = 500;
    connect(m_timerAnimation, &QTimer::timeout,
            this, &widGraphButton::m_slotAnimationTimer);
    m_timerAnimation->setInterval(ms);
    m_timerAnimation->start();
    m_isAnimation = true;
}

void widGraphButton::m_drawInside(painterAntiAl &painter)
{
    painter.save();
    if (m_isCheckable && m_isChecked) {
        painter.drawImage(QRect(0, 0, width(), height()),
                          m_iconActive);
    }
    else {
        painter.drawImage(QRect(0, 0, width(), height()),
                          m_icon);
    }
    painter.restore();
}

widGraphTitleText::widGraphTitleText(widGraph *graph):
    widGraphElement(graph)
{
}

void widGraphTitleText::paintEvent(QPaintEvent */*event*/)
{
    // Painter
        painterAntiAl painter(this);
    // Pen
        QPen pen(Qt::black, 2);
        painter.setPen(pen);
    // Font
        auto ptr_dataTitle = ptr_graph->m_getData().lock()->m_title;
        QFont font;
        font.setPixelSize(ptr_dataTitle->m_fontText);
        painter.setFont(font);
    // Draw
        const std::string &title = ptr_dataTitle->m_text;
        QRectF rect = QRectF(QPointF(0,0),
                             QSizeF(width(), height()));
        painter.drawText(rect, QString::fromStdString(title),
                         QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
}

void widGraphTitleText::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget:: mouseDoubleClickEvent(event);
    event->ignore();
}

void widGraphTitleText::m_setDimensions()
{
  //  auto ptr_dataTitle = ptr_graph->m_getData().lock()->m_title;
  //  setFixedWidth(500);
}

widGraphButtonAutoAxes::widGraphButtonAutoAxes(widGraph *graph):
    widGraphButton(graph, QImage(":/images/autoAxes.png"), QImage(":/images/autoAxes.png"), "Set automatic axes")
{

}

void widGraphButtonAutoAxes::m_onClick()
{
    auto ptr_data = ptr_graph->m_getData().lock();
    ptr_data->m_control->m_setNothing();
    auto ptr_dataX = ptr_data->m_X;
    auto ptr_dataY1 = ptr_data->m_Y1;
    auto ptr_dataY2 = ptr_data->m_Y2;
    ptr_dataX->m_autoAxis = true;
    ptr_dataX->m_autoStep = true;
    ptr_dataY1->m_autoAxis = true;
    ptr_dataY1->m_autoStep = true;
    ptr_dataY2->m_autoAxis = true;
    ptr_dataY2->m_autoStep = true;
}

widGraphButtonZoomIn::widGraphButtonZoomIn(widGraph *graph):
    widGraphButton(graph, QImage(":/images/zoomOff.png"), QImage(":/images/zoomOn.png"), "Enable/disable zoom")
{
    // Set as checkable
        m_isCheckable = true;
}

void widGraphButtonZoomIn::m_loadValues()
{
    auto ptr_data = ptr_graph->m_getData().lock();
    m_isChecked = ptr_data->m_control->m_zoom;
}

void widGraphButtonZoomIn::m_onClick()
{
    auto ptr_data = ptr_graph->m_getData().lock();
    ptr_data->m_control->m_setZoom(m_isChecked);
}

widGraphButtonZoomOut::widGraphButtonZoomOut(widGraph *graph):
    widGraphButton(graph, QImage(":/images/zoomOut.png"), QImage(":/images/zoomOut.png"), "Zoom out")
{
}

void widGraphButtonZoomOut::m_onClick()
{
    auto ptr_data = ptr_graph->m_getData().lock();
    ptr_data->m_control->m_setNothing();
    ptr_graph->m_zoomOut();
}

widGraphButtonMove::widGraphButtonMove(widGraph *graph):
    widGraphButton(graph, QImage(":/images/moveOff.png"), QImage(":/images/moveOn.png"), "Enable/disable Move")
{
    // Set as checkable
        m_isCheckable = true;
}

void widGraphButtonMove::m_loadValues()
{
    auto ptr_data = ptr_graph->m_getData().lock();
    m_isChecked = ptr_data->m_control->m_move;
}

void widGraphButtonMove::m_onClick()
{
    auto ptr_data = ptr_graph->m_getData().lock();
    ptr_data->m_control->m_setMove(m_isChecked);
}

widGraphButtonScreenshot::widGraphButtonScreenshot(widGraph *graph):
    widGraphButton(graph, QImage(":/images/screenshot.png"), QImage(":/images/screenshot.png"), "Take a screenshot")
{
}

void widGraphButtonScreenshot::m_onClick()
{
    auto ptr_data = ptr_graph->m_getData().lock();
    ptr_data->m_control->m_setNothing();
    ptr_graph->m_takeScreenshot();
}

void widGraphYAxes::m_drawZoomCursor(painterAntiAl &painter)
{
    painter.save();
    if (m_isMouseZooming) {
        QPoint currentPoint = mapFromGlobal(QCursor::pos());
        if (m_supDistanceForActionIsSufficient(m_startingPoint, currentPoint)) {
            int center = (m_getNumbersStart() + m_getNumbersEnd())/2;
            int left = center - m_zoomCursorWidth/2;
            int right = center + m_zoomCursorWidth/2;
            // Start line
                QPoint startLeft(left, m_startingPoint.y());
                QPoint startRight(right, m_startingPoint.y());
                QLine topLine(startLeft, startRight);
                painter.drawLine(topLine);
            // End line
                QPoint endLeft(left, currentPoint.y());
                QPoint endRight(right, currentPoint.y());
                QLine bottomLine(endLeft, endRight);
                painter.drawLine(bottomLine);
            // Long line
                painter.drawLine(topLine.center(), bottomLine.center());
        }
    }
    painter.restore();
}

void widGraphYAxes::m_drawMoveCursor(painterAntiAl &painter)
{
    painter.save();
    QPoint currentPoint = mapFromGlobal(QCursor::pos());
    if (m_isMouseMoving) {
        if (m_supDistanceForActionIsSufficient(m_startingPoint, currentPoint)) {
            QPen penRect(Qt::black, 1, Qt::DashLine);
            painter.setPen(penRect);
            int x = (m_getNumbersStart() + m_getNumbersEnd())/2;
            QPoint start(x, m_startingPoint.y());
            QPoint end(x, currentPoint.y());
            QLine line(start, end);
            painter.setBrush(Qt::red);
            painter.drawEllipse(QPoint(x, m_startingPoint.y()), 2, 2);
            painter.drawLine(line);
        }
    }
    painter.restore();
}

graphLayout::graphLayout()
{
    setContentsMargins(0,0,0,0);
    setSpacing(0);
}

painterAntiAl::painterAntiAl(QPaintDevice *device):
    QPainter(device)
{
    setRenderHint(QPainter::Antialiasing);
}
