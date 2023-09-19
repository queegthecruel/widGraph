#include "widGraph.h"
#include "widOthers.h"
#include "dialogGraph.h"
#include <QApplication>
#include <QClipboard>>

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
        graphLayout *layBackground = new graphLayout(this);
        layBackground->addWidget(m_widTitle, 0,0, 1,3);
        layBackground->addWidget(m_widX, 2,0, 1,3);
        layBackground->addWidget(m_widY1, 0,0, 3,1);
        layBackground->addWidget(m_widY2, 0,2, 3,1);
        layBackground->addWidget(m_widArea, 1,1, 1,1);
        layBackground->addWidget(m_widLegend, 3,0, 1,3);
      // Stylesheet
        setStyleSheet("background: transparent;");
        setFocusPolicy(Qt::StrongFocus);
}

void widGraph::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        m_widArea->m_cancelOperation();
        m_widX->m_cancelOperation();
        m_widY1->m_cancelOperation();
        m_widY2->m_cancelOperation();
        m_loadValues();
    }
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
    setUpdatesEnabled(true);
}

void widGraph::m_setCurveStyle(int curveIndex, int R, int G, int B, int axis)
{
    if (curveIndex == -1)
        curveIndex = m_data->m_vectorOfObjects.size() - 1;

    auto ptr_curveData = m_data->m_vectorOfObjects[curveIndex]->m_getData().lock();
    ptr_curveData->m_R = R;
    ptr_curveData->m_G = G;
    ptr_curveData->m_B = B;
    ptr_curveData->m_prefferedYAxis = axis;
}

void widGraph::m_setCurveName(int curveIndex, const std::string &name)
{
    if (curveIndex == -1)
        curveIndex = m_data->m_vectorOfObjects.size() - 1;

    auto ptr_curveData = m_data->m_vectorOfObjects[curveIndex]->m_getData().lock();
    ptr_curveData->m_name = name;
}

void widGraph::m_openDialog(int tabIndex)
{
    if (m_dialog == nullptr) {
        m_dialog = std::make_unique<dialogGraph>(this, m_data, tabIndex);
        connect(m_dialog.get(), &QDialog::finished, this, &widGraph::m_slotDialogClosed);
        m_dialog->show();
    }
}

void widGraph::m_takeScreenshot()
{
    setStyleSheet(".QWidget {background: white;}");
    int ratio = 5;
    QSize imageSize = size()*ratio;
    QImage image = QImage(imageSize, QImage::Format_RGB32);
    image.setDevicePixelRatio(ratio);
    render(&image);
    QApplication::clipboard()->setImage(image);
    setStyleSheet(".QWidget {background: transparent;}");
}

void widGraph::m_slotDialogClosed(int status)
{
    m_dialog = nullptr;
}

void widGraphDrawArea::m_drawHorGrid(painterAntiAl &painter)
{
    auto ptr_y = ptr_graph->m_getY1Axis();
    auto ptr_data = ptr_graph->m_getData().lock();
    auto ptr_dataY = ptr_data->m_Y1;
    double minY = ptr_dataY->m_min;
    double maxY = ptr_dataY->m_max;
    double stepY = ptr_dataY->m_step;

    double numberY = minY;
    while (numberY <= maxY)
    {
        double posY = ptr_y->m_getDrawAreaPositionFromValue(numberY);
        painter.drawLine(0,posY,width(),posY);
        numberY = numberY + stepY;
    }
}

void widGraphDrawArea::m_drawVertGrid(painterAntiAl &painter)
{
    auto ptr_x = ptr_graph->m_getXAxis();
    auto ptr_dataX = ptr_graph->m_getData().lock()->m_X;
    double minX = ptr_dataX->m_min;
    double maxX = ptr_dataX->m_max;
    double stepX = ptr_dataX->m_step;

    double numberX = minX;
    while (numberX <= maxX)
    {
        double posX = ptr_x->m_getDrawAreaPositionFromValue(numberX);
        painter.drawLine(posX,0,posX,width());
        numberX = numberX + stepX;
    }
}

void widGraphDrawArea::m_setAxisMinMax(double startX, double endX, double startY1, double endY1, double startY2, double endY2)
{
    ptr_graph->m_getXAxis()->m_setAxisMinMax(startX, endX);
    ptr_graph->m_getY1Axis()->m_setAxisMinMax(startY1, endY1);
    ptr_graph->m_getY2Axis()->m_setAxisMinMax(startY2, endY2);
}

