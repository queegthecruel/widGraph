#include "objectsGraph.h"
#include "dataGraph.h"
#include "widGraph.h"

graphCurve::graphCurve(std::shared_ptr<std::vector<double> > ptr_dataX, std::shared_ptr<std::vector<double> > ptr_dataY):
    w_dataX(ptr_dataX), w_dataY(ptr_dataY)
{
    m_data = std::make_shared<dataGraphObject>(true, true);

    s_dataY = w_dataY.lock();
    int noY = s_dataY->size();
    if (noY == 0) return;
    if (w_dataX.expired()) {
        s_dataX = std::make_shared<std::vector<double>>(noY, 0);
        double x = 0;
        for (auto &var: *s_dataX)
            var = x++;
    }
    else
        s_dataX = w_dataX.lock();
    qDebug() << "Curve 1";
}

graphCurve::graphCurve(std::shared_ptr<std::vector<double> > ptr_dataY):
    w_dataY(ptr_dataY)
{
    m_data = std::make_shared<dataGraphObject>(true, true);

    s_dataY = w_dataY.lock();
    int noY = s_dataY->size();
    if (noY == 0) return;
    if (w_dataX.expired()) {
        s_dataX = std::make_shared<std::vector<double>>(noY, 0);
        double x = 0;
        for (auto &var: *s_dataX)
            var = x++;
    }
    else
        s_dataX = w_dataX.lock();
    qDebug() << "Curve 2";
}

void graphCurve::m_drawItself(QPainter *painter, widGraph *ptr_graph)
{
    painter->save();
    // Get appropriate axes
        auto [ptr_x, ptr_y] = m_getAppropriateAxes(ptr_graph);
    // Draw curve
        auto [curveColor, curveWidth, curveStyleIndex, curveEnabled] = m_data->m_getStyleOfCurve();
        if (curveEnabled) {
            QPainterPath pathCurve = m_getCurvePainterPath(ptr_x, ptr_y);
            painter->setPen(QPen(curveColor, curveWidth, dataGraphObject::getPenStyleFromIndex(curveStyleIndex)));
            painter->drawPath(pathCurve);
        }
    // Draw points
        auto [pointsColor, pointsWidth, pointsShapeSize, pointsStyleIndex, pointsEnabled] = m_data->m_getStyleOfPoints();
        if (pointsEnabled) {
            QPainterPath pathPoints = m_getPointsPainterPath(ptr_x, ptr_y,
                                      dataGraphObject::getShapeStyleFromIndex(pointsStyleIndex), pointsShapeSize);
            painter->setPen(QPen(pointsColor, pointsWidth, Qt::SolidLine));
            painter->drawPath(pathPoints);
        }
    // Draw area

    painter->restore();
}

QPainterPath graphCurve::m_getCurvePainterPath(widGraphAxis* ptr_x, widGraphAxis* ptr_y)
{
    QPainterPath pathCurve;
    // Move to the first point
        pathCurve.moveTo(ptr_x->m_getDrawAreaPositionFromValue(*s_dataX->begin()),
                    ptr_y->m_getDrawAreaPositionFromValue(*s_dataY->begin()));
    // All points
        for (auto itX = s_dataX->begin(), itY = s_dataY->begin();
            itX < s_dataX->end() || itY < s_dataY->end();
             itX++, itY++) {
            pathCurve.lineTo(ptr_x->m_getDrawAreaPositionFromValue(*itX),
                        ptr_y->m_getDrawAreaPositionFromValue(*itY));
        }
    return pathCurve;
}

QPainterPath graphCurve::m_getPointsPainterPath(widGraphAxis *ptr_x, widGraphAxis *ptr_y,
                                                pointsShapes style, double shapeSize)
{
    QPainterPath pathPoints;
    // Move to the first point
        pathPoints.moveTo(ptr_x->m_getDrawAreaPositionFromValue(*s_dataX->begin()),
                    ptr_y->m_getDrawAreaPositionFromValue(*s_dataY->begin()));
    // All points
        for (auto itX = s_dataX->begin(), itY = s_dataY->begin();
            itX < s_dataX->end() || itY < s_dataY->end();
             itX++, itY++) {
            QPointF point = QPointF(ptr_x->m_getDrawAreaPositionFromValue(*itX),
                                    ptr_y->m_getDrawAreaPositionFromValue(*itY));
            QPainterPath pathPoint = graphObjects::m_createPoint(point, shapeSize, style);
            pathPoints.addPath(pathPoint);
        }
        return pathPoints;
}
double graphCurve::m_getMinX() {
    auto it = min_element(std::begin(*s_dataX), std::end(*s_dataX));
    return *it;
}

