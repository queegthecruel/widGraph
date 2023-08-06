#include "widGraph.h"
#include "widOthers.h"

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
        QGridLayout *layBackground = new QGridLayout(this);
        layBackground->setContentsMargins(0,0,0,0);
        layBackground->setSpacing(0);
        layBackground->addWidget(m_widTitle, 0,0, 1,3);
        layBackground->addWidget(m_widX, 2,0, 1,3);
        layBackground->addWidget(m_widY1, 0,0, 3,1);
        layBackground->addWidget(m_widY2, 0,2, 3,1);
        layBackground->addWidget(m_widArea, 1,1, 1,1);
        layBackground->addWidget(m_widLegend, 3,0, 1,3);
      // Stylesheet
        setStyleSheet("background: transparent;");
}

void widGraph::m_addObject(std::shared_ptr<graphObjects> ptr_object)
{
    m_data->m_vectorOfObjects.push_back(ptr_object);
}

void widGraph::m_loadValues()
{
    // Set dimensions
        m_widLegend->m_setDimensions();
        m_widX->m_setDimensions();
        m_widY1->m_setDimensions();
        m_widY2->m_setDimensions();
        m_widTitle->m_setDimensions();

    // Set axis
        m_widX->m_setAutoAxis();
        m_widY1->m_setAutoAxis();
        m_widY2->m_setAutoAxis();
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

void widGraphDrawArea::m_drawHorGrid(painterAntiAl &painter)
{
    auto ptr_y = ptr_graph->m_getY1Axis();
    auto ptr_data = ptr_graph->m_getData().lock();
    const auto &ptr_dataY = ptr_data->m_Y1;
    const double &minY = ptr_dataY.m_min;
    const double &maxY = ptr_dataY.m_max;
    const double &stepY = ptr_dataY.m_step;

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
    const auto &ptr_dataX = ptr_graph->m_getData().lock()->m_X;
    const double &minX = ptr_dataX.m_min;
    const double &maxX = ptr_dataX.m_max;
    const double &stepX = ptr_dataX.m_step;

    double numberX = minX;
    while (numberX <= maxX)
    {
        double posX = ptr_x->m_getDrawAreaPositionFromValue(numberX);
        painter.drawLine(posX,0,posX,width());
        numberX = numberX + stepX;
    }
}

widGraphDrawArea::widGraphDrawArea(widGraph *graph): widGraphElement(graph)
{
  //  setStyleSheet("background:transparent;");
}

void widGraphDrawArea::paintEvent(QPaintEvent */*event*/)
{
    // Painter
    painterAntiAl painter(this);
    painter.save();

    // Curves
        const auto &vectorObjects = ptr_graph->m_getData().lock()->m_vectorOfObjects;
        for(auto &var: vectorObjects)
            var->m_drawItself(&painter, ptr_graph);

    // Grid
        QPen penGrid(Qt::gray, 1, Qt::DashLine);
        painter.setPen(penGrid);
        m_drawHorGrid(painter);
        m_drawVertGrid(painter);

    // Rectangle
        QPen penRect(Qt::black, 2);
        painter.setPen(penRect);
        QRect rect(0, 0, width()-1, height()-1);
        painter.drawRect(rect);

    // Painter
        painter.restore();
}

widGraphTitle::widGraphTitle(widGraph *graph): widGraphElement(graph)
{
    setStyleSheet("background:gray;");
}

void widGraphTitle::paintEvent(QPaintEvent */*event*/)
{

}

void widGraphTitle::m_setDimensions()
{
    const auto &ptr_dataTitle = ptr_graph->m_getData().lock()->m_title;
    setFixedHeight(ptr_dataTitle.m_height);
}

widGraphXAxis::widGraphXAxis(widGraph *graph): widGraphAxis(graph)
{
      setStyleSheet("background:transparent;");
}

double widGraphXAxis::m_getDrawAreaPositionFromValue(const double &value)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    const double &startScreenX = ptr_data->m_Y1.m_width;
    const double &endScreenX = width() - ptr_data->m_Y2.m_width;
    const double &minX = ptr_data->m_X.m_min;
    const double &maxX = ptr_data->m_X.m_max;
    return (endScreenX-startScreenX)*(value-minX)/(maxX-minX);
}