void widGraphDrawArea::m_cancelOperation()
{
    m_isMouseZooming = false;
    m_isMouseDragging = false;
}

widGraphDrawArea::widGraphDrawArea(widGraph *graph):
    widGraphElement(graph, dataGraph::DRAWAREA)
{
    //  setStyleSheet("background:transparent;");
}

void widGraphDrawArea::mousePressEvent(QMouseEvent *event)
{
    m_startingPoint = event->pos();
    auto ptr_control = ptr_graph->m_getData().lock()->m_control;
    if (ptr_control->m_zoom) {
        m_isMouseZooming = true;
        m_isMouseDragging = false;
    }
    else if (ptr_control->m_move) {
        m_isMouseDragging = true;
        m_isMouseZooming = false;
    }
    else {
        m_isMouseZooming = false;
        m_isMouseDragging = false;
    }
}

void widGraphDrawArea::mouseMoveEvent(QMouseEvent *event)
{
    update();
}

void widGraphDrawArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isMouseZooming) {
        m_isMouseZooming = false;
        QPoint currentPoint = mapFromGlobal(QCursor::pos());

        if (widGraphAxis::m_supDistanceForZoomIsSufficient(m_startingPoint, currentPoint)) {
        double startX = ptr_graph->m_getXAxis()->m_getValueFromDrawAreaPosition(m_startingPoint.x());
        double startY1 = ptr_graph->m_getY1Axis()->m_getValueFromDrawAreaPosition(m_startingPoint.y());
        double startY2 = ptr_graph->m_getY2Axis()->m_getValueFromDrawAreaPosition(m_startingPoint.y());

        double endX = ptr_graph->m_getXAxis()->m_getValueFromDrawAreaPosition(currentPoint.x());
        double endY1 = ptr_graph->m_getY1Axis()->m_getValueFromDrawAreaPosition(currentPoint.y());
        double endY2 = ptr_graph->m_getY2Axis()->m_getValueFromDrawAreaPosition(currentPoint.y());

        m_setAxisMinMax(startX, endX, startY1, endY1, startY2, endY2);
            ptr_graph->m_loadValues();
        }
        else
            update();
        m_startingPoint = QPoint(0,0);
    }
    if (m_isMouseDragging) {
        m_isMouseDragging = false;
        QPoint currentPoint = mapFromGlobal(QCursor::pos());
        double startX = ptr_graph->m_getXAxis()->m_getValueFromDrawAreaPosition(m_startingPoint.x());
        double startY1 = ptr_graph->m_getY1Axis()->m_getValueFromDrawAreaPosition(m_startingPoint.y());
        double startY2 = ptr_graph->m_getY2Axis()->m_getValueFromDrawAreaPosition(m_startingPoint.y());

        double endX = ptr_graph->m_getXAxis()->m_getValueFromDrawAreaPosition(currentPoint.x());
        double endY1 = ptr_graph->m_getY1Axis()->m_getValueFromDrawAreaPosition(currentPoint.y());
        double endY2 = ptr_graph->m_getY2Axis()->m_getValueFromDrawAreaPosition(currentPoint.y());

        double deltaX = startX - endX;
        double deltaY1 = startY1 - endY1;
        double deltaY2 = startY2 - endY2;

        double currStartX = ptr_graph->m_getData().lock()->m_X->m_min;
        double currEndX = ptr_graph->m_getData().lock()->m_X->m_max;
        double currStartY1 = ptr_graph->m_getData().lock()->m_Y1->m_min;
        double currEndY1 = ptr_graph->m_getData().lock()->m_Y1->m_max;
        double currStartY2 = ptr_graph->m_getData().lock()->m_Y2->m_min;
        double currEndY2 = ptr_graph->m_getData().lock()->m_Y2->m_max;

        m_setAxisMinMax(currStartX + deltaX, currEndX + deltaX,
                        currStartY1 + deltaY1, currEndY1 + deltaY1,
                        currStartY2 + deltaY2, currEndY2 + deltaY2);
    }
    ptr_graph->m_loadValues();
}