double graphCurve::m_getMaxX() {
    auto it = max_element(std::begin(*s_dataX), std::end(*s_dataX));
    return *it;
}

double graphCurve::m_getMinY() {
    s_dataY = w_dataY.lock();
    auto it = min_element(std::begin(*s_dataY), std::end(*s_dataY));
    return *it;
}

double graphCurve::m_getMaxY() {
    s_dataY = w_dataY.lock();
    auto it = max_element(std::begin(*s_dataY), std::end(*s_dataY));
    return *it;
}

graphObjects::graphObjects()
{
}

int graphObjects::m_getPrefferedYAxis()
{
    return m_data->m_getPrefferedYAxis();
}

QPainterPath graphObjects::m_createPoint(QPointF point, double shapeSize, pointsShapes style)
{
    double widthHalf = shapeSize/2;
    QPainterPath path;

    switch (style) {
        case pointsShapes::NONE:

        break;
        case pointsShapes::POINT:
            path.moveTo(point + QPointF(-widthHalf, -widthHalf));
            path.addEllipse(point, widthHalf, widthHalf);
        break;
        case pointsShapes::CROSS:
            path.moveTo(point + QPointF(-widthHalf, -widthHalf));
            path.lineTo(point + QPointF(widthHalf, widthHalf));
            path.moveTo(point + QPointF(-widthHalf, widthHalf));
            path.lineTo(point + QPointF(widthHalf, -widthHalf));
        default:
        break;
        case pointsShapes::SQUARE:
            path.moveTo(point + QPointF(-widthHalf, -widthHalf));
            path.lineTo(point + QPointF(widthHalf, -widthHalf));
            path.lineTo(point + QPointF(widthHalf, widthHalf));
            path.lineTo(point + QPointF(-widthHalf, widthHalf));
            path.lineTo(point + QPointF(-widthHalf, -widthHalf));
        break;
        case pointsShapes::CIRCLE:
            path.moveTo(point + QPointF(-widthHalf, -widthHalf));
            path.addEllipse(point, widthHalf, widthHalf);
        break;
        case pointsShapes::TRIANGLE:
            path.moveTo(point + QPointF(-0.866*widthHalf, 0.5*widthHalf));
            path.lineTo(point + QPointF(0.866*widthHalf, 0.5*widthHalf));
            path.lineTo(point + QPointF(0, -widthHalf));
            path.lineTo(point + QPointF(-0.866*widthHalf, 0.5*widthHalf));
        break;
    }


    return path;
}

std::shared_ptr<graphObjects> graphObjects::m_createGraphObject(int /*type*/)
{
std::shared_ptr<graphObjects> ptr_object = nullptr;
  /*  switch (type) {
    case CURVE:
        ptr_object = std::make_shared<graphCurve>();
        break;
    case YVALUE:
        ptr_object = std::make_shared<graphYValue>();
        break;
    }*/
    return ptr_object;
}

std::tuple<widGraphAxis *, widGraphAxis *> graphObjects::m_getAppropriateAxes(widGraph *ptr_graph)
{
    widGraphAxis* ptr_x = ptr_graph->m_getXAxis();
    widGraphAxis* ptr_y;
    if (m_getPrefferedYAxis() == 0)
        ptr_y = ptr_graph->m_getY1Axis();
    else
        ptr_y = ptr_graph->m_getY2Axis();
    return {ptr_x, ptr_y};
}

graphYValue::graphYValue(std::shared_ptr<double> ptr_dataY):
    w_dataY(ptr_dataY)
{
    m_data = std::make_shared<dataGraphObject>(true, false);

    s_dataY = w_dataY.lock();

    qDebug() << "Curve 3";
}

void graphYValue::m_drawItself(QPainter *painter, widGraph *ptr_graph)
{
    painter->save();
    // Get appropriate axes
        auto [ptr_x, ptr_y] = m_getAppropriateAxes(ptr_graph);
    // Draw curve
        auto [curveColor, curveWidth, curveStyleIndex, curveEnabled] = m_data->m_getStyleOfCurve();
        if (curveEnabled) {
            QPainterPath pathCurve = m_getCurvePainterPath(ptr_x, ptr_y);
            painter->setPen(QPen(curveColor, curveWidth, dataGraphObject::getPenStyleFromIndex(curveStyleIndex)));
            painter->drawPath(pathCurve);
        }
    painter->restore();
}

