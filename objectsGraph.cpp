#include "objectsGraph.h"
#include "dataGraph.h"
#include "widGraph.h"

graphCurve::graphCurve(std::shared_ptr<std::vector<double> > ptr_dataX, std::shared_ptr<std::vector<double> > ptr_dataY):
    w_dataX(ptr_dataX), w_dataY(ptr_dataY)
{
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
    painter->setPen(QPen(m_data->m_getColor(), 3));
    // Get appropriate axes
        auto [ptr_x, ptr_y] = m_getAppropriateAxes(ptr_graph);
    // Get apinter paths
        QPainterPath pathCurve = m_getCurvePainterPath(ptr_x, ptr_y);
        QPainterPath pathPoints = m_getPointsPainterPath(ptr_x, ptr_y);
    // Draw
        painter->drawPath(pathCurve);
        painter->drawPath(pathPoints);
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

QPainterPath graphCurve::m_getPointsPainterPath(widGraphAxis *ptr_x, widGraphAxis *ptr_y)
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
            QPainterPath pathPoint = graphObjects::m_createPoint(point);
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
    m_data = std::make_shared<dataGraphObject>();
}

int graphObjects::m_getPrefferedYAxis()
{
    return m_data->m_getPrefferedYAxis();
}

QPainterPath graphObjects::m_createPoint(QPointF point, double width)
{
    double widthHalf = width/2;
    QPainterPath path;
    path.moveTo(point + QPointF(-widthHalf, -widthHalf));
    path.lineTo(point + QPointF(widthHalf, widthHalf));
    path.moveTo(point + QPointF(-widthHalf, widthHalf));
    path.lineTo(point + QPointF(widthHalf, -widthHalf));

    return path;
}

std::shared_ptr<graphObjects> graphObjects::m_createGraphObject(int type)
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
    s_dataY = w_dataY.lock();

    qDebug() << "Curve 3";
}

void graphYValue::m_drawItself(QPainter *painter, widGraph *ptr_graph)
{
    painter->save();
    painter->setPen(QPen(m_data->m_getColor(), 3));
    // Get appropriate axes
        auto [ptr_x, ptr_y] = m_getAppropriateAxes(ptr_graph);
    // Get painter paths
        QPainterPath pathCurve = m_getCurvePainterPath(ptr_x, ptr_y);
    // Draw
        painter->drawPath(pathCurve);
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

dataGraphObject::dataGraphObject()
{

}

dataGraphObject::dataGraphObject(std::ifstream &instream)
{
    // Axis
        m_prefferedYAxis = readInt(instream);
    // Name
        m_name = readString(instream);
    // Color
        m_R = readInt(instream);
        m_G = readInt(instream);
        m_B = readInt(instream);
        m_A = readInt(instream);
}

void dataGraphObject::m_saveToFile(std::ofstream &outstream)
{
    // Axis
        writeInt(outstream, m_prefferedYAxis);
    // Name
        writeString(outstream, m_name);
    // Color
        writeInt(outstream, m_R);
        writeInt(outstream, m_G);
        writeInt(outstream, m_B);
        writeInt(outstream, m_A);

}