void widGraphDrawArea::paintEvent(QPaintEvent */*event*/)
{
    // Painter
    painterAntiAl painter(this);
    painter.save();

    // Curves
        auto vectorObjects = ptr_graph->m_getData().lock()->m_vectorOfObjects;
        for(auto &var: vectorObjects)
            var->m_drawItself(&painter, ptr_graph);

    // Grid
        auto ptr_data = ptr_graph->m_getData().lock()->m_drawArea;
        if (ptr_data->m_showGrid) {
            QPen penGrid(Qt::gray, 1, Qt::DashLine);
            painter.setPen(penGrid);
            m_drawHorGrid(painter);
            m_drawVertGrid(painter);
        }

    // Border
        QPen penBorder(Qt::black, 2);
        painter.setPen(penBorder);
        QRect rectBorder(0, 0, width()-1, height()-1);
        painter.drawRect(rectBorder);

    // Selection rectangle
        QPoint currentPoint = mapFromGlobal(QCursor::pos());
        if (m_isMouseZooming
            && widGraphAxis::m_supDistanceForZoomIsSufficient(m_startingPoint, currentPoint)) {
            QPen penRect(Qt::black, 1, Qt::DashLine);
            painter.setPen(penRect);
            QRect rect(m_startingPoint, currentPoint);
            painter.drawRect(rect);
        }
    // Move trajectory
        if (m_isMouseDragging) {
            QPen penRect(Qt::black, 1, Qt::DashLine);
            painter.setPen(penRect);
            QLine line(m_startingPoint, currentPoint);
            painter.setBrush(Qt::red);
            painter.drawEllipse(m_startingPoint, 2, 2);
            painter.drawLine(line);
        }
    // Painter
        painter.restore();
}

widGraphTitle::widGraphTitle(widGraph *graph):
    widGraphElement(graph, dataGraph::TITLE)
{
    m_text = new widGraphTitleText(ptr_graph);
    m_butAuto = new widGraphButtonAutoAxes(ptr_graph);
    m_butShowGrid = new widGraphButtonShowGrid(ptr_graph);
    m_butZoom = new widGraphButtonZoom(ptr_graph);
    m_butMove = new widGraphButtonMove(ptr_graph);
    m_butScreenshot = new widGraphButtonScreenshot(ptr_graph);
    m_layBackground = new HBoxLayout(this);
    m_layBackground->setSpacing(2);
    m_layBackground->addWidget(m_text);
    m_layBackground->addWidget(m_butZoom);
    m_layBackground->addWidget(m_butMove);
    m_layBackground->addWidget(m_butAuto);
    m_layBackground->addWidget(m_butShowGrid);
    m_layBackground->addWidget(m_butScreenshot);
}

void widGraphTitle::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
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
    m_butZoom->m_setDimensions();
    m_butZoom->m_loadValues();
    m_butMove->m_setDimensions();
    m_butMove->m_loadValues();
    m_butAuto->m_setDimensions();
    m_butShowGrid->m_setDimensions();
    m_butScreenshot->m_setDimensions();
}

widGraphXAxis::widGraphXAxis(widGraph *graph):
    widGraphAxis(graph, dataGraph::X)
{

}

double widGraphXAxis::m_getDrawAreaPositionFromValue(double value)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenX = ptr_data->m_Y1->m_width;
    double endScreenX = width() - ptr_data->m_Y2->m_width;

    double minX = ptr_data->m_X->m_min;
    double maxX = ptr_data->m_X->m_max;
    return (endScreenX-startScreenX)*(value-minX)/(maxX-minX);
}

double widGraphXAxis::m_getValueFromDrawAreaPosition(double position)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenX = ptr_data->m_Y1->m_width;
    double y2axisWidth= ptr_data->m_Y2->m_width;
    double endScreenX = width() - y2axisWidth;

    double minX = ptr_data->m_X->m_min;
    double maxX = ptr_data->m_X->m_max;
    double ret = position*(maxX-minX)/(endScreenX-startScreenX) + minX;
    return ret;
}

std::tuple<double, double> widGraphXAxis::m_getMinAndMax()
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
    auto ptr_dataX = ptr_graph->m_getData().lock()->m_X;
    ptr_dataX->m_height = m_getEndFromTop();
    setFixedHeight(ptr_dataX->m_height);
}

std::weak_ptr<dataAxis> widGraphXAxis::m_getData()
{
    return ptr_graph->m_getData().lock()->m_X;
}