QPainterPath graphYValue::m_getCurvePainterPath(widGraphAxis *ptr_x, widGraphAxis *ptr_y)
{
    QPainterPath pathCurve;
    // Move to the first point
        double minX = ptr_x->m_getData().lock()->m_min;
        double maxX = ptr_x->m_getData().lock()->m_max;
        pathCurve.moveTo(ptr_x->m_getDrawAreaPositionFromValue(minX),
                    ptr_y->m_getDrawAreaPositionFromValue(*s_dataY));
    // All points
        pathCurve.lineTo(ptr_x->m_getDrawAreaPositionFromValue(maxX),
                    ptr_y->m_getDrawAreaPositionFromValue(*s_dataY));
    return pathCurve;
}

dataGraphObject::dataGraphObject(bool hasCurve, bool hasPoints):
    m_hasCurve(hasCurve), m_hasPoints(hasPoints)
{

}

dataGraphObject::dataGraphObject(std::ifstream &instream):
    m_hasCurve(true), m_hasPoints(true)
{
    // Axis
        m_prefferedYAxis = readInt(instream);
    // Name
        m_name = readString(instream);
    // Color
        m_curveR = readInt(instream);
        m_curveG = readInt(instream);
        m_curveB = readInt(instream);
        m_curveA = readInt(instream);
}

void dataGraphObject::m_saveToFile(std::ofstream &outstream)
{
    // Axis
        writeInt(outstream, m_prefferedYAxis);
    // Name
        writeString(outstream, m_name);
    // Color
        writeInt(outstream, m_curveR);
        writeInt(outstream, m_curveG);
        writeInt(outstream, m_curveB);
        writeInt(outstream, m_curveA);

}

void dataGraphObject::m_setCurveColor(const QColor &color)
{
    m_curveR = color.red();
    m_curveG = color.green();
    m_curveB = color.blue();
    m_curveA = color.alpha();
}
/*
Qt::PenStyle dataGraphObject::m_getCurveStyle() const
{
    return getPenStyleFromIndex(m_curveStyleIndex);
}*/

void dataGraphObject::m_setStyleOfCurve(QColor color, int width, int styleIndex, bool show)
{
    m_setCurveColor(color);
    m_setCurveWidth(width);
    m_setCurveStyleIndex(styleIndex);
    m_setShowCurve(show);
}

void dataGraphObject::m_setPointsColor(const QColor &color)
{
    m_pointsR = color.red();
    m_pointsG = color.green();
    m_pointsB = color.blue();
    m_pointsA = color.alpha();
}

void dataGraphObject::m_setStyleOfPoints(QColor color, int penWidth, double shapeSize, int styleIndex, bool show)
{
    m_setPointsColor(color);
    m_setPointsWidth(penWidth);
    m_setPointsShapeSize(shapeSize);
    m_setPointsStyleIndex(styleIndex);
    m_setShowPoints(show);
}

std::tuple<QColor, int, int, int, bool> dataGraphObject::m_getStyleOfPoints()
{
    return {m_getPointsColor(), m_pointsWidth, m_pointsShapeSize, m_pointsStyleIndex, m_showPoints};
}

std::tuple<QColor, int, int, bool> dataGraphObject::m_getStyleOfCurve()
{
    return {m_getCurveColor(), m_curveWidth, m_curveStyleIndex, m_showCurve};
}

Qt::PenStyle dataGraphObject::getPenStyleFromIndex(int index)
{
    switch (index) {
    case 0:
        return Qt::NoPen;
        break;
    case 1:
        return Qt::SolidLine;
        break;
    case 2:
        return Qt::DashLine;
        break;
    case 3:
        return Qt::DotLine;
        break;
    case 4:
        return Qt::DashDotLine;
        break;
    case 5:
        return Qt::DashDotDotLine;
        break;
    default:
        return Qt::SolidLine;
        break;
    }
}

pointsShapes dataGraphObject::getShapeStyleFromIndex(int index)
{
    switch (index) {
    case 0:
        return pointsShapes::NONE;
        break;
    case 1:
        return pointsShapes::POINT;
        break;
    case 2:
        return pointsShapes::CROSS;
        break;
    case 3:
        return pointsShapes::SQUARE;
        break;
    case 4:
        return pointsShapes::CIRCLE;
        break;
    case 5:
        return pointsShapes::TRIANGLE;
        break;
    default:
        return pointsShapes::CROSS;
        break;
    }
}