void widGraphXAxis::m_setAutoAxis()
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double minX = 0;
    double maxX = 0;
    for (auto &var:ptr_data->m_vectorOfObjects) {
        minX = std::min(minX, var->m_getMinX());
        maxX = std::max(maxX, var->m_getMaxX());
    }
    auto [niceMin, niceMax, niceStep] = m_calculateNiceMaxMin(minX, maxX);
    ptr_data->m_X.m_min = niceMin;
    ptr_data->m_X.m_max = niceMax;
    ptr_data->m_X.m_step = niceStep;
}

void widGraphXAxis::m_setDimensions()
{
    auto &ptr_dataX = ptr_graph->m_getData().lock()->m_X;
    ptr_dataX.m_height = m_getEndFromTop();
    setFixedHeight(ptr_dataX.m_height);
}

double widGraphXAxis::m_getPositionFromValue(const double &value)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    const double &startScreenX = ptr_data->m_Y1.m_width;
    return startScreenX + m_getDrawAreaPositionFromValue(value);
}

void widGraphXAxis::m_drawLine(painterAntiAl &painter)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    const double &startScreenX = ptr_data->m_Y1.m_width;
    const double &endScreenX = width() - ptr_data->m_Y2.m_width;
    painter.drawLine(startScreenX,0,endScreenX,0);
}

void widGraphXAxis::m_drawTicks(painterAntiAl &painter)
{
    painter.save();
    const auto &ptr_dataX = ptr_graph->m_getData().lock()->m_X;

    const double &minX = ptr_dataX.m_min;
    const double &maxX = ptr_dataX.m_max;
    const double &stepX = ptr_dataX.m_step;

    double numberX = minX;
    while (numberX <= maxX)
    {
        double posX = m_getPositionFromValue(numberX);
        painter.drawLine(QPoint(posX,0),
                         QPoint(posX,m_getTicksEndFromTop()));
        numberX = numberX + stepX;
    }
    painter.restore();
}

void widGraphXAxis::m_drawNumbers(painterAntiAl &painter)
{
    painter.save();
    const auto &ptr_dataX = ptr_graph->m_getData().lock()->m_X;
    QFont font;
    font.setPixelSize(ptr_dataX.m_fontNumbers);
    painter.setFont(font);

    const double &minX = ptr_dataX.m_min;
    const double &maxX = ptr_dataX.m_max;
    const double &stepX = ptr_dataX.m_step;

    double numberX = minX;
    while (numberX <= maxX)
    {
        double posX = m_getPositionFromValue(numberX);
        QRect rect(QPoint(posX - 20, m_getNumbersStartFromTop()),
                   QPoint(posX + 20, m_getNumberEndsFromTop()));
        painter.drawText(rect, QString::number(numberX),
                         QTextOption(Qt::AlignCenter));
        numberX = numberX + stepX;
    }
    painter.restore();
}

void widGraphXAxis::m_drawText(painterAntiAl &painter)
{
    painter.save();
    const auto &ptr_dataX = ptr_graph->m_getData().lock()->m_X;
    QFont font;
    font.setPixelSize(ptr_dataX.m_fontText);
    painter.setFont(font);

    const auto &ptr_dataY2 = ptr_graph->m_getData().lock()->m_Y2;
    double y2width = ptr_dataY2.m_width;
    QRect rect(QPoint(0, m_getTextStartFromTop()),
               QPoint(width() - y2width, m_getTextEndsFromTop()));
    std::string text = ptr_dataX.m_text;
    if (ptr_dataX.m_unit != "")
        text = text + " [" + ptr_dataX.m_unit + "]";
    painter.drawText(rect, QString::fromStdString(text),
                     QTextOption(Qt::AlignRight | Qt::AlignVCenter));
    painter.restore();
}

double widGraphXAxis::m_getTicksEndFromTop()
{
    return m_tickLength;
}

double widGraphXAxis::m_getNumbersStartFromTop()
{
    return m_getTicksEndFromTop();
}