double widGraphXAxis::m_getPositionFromValue(double value)
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
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenX = ptr_data->m_Y1->m_width;
    double endScreenX = width() - ptr_data->m_Y2->m_width;
    painter.drawLine(startScreenX,0,endScreenX,0);
}

void widGraphXAxis::m_drawTicks(painterAntiAl &painter)
{
    painter.save();
    auto ptr_dataX = ptr_graph->m_getData().lock()->m_X;

    double minX = ptr_dataX->m_min;
    double maxX = ptr_dataX->m_max;
    double stepX = ptr_dataX->m_step;

    double numberX = minX;
    while (numberX <= maxX)
    {
        double posX = m_getPositionFromValue(numberX);
        painter.drawLine(QPoint(posX,m_getTicksStart()),
                         QPoint(posX,m_getTicksEnd()));
        numberX = numberX + stepX;
    }
    painter.restore();
}

void widGraphXAxis::m_drawNumbers(painterAntiAl &painter)
{
    painter.save();
    auto ptr_dataX = ptr_graph->m_getData().lock()->m_X;
    QFont font;
    font.setPixelSize(ptr_dataX->m_fontNumbers);
    painter.setFont(font);

    double minX = ptr_dataX->m_min;
    double maxX = ptr_dataX->m_max;
    double stepX = ptr_dataX->m_step;

    int nSteps = (maxX - minX)/stepX;
    double numberLength = width()/nSteps/2;
    double numberX = minX;
    while (numberX <= maxX)
    {
        double posX = m_getPositionFromValue(numberX);
        QRect rect(QPoint(posX - numberLength, m_getNumbersStart()),
                   QPoint(posX + numberLength, m_getNumbersEnd()));
        painter.drawText(rect, QString::number(numberX, 'f', 2),
                         QTextOption(Qt::AlignCenter));
        numberX = numberX + stepX;
    }
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
        if (m_supDistanceForZoomIsSufficient(m_startingPoint, currentPoint)) {
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

widGraphY1Axis::widGraphY1Axis(widGraph *graph):
    widGraphAxis(graph, dataGraph::Y1)
{

}

double widGraphY1Axis::m_getDrawAreaPositionFromValue(double value)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double titleHeight = ptr_data->m_title->m_height;
    double xAXisHeight = ptr_data->m_X->m_height;
    double yStart = height() - xAXisHeight;

    double minY = ptr_data->m_Y1->m_min;
    double maxY = ptr_data->m_Y1->m_max;
    return - (yStart-titleHeight)*(value-maxY)/(maxY-minY);
}

double widGraphY1Axis::m_getValueFromDrawAreaPosition(double position)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double titleHeight = ptr_data->m_title->m_height;
    double xAXisHeight = ptr_data->m_X->m_height;
    double yStart = height() - xAXisHeight;

    double minY = ptr_data->m_Y1->m_min;
    double maxY = ptr_data->m_Y1->m_max;
    return - position*(maxY-minY)/(yStart-titleHeight) + maxY;
}

std::tuple<double, double> widGraphY1Axis::m_getMinAndMax()
{
    auto ptr_graphData = ptr_graph->m_getData().lock();
    double minY = 0;
    double maxY = 0;
    for (auto &var:ptr_graphData->m_vectorOfObjects) {
        if(var->m_getPrefferedYAxis() == 0) {
            minY = std::min(minY, var->m_getMinY());
            maxY = std::max(maxY, var->m_getMaxY());
        }
    }
    return {minY, maxY};
}

void widGraphY1Axis::m_setDimensions()
{
    auto ptr_dataY1 = ptr_graph->m_getData().lock()->m_Y1;
    ptr_dataY1->m_width = m_getTicksStart();
    setFixedWidth(ptr_dataY1->m_width);
}

std::weak_ptr<dataAxis> widGraphY1Axis::m_getData()
{
    return ptr_graph->m_getData().lock()->m_Y1;
}

double widGraphY1Axis::m_getPositionFromValue(double value)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenY = ptr_data->m_title->m_height;
    return m_getDrawAreaPositionFromValue(value) + startScreenY;
}

double widGraphY1Axis::m_getValueFromPosition(double position)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenY = ptr_data->m_title->m_height;
    return m_getValueFromDrawAreaPosition(position - startScreenY);
}

