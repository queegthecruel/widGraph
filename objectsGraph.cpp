#include "objectsGraph.h"
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
    painter->setPen(QPen(QColor(m_data->m_R, m_data->m_G, m_data->m_B), 3));

    widGraphAxis* ptr_x = ptr_graph->m_getXAxis();
    widGraphAxis* ptr_y;
    if (m_getPrefferedYAxis() == 0)
        ptr_y = ptr_graph->m_getY1Axis();
    else
        ptr_y = ptr_graph->m_getY2Axis();

    QPainterPath path;

    path.moveTo(ptr_x->m_getDrawAreaPositionFromValue(*s_dataX->begin()),
                ptr_y->m_getDrawAreaPositionFromValue(*s_dataY->begin()));

    for (auto itX = s_dataX->begin(), itY = s_dataY->begin();
        itX < s_dataX->end() || itY < s_dataY->end();
         itX++, itY++) {
        path.lineTo(ptr_x->m_getDrawAreaPositionFromValue(*itX),
                    ptr_y->m_getDrawAreaPositionFromValue(*itY));
    }
    painter->drawPath(path);
    painter->restore();
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
    return m_data->m_prefferedYAxis;
}