double widGraphXAxis::m_getNumberEndsFromTop()
{
    const auto &ptr_dataX = ptr_graph->m_getData().lock()->m_X;
    double rowHeight = 1.2*ptr_dataX.m_fontNumbers;
    return m_getNumbersStartFromTop() + rowHeight;
}

double widGraphXAxis::m_getTextStartFromTop()
{
    return m_getNumberEndsFromTop();
}

double widGraphXAxis::m_getTextEndsFromTop()
{
    const auto &ptr_dataX = ptr_graph->m_getData().lock()->m_X;
    double rowHeight = 1.2*ptr_dataX.m_fontText;
    return m_getTextStartFromTop() + rowHeight;
}

double widGraphXAxis::m_getEndFromTop()
{
    return m_getTextEndsFromTop() + 3;
}

widGraphY1Axis::widGraphY1Axis(widGraph *graph): widGraphAxis(graph)
{

}

double widGraphY1Axis::m_getDrawAreaPositionFromValue(const double &value)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    const double &titleHeight = ptr_data->m_title.m_height;
    const double &xAXisHeight = ptr_data->m_X.m_height;
    const double &yStart = height() - xAXisHeight;

    const double &minY = ptr_data->m_Y1.m_min;
    const double &maxY = ptr_data->m_Y1.m_max;
    return - (yStart-titleHeight)*(value-maxY)/(maxY-minY);
}

void widGraphY1Axis::m_setAutoAxis()
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double minY = 0;
    double maxY = 0;
    for (auto &var:ptr_data->m_vectorOfObjects) {
        if(var->m_getPrefferedYAxis() == 0) {
            minY = std::min(minY, var->m_getMinY());
            maxY = std::max(maxY, var->m_getMaxY());
        }
    }
    auto [niceMin, niceMax, niceStep] = m_calculateNiceMaxMin(minY, maxY);
    ptr_data->m_Y1.m_min = niceMin;
    ptr_data->m_Y1.m_max = niceMax;
    ptr_data->m_Y1.m_step = niceStep;
}

void widGraphY1Axis::m_setDimensions()
{
    const auto &ptr_dataY1 = ptr_graph->m_getData().lock()->m_Y1;
    setFixedWidth(ptr_dataY1.m_width);
}

double widGraphY1Axis::m_getPositionFromValue(const double &value)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    const double &startScreenY = ptr_data->m_title.m_height;
    return m_getDrawAreaPositionFromValue(value) + startScreenY;
}

void widGraphY1Axis::m_drawLine(painterAntiAl &painter)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    const double &startScreenY = ptr_data->m_title.m_height;
    const double &endScreenY = height() - ptr_data->m_X.m_height;
    const double &right = width() - 1;
    painter.drawLine(right,startScreenY,right,endScreenY);
}

void widGraphY1Axis::m_drawTicks(painterAntiAl &painter)
{
    const auto &ptr_dataY = ptr_graph->m_getData().lock()->m_Y1;
     const double &right = width() - 1;
     const double &minY = ptr_dataY.m_min;
     const double &maxY = ptr_dataY.m_max;
     const double &stepY = ptr_dataY.m_step;

     double numberY = minY;
     while (numberY <= maxY)
     {
         double posY = m_getPositionFromValue(numberY);
         painter.drawLine(right,posY,right - 10,posY);
         numberY = numberY + stepY;
     }
}

void widGraphY1Axis::m_drawNumbers(painterAntiAl &painter)
{
    const auto &ptr_dataY = ptr_graph->m_getData().lock()->m_Y1;
    const double &right = width() - 1;
    const double &minY = ptr_dataY.m_min;
    const double &maxY = ptr_dataY.m_max;
    const double &stepY = ptr_dataY.m_step;
    double numberY = minY;
    while (numberY <= maxY)
    {
        double posY = m_getPositionFromValue(numberY);
        QRect rect(right - 45, posY - 5, 30, 10);
        painter.drawText(rect, QString::number(numberY),
                         QTextOption(Qt::AlignRight | Qt::AlignVCenter));
        numberY = numberY + stepY;
    }
}