std::tuple<double, double> widGraphY1Axis::m_getStartAndEndFromMouse(QPointF start, QPointF end)
{
    double startY = m_getValueFromPosition(start.y());
    double endY = m_getValueFromPosition(end.y());
    return {startY, endY};
}

void widGraphY1Axis::m_drawLine(painterAntiAl &painter)
{
    painter.save();
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenY = ptr_data->m_title->m_height;
    double endScreenY = height() - ptr_data->m_X->m_height;
    double right = width() - 1;
    painter.drawLine(right,startScreenY,right,endScreenY);
    painter.restore();
}

void widGraphY1Axis::m_drawTicks(painterAntiAl &painter)
{
    painter.save();
    auto ptr_dataY = ptr_graph->m_getData().lock()->m_Y1;
     double minY = ptr_dataY->m_min;
     double maxY = ptr_dataY->m_max;
     double stepY = ptr_dataY->m_step;

     double numberY = minY;
     while (numberY <= maxY)
     {
         double posY = m_getPositionFromValue(numberY);
         painter.drawLine(QPoint(m_getTicksStart(),posY),
                          QPoint(m_getTicksEnd(),posY));
         numberY = numberY + stepY;
     }
     painter.restore();
}

void widGraphY1Axis::m_drawNumbers(painterAntiAl &painter)
{
    painter.save();
    auto ptr_dataY = ptr_graph->m_getData().lock()->m_Y1;
    QFont font;
    font.setPixelSize(ptr_dataY->m_fontNumbers);
    painter.setFont(font);

    double minY = ptr_dataY->m_min;
    double maxY = ptr_dataY->m_max;
    double stepY = ptr_dataY->m_step;

    double numberY = minY;
    while (numberY <= maxY)
    {
        double posY = m_getPositionFromValue(numberY);
        QRect rect(QPoint(m_getNumbersEnd(), posY - ptr_dataY->m_fontNumbers/2),
                   QPoint(m_getNumbersStart(), posY + ptr_dataY->m_fontNumbers/2));
        painter.drawText(rect, QString::number(numberY, 'f', 2),
                         QTextOption(Qt::AlignRight | Qt::AlignVCenter));
        numberY = numberY + stepY;
    }
    painter.restore();
}

void widGraphY1Axis::m_drawText(painterAntiAl &painter)
{
    painter.save();
    auto ptr_dataY = ptr_graph->m_getData().lock()->m_Y1;
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

void widGraphY1Axis::m_drawZoomCursor(painterAntiAl &painter)
{
    painter.save();
    if (m_isMouseZooming) {
        QPoint currentPoint = mapFromGlobal(QCursor::pos());
        if (m_supDistanceForZoomIsSufficient(m_startingPoint, currentPoint)) {
            int right = m_getNumbersStart();
            int left = right - m_zoomCursorWidth;
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

double widGraphY1Axis::m_getTicksStart()
{
    return m_getTicksEnd() + m_tickLength;
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

widGraphAxis::widGraphAxis(widGraph *graph, int elementNumber):
    widGraphElement(graph, elementNumber)
{}

void widGraphAxis::mousePressEvent(QMouseEvent *event)
{
    auto ptr_control = ptr_graph->m_getData().lock()->m_control;
    if (ptr_control->m_zoom) {
        m_isMouseZooming = true;
        m_startingPoint = event->pos();
    }
    else {
        m_isMouseZooming = false;
        m_startingPoint = QPoint(0,0);
    }
}

void widGraphAxis::mouseMoveEvent(QMouseEvent *event)
{
    update();
}

void widGraphAxis::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isMouseZooming) {
        m_isMouseZooming = false;
        QPoint currentPoint = mapFromGlobal(QCursor::pos());
        if (m_supDistanceForZoomIsSufficient(m_startingPoint, currentPoint)) {
            auto [start, end] = m_getStartAndEndFromMouse(m_startingPoint, currentPoint);
            m_setAxisMinMax(start, end);
            ptr_graph->m_loadValues();
        }
        else
            update();
        m_startingPoint = QPoint(0,0);
    }
}

void widGraphAxis::paintEvent(QPaintEvent *)
{
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
    int maxTicks = 7;
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
            auto [min, max] = m_getMinAndMax();
        // Calculate nice numbers
            auto [niceMin, niceMax, niceStep] = m_calculateNiceMaxMin(min, max);
        // Save them
            ptr_axisData->m_setMinMaxStep(niceMin, niceMax, niceStep);
    }
    else if (ptr_axisData->m_autoStep) {
        // Min and max values
            double min = ptr_axisData->m_min;
            double max = ptr_axisData->m_max;
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

void widGraphAxis::m_cancelOperation()
{
    m_isMouseZooming = false;
}

bool widGraphAxis::m_supDistanceForZoomIsSufficient(const QPoint &x1, const QPoint &x2)
{
    int dist_x = x1.x() - x2.x();
    int dist_y = x1.y() - x2.y();
    int pointsDistance = sqrt(dist_x*dist_x + dist_y*dist_y);
    if (pointsDistance > 20) return true;
    else return false;
}

widGraphY2Axis::widGraphY2Axis(widGraph *graph):
    widGraphAxis(graph, dataGraph::Y2)
{
}

double widGraphY2Axis::m_getDrawAreaPositionFromValue(double value)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double titleHeight = ptr_data->m_title->m_height;
    double xAXisHeight = ptr_data->m_X->m_height;
    double yStart = height() - xAXisHeight;

    double minY = ptr_data->m_Y2->m_min;
    double maxY = ptr_data->m_Y2->m_max;
    return - (yStart-titleHeight)*(value-maxY)/(maxY-minY);
}

double widGraphY2Axis::m_getValueFromDrawAreaPosition(double position)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double titleHeight = ptr_data->m_title->m_height;
    double xAXisHeight = ptr_data->m_X->m_height;
    double yStart = height() - xAXisHeight;

    double minY = ptr_data->m_Y2->m_min;
    double maxY = ptr_data->m_Y2->m_max;
    return - position*(maxY-minY)/(yStart-titleHeight) + maxY;
}

std::tuple<double, double> widGraphY2Axis::m_getMinAndMax()
{
    auto ptr_graphData = ptr_graph->m_getData().lock();
    double minY = 0;
    double maxY = 0;
    for (auto &var: ptr_graphData->m_vectorOfObjects) {
        if(var->m_getPrefferedYAxis() == 1) {
            minY = std::min(minY, var->m_getMinY());
            maxY = std::max(maxY, var->m_getMaxY());
        }
    }
    return {minY, maxY};
}

void widGraphY2Axis::m_setDimensions()
{
    auto ptr_dataY2 = m_getData().lock();
    ptr_dataY2->m_width = m_getTextEnds() + m_spaceBorder;
    setFixedWidth(ptr_dataY2->m_width);
}

std::weak_ptr<dataAxis> widGraphY2Axis::m_getData()
{
    return ptr_graph->m_getData().lock()->m_Y2;
}

double widGraphY2Axis::m_getPositionFromValue(double value)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenY = ptr_data->m_title->m_height;
    return m_getDrawAreaPositionFromValue(value) + startScreenY;
}

double widGraphY2Axis::m_getValueFromPosition(double position)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenY = ptr_data->m_title->m_height;
    return m_getValueFromDrawAreaPosition(position - startScreenY);
}

std::tuple<double, double> widGraphY2Axis::m_getStartAndEndFromMouse(QPointF start, QPointF end)
{
    double startY = m_getValueFromPosition(start.y());
    double endY = m_getValueFromPosition(end.y());
    return {startY, endY};
}

void widGraphY2Axis::m_drawLine(painterAntiAl &painter)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double startScreenY = ptr_data->m_title->m_height;
    double endScreenY = height() - ptr_data->m_X->m_height;
    painter.drawLine(0,startScreenY,0,endScreenY);
}

void widGraphY2Axis::m_drawTicks(painterAntiAl &painter)
{
    auto ptr_dataY = m_getData().lock();
    double minY = ptr_dataY->m_min;
    double maxY = ptr_dataY->m_max;
    double stepY = ptr_dataY->m_step;

    double numberY = minY;
    while (numberY <= maxY)
    {
        double posY = m_getPositionFromValue(numberY);
        painter.drawLine(QPoint(m_getTicksStart(),posY),
                         QPoint(m_getTicksEnd(),posY));
        numberY = numberY + stepY;
    }
}