void widGraphAxis::paintEvent(QPaintEvent *)
{
    //Painter
        painterAntiAl painter(this);
        painter.save();
    //Pen
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
    // Painter
        painter.restore();
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
    int maxTicks = 10;
    double range = m_supCalculateNiceNumbers(max - min, false);
    double tickSpacing = m_supCalculateNiceNumbers(range / (maxTicks - 1), true);
    double niceMin = floor(min / tickSpacing) * tickSpacing;
    double niceMax = ceil(max / tickSpacing) * tickSpacing;
    return std::tuple<double, double, double>(niceMin, niceMax, tickSpacing);
}

widGraphY2Axis::widGraphY2Axis(widGraph *graph): widGraphAxis(graph)
{
}

double widGraphY2Axis::m_getDrawAreaPositionFromValue(const double &value)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    const double &titleHeight = ptr_data->m_title.m_height;
    const double &xAXisHeight = ptr_data->m_X.m_height;
    const double &yStart = height() - xAXisHeight;

    const double &minY = ptr_data->m_Y2.m_min;
    const double &maxY = ptr_data->m_Y2.m_max;
    return - (yStart-titleHeight)*(value-maxY)/(maxY-minY);
}

void widGraphY2Axis::m_setAutoAxis()
{
    auto ptr_data = ptr_graph->m_getData().lock();
    double minY = 0;
    double maxY = 0;
    for (auto &var:ptr_data->m_vectorOfObjects) {
        if(var->m_getPrefferedYAxis() == 1) {
            minY = std::min(minY, var->m_getMinY());
            maxY = std::max(maxY, var->m_getMaxY());
        }
    }

    auto [niceMin, niceMax, niceStep] = m_calculateNiceMaxMin(minY, maxY);
    ptr_data->m_Y2.m_min = niceMin;
    ptr_data->m_Y2.m_max = niceMax;
    ptr_data->m_Y2.m_step = niceStep;
}

void widGraphY2Axis::m_setDimensions()
{
    const auto &ptr_dataY2 = ptr_graph->m_getData().lock()->m_Y2;
    setFixedWidth(ptr_dataY2.m_width);
}

double widGraphY2Axis::m_getPositionFromValue(const double &value)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    const double &startScreenY = ptr_data->m_title.m_height;
    return m_getDrawAreaPositionFromValue(value) + startScreenY;
}

void widGraphY2Axis::m_drawLine(painterAntiAl &painter)
{
    auto ptr_data = ptr_graph->m_getData().lock();
    const double &startScreenY = ptr_data->m_title.m_height;
    const double &endScreenY = height() - ptr_data->m_X.m_height;
    painter.drawLine(0,startScreenY,0,endScreenY);
}

void widGraphY2Axis::m_drawTicks(painterAntiAl &painter)
{
    const auto &ptr_dataY = ptr_graph->m_getData().lock()->m_Y2;
    const double &minY = ptr_dataY.m_min;
    const double &maxY = ptr_dataY.m_max;
    const double &stepY = ptr_dataY.m_step;

    double numberY = minY;
    while (numberY <= maxY)
    {
        double posY = m_getPositionFromValue(numberY);
        painter.drawLine(0,posY,0 + 10,posY);
        numberY = numberY + stepY;
    }
}

void widGraphY2Axis::m_drawNumbers(painterAntiAl &painter)
{
    const auto &ptr_dataY = ptr_graph->m_getData().lock()->m_Y2;
    const double &minY = ptr_dataY.m_min;
    const double &maxY = ptr_dataY.m_max;
    const double &stepY = ptr_dataY.m_step;
    double numberY = minY;
    while (numberY <= maxY)
    {
        double posY = m_getPositionFromValue(numberY);
        QRect rect(15, posY - 5, 30, 10);
        painter.drawText(rect, QString::number(numberY),
                         QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
        numberY = numberY + stepY;
    }

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
    const auto &ptr_legendData = ptr_graph->m_getData().lock()->m_legend;
    const auto &listCurves = ptr_graph->m_getData().lock()->m_vectorOfObjects;
    int i = 0;
    double rowHeight = 1.2*ptr_legendData.m_fontHeight;
    QFont font;
    font.setPixelSize(ptr_legendData.m_fontHeight);
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
    double rowHeight = 1.2*ptr_legendData.m_fontHeight;
    double height = numberCurves * rowHeight;
    setFixedHeight(height);
}