void widGraphY2Axis::m_drawNumbers(painterAntiAl &painter)
{
    auto ptr_dataY = m_getData().lock();
    QFont font;
    font.setPixelSize(ptr_dataY->m_fontNumbers);
    painter.setFont(font);

    double minY = ptr_dataY->m_min;
    double maxY = ptr_dataY->m_max;
    double stepY = ptr_dataY->m_step;
    double numberY = minY;
    while (numberY <= maxY)
    {
        double posY = m_getPositionFromValue(numberY);
        QRect rect(QPoint(m_getNumbersStart(), posY - ptr_dataY->m_fontNumbers/2),
                   QPoint(m_getNumbersEnd(), posY + ptr_dataY->m_fontNumbers/2));
        painter.drawText(rect, QString::number(numberY, 'f', 2),
                         QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
        numberY = numberY + stepY;
    }

}

void widGraphY2Axis::m_drawText(painterAntiAl &painter)
{
    painter.save();
    auto ptr_dataY = m_getData().lock();
    QFont font;
    font.setPixelSize(ptr_dataY->m_fontText);
    painter.setFont(font);

    std::string text = ptr_dataY->m_text;
    if (ptr_dataY->m_unit != "")
        text = text + " [" + ptr_dataY->m_unit + "]";
    QRect rect(QPoint(0, m_getTextEnds()),
               QPoint(-height(), m_getTextStart()));
    painter.rotate(270);
    painter.drawText(rect, Qt::AlignCenter, QString::fromStdString(text));
    painter.restore();
}

void widGraphY2Axis::m_drawZoomCursor(painterAntiAl &painter)
{
    painter.save();
    if (m_isMouseZooming) {
        QPoint currentPoint = mapFromGlobal(QCursor::pos());
        if (m_supDistanceForZoomIsSufficient(m_startingPoint, currentPoint)) {
            int right = m_getNumbersEnd();
            int left = right - m_zoomCursorWidth;
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
    widGraphElement(graph, dataGraph::LEGEND)
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
        QRect rect(startLeft, i*rowHeight, width() - startLeft, rowHeight);
        auto ptr_curveData = var->m_getData().lock();
        QString axis = ptr_curveData->m_prefferedYAxis == 0 ? "L" : "R";
        QString text = QString::fromStdString(ptr_curveData->m_name) + " [" + axis + "]" ;
        painter.drawText(rect, text, QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
        ++i;
    }
}

void widGraphLegend::paintEvent(QPaintEvent *event)
{
    painterAntiAl painter(this);
    m_drawTopLine(painter);
    m_drawTexts(painter);
}

void widGraphLegend::m_setDimensions()
{
    const auto &listCurves = ptr_graph->m_getData().lock()->m_vectorOfObjects;
    int numberCurves = listCurves.size();
    auto ptr_legendData = ptr_graph->m_getData().lock()->m_legend;
    double rowHeight = 1.2*ptr_legendData->m_fontText;
    double height = numberCurves * rowHeight;
    setFixedHeight(height);
}


void widGraphElement::mouseDoubleClickEvent(QMouseEvent *event)
{
    ptr_graph->m_openDialog(m_elementNumber);
    event->accept();
}

widGraphButton::widGraphButton(widGraph *graph, const QString &tooltip):
    widGraphElement(graph, -1)
{
    setToolTip(tooltip);
}

void widGraphButton::paintEvent(QPaintEvent *event)
{
    // Painter
    painterAntiAl painter(this);
    // Pen
        m_drawBorder(painter);
    // Draw inside
        m_drawInside(painter);
}

void widGraphButton::m_drawBorder(painterAntiAl &painter)
{
    painter.save();
    // Pen
        QPen pen;
        if (m_isCheckable && m_isChecked)
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
    m_isChecked = !m_isChecked;
    m_onClick();
    ptr_graph->m_loadValues();
}

void widGraphButton::m_setDimensions()
{
    setFixedSize(m_size.toSize());
}

widGraphTitleText::widGraphTitleText(widGraph *graph):
    widGraphElement(graph, -1)
{
}

void widGraphTitleText::paintEvent(QPaintEvent *event)
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
                         QTextOption(Qt::AlignCenter));
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
    widGraphButton(graph, "Set automatic axes")
{

}

void widGraphButtonAutoAxes::m_onClick()
{
    auto ptr_data = ptr_graph->m_getData().lock();
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

widGraphButtonZoom::widGraphButtonZoom(widGraph *graph):
    widGraphButton(graph, "Enable/disable zoom")
{
    // Set as checkable
        m_isCheckable = true;
    // Initial value
//        m_loadValues();
}

void widGraphButtonZoom::m_loadValues()
{
    auto ptr_data = ptr_graph->m_getData().lock();
    m_isChecked = ptr_data->m_control->m_zoom;
}

void widGraphButtonZoom::m_onClick()
{
    auto ptr_data = ptr_graph->m_getData().lock();
    ptr_data->m_control->m_setZoom(m_isChecked);
}

void widGraphButtonZoom::m_drawInside(painterAntiAl &painter)
{
    painter.save();
    // Pen
        QPen pen;
        if (m_isChecked)
            pen = QPen(Qt::blue, 2);
        else
            pen = QPen(Qt::black, 1);
    // Font
        QFont font;
        font.setPixelSize(18);
        painter.setFont(font);
    // Draw
        painter.drawText(QRect(0, 0, width(), height()),
                         "Z", QTextOption(Qt::AlignCenter));
    painter.restore();
}


widGraphButtonMove::widGraphButtonMove(widGraph *graph):
    widGraphButton(graph, "Enable/disable Move")
{
    // Set as checkable
        m_isCheckable = true;
    // Initial value
//        m_loadValues();
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

void widGraphButtonMove::m_drawInside(painterAntiAl &painter)
{
    painter.save();
    // Pen
        QPen pen;
        if (m_isChecked)
            pen = QPen(Qt::blue, 2);
        else
            pen = QPen(Qt::black, 1);
    // Font
        QFont font;
        font.setPixelSize(18);
        painter.setFont(font);
    // Draw
        painter.drawText(QRect(0, 0, width(), height()),
                         "M", QTextOption(Qt::AlignCenter));
    painter.restore();
}


widGraphButtonShowGrid::widGraphButtonShowGrid(widGraph *graph):
    widGraphButton(graph, "Show/hide grid")
{
    // Set as checkable
        m_isCheckable = true;
    // Initial value
        auto ptr_data = ptr_graph->m_getData().lock();
        auto ptr_dataDrawArea = ptr_data->m_drawArea;
        m_isChecked = ptr_dataDrawArea->m_showGrid;
}

void widGraphButtonShowGrid::m_onClick()
{
    auto ptr_data = ptr_graph->m_getData().lock();
    auto ptr_dataDrawArea = ptr_data->m_drawArea;
    ptr_dataDrawArea->m_showGrid = m_isChecked;
}

void widGraphButtonShowGrid::m_drawInside(painterAntiAl &painter)
{
    painter.save();
    // Pen
        QPen pen;
        if (m_isChecked)
            pen = QPen(Qt::blue, 2);
        else
            pen = QPen(Qt::black, 1);
        painter.setPen(pen);
    // Draw grid
        double w = width()/4;
        double h = height()/4;
        painter.drawLine(w, 0, w, height());
        painter.drawLine(2*w, 0, 2*w, height());
        painter.drawLine(3*w, 0, 3*w, height());
        painter.drawLine(0, h, width(), h);
        painter.drawLine(0, 2*h, width(), 2*h);
        painter.drawLine(0, 3*h, width(), 3*h);
    painter.restore();
}

void widGraphButtonAutoAxes::m_drawInside(painterAntiAl &painter)
{
    painter.save();
    // Pen
        QPen pen;
        pen = QPen(Qt::blue, 1);
        painter.setPen(pen);
    // Font
        QFont font;
        font.setPixelSize(18);
        painter.setFont(font);
    // Draw
        painter.drawText(QRect(0, 0, width(), height()),
                         "A", QTextOption(Qt::AlignCenter));
    painter.restore();
}

widGraphButtonScreenshot::widGraphButtonScreenshot(widGraph *graph):
    widGraphButton(graph, "Take a screenshot")
{

}

void widGraphButtonScreenshot::m_onClick()
{
    ptr_graph->m_takeScreenshot();
}

void widGraphButtonScreenshot::m_drawInside(painterAntiAl &painter)
{
    painter.save();
    // Pen
        QPen pen;
        pen = QPen(Qt::blue, 1);
        painter.setPen(pen);
    // Font
        QFont font;
        font.setPixelSize(18);
        painter.setFont(font);
    // Draw
        painter.drawText(QRect(0, 0, width(), height()),
                         "S", QTextOption(Qt::AlignCenter));
    painter.restore();
}
