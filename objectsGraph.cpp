#include "objectsGraph.h"
#include "dataGraph.h"
#include "widGraph.h"

graphCurve::graphCurve(std::string name,
                       std::shared_ptr<std::vector<double> > ptr_dataX,
                       std::shared_ptr<std::vector<double> > ptr_dataY):
    graphObject(true, true, true, false, true, false),
    w_dataX(ptr_dataX), w_dataY(ptr_dataY)
{
    m_data->m_setName(name);
    QIcon icon(":/images/object_curve.png");
    m_data->m_setIcon(icon);

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
}

graphCurve::graphCurve(const graphCurve &oldGraphObject):
    graphObject(oldGraphObject)
{
    w_dataX = oldGraphObject.w_dataX;
    w_dataY = oldGraphObject.w_dataY;
    s_dataX = std::make_shared<std::vector<double>>(*oldGraphObject.s_dataX);
    s_dataY = std::make_shared<std::vector<double>>(*oldGraphObject.s_dataY);
}

graphCurve::graphCurve(std::string name,
                       std::shared_ptr<std::vector<double> > ptr_dataY):
    graphObject(true, true, true, false, true, false),
    w_dataY(ptr_dataY)
{
    m_data->m_setName(name);
    QIcon icon(":/images/object_curve.png");
    m_data->m_setIcon(icon);

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
            painter->setPen(QPen(curveColor, curveWidth,
                                 dataGraphObject::getPenStyleFromIndex(curveStyleIndex)));
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
        auto [areaColor, areaStyleIndex, areaEnabled] = m_data->m_getStyleOfArea();
        if (areaEnabled) {
            QPainterPath pathArea = m_getAreaPainterPath(ptr_x, ptr_y);
            painter->setPen(QPen(Qt::transparent));
            painter->setBrush(QBrush(areaColor, dataGraphObject::getAreaStyleFromIndex(areaStyleIndex)));
            painter->drawPath(pathArea);
        }
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
            QPainterPath pathPoint = graphObject::m_createPoint(point, shapeSize, style);
            pathPoints.addPath(pathPoint);
        }
        return pathPoints;
}

QPainterPath graphCurve::m_getAreaPainterPath(widGraphAxis *ptr_x, widGraphAxis *ptr_y)
{
    QPainterPath pathArea;
    // Pre points
        auto point1 = QPointF(ptr_x->m_getDrawAreaPositionFromValue(*s_dataX->begin()),
                              ptr_y->m_getDrawAreaPositionFromValue(0));
        pathArea.moveTo(point1);
        auto point2 = QPointF(ptr_x->m_getDrawAreaPositionFromValue(*s_dataX->begin()),
                              ptr_y->m_getDrawAreaPositionFromValue(*s_dataY->begin()));
        pathArea.lineTo(point2);
    // All points
        pathArea.addPath(m_getCurvePainterPath(ptr_x, ptr_y));
        auto point3 = QPointF(ptr_x->m_getDrawAreaPositionFromValue(*(s_dataX->end() - 1)),
                              ptr_y->m_getDrawAreaPositionFromValue(0));
    // After points
        pathArea.lineTo(point3);
        pathArea.lineTo(point1);
    return pathArea;
}
double graphCurve::m_getMinX() {
    auto it = std::min_element(std::begin(*s_dataX), std::end(*s_dataX));
    return *it;
}

double graphCurve::m_getMaxX() {
    auto it = std::max_element(std::begin(*s_dataX), std::end(*s_dataX));
    return *it;
}

double graphCurve::m_getMinY() {
    s_dataY = w_dataY.lock();
    auto it = std::min_element(std::begin(*s_dataY), std::end(*s_dataY));
    return *it;
}

double graphCurve::m_getMaxY() {
    s_dataY = w_dataY.lock();
    auto it = std::max_element(std::begin(*s_dataY), std::end(*s_dataY));
    return *it;
}

double graphCurve::m_getAvgY()
{
    s_dataY = w_dataY.lock();
    double sum = std::accumulate(std::begin(*s_dataY), std::end(*s_dataY), 0.0);
    double m =  sum / s_dataY->size();
    return m;
}

int graphCurve::m_getNValues()
{
    s_dataY = w_dataY.lock();
    return s_dataY->size();
}

graphObject::graphObject(
    bool hasCurve, bool hasPoints, bool hasArea, bool hasColumns,
    bool hasLegend, bool hasOrientation)
{
    m_data = std::make_shared<dataGraphObject>(
            hasCurve, hasPoints, hasArea, hasColumns,
            hasLegend, hasOrientation);
}

graphObject::graphObject(const graphObject &oldGraphObject)
{
    m_data = std::make_shared<dataGraphObject>(*oldGraphObject.m_data);
}

enum yAxisPosition graphObject::m_getPrefferedYAxis()
{
    return m_data->m_getPrefferedYAxis();
}

QPainterPath graphObject::m_createPoint(QPointF point, double shapeSize, pointsShapes style)
{
    double widthHalf = shapeSize/2;
    QPainterPath path;
    double k = 0.6;
    switch (style) {
        case pointsShapes::NONE:

        break;
        case pointsShapes::POINT:
         //   path.moveTo(point + QPointF(-widthHalf/2, -widthHalf/2));
            path.moveTo(point + QPointF(-widthHalf*k, -widthHalf*k));
            path.lineTo(point + QPointF(widthHalf*k, widthHalf*k));
            path.moveTo(point + QPointF(-widthHalf*k, widthHalf*k));
            path.lineTo(point + QPointF(widthHalf*k, -widthHalf*k));
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
        case pointsShapes::TRIANGLE_REV:
            path.moveTo(point + QPointF(-0.866*widthHalf, -0.5*widthHalf));
            path.lineTo(point + QPointF(0.866*widthHalf, -0.5*widthHalf));
            path.lineTo(point + QPointF(0, widthHalf));
            path.lineTo(point + QPointF(-0.866*widthHalf, -0.5*widthHalf));
        break;
    }


    return path;
}

std::shared_ptr<graphObject> graphObject::m_createGraphObject(int /*type*/)
{
    std::shared_ptr<graphObject> ptr_object = nullptr;
    return ptr_object;
}

std::tuple<widGraphAxis *, widGraphAxis *> graphObject::m_getAppropriateAxes(widGraph *ptr_graph)
{
    widGraphAxis* ptr_x = ptr_graph->m_getXAxis();
    widGraphAxis* ptr_y;
    enum yAxisPosition yAxis = m_getPrefferedYAxis();
    switch (yAxis) {
    case yAxisPosition::LEFT:
    default:
        ptr_y = ptr_graph->m_getY1Axis();
        break;
    case yAxisPosition::RIGHT:
        ptr_y = ptr_graph->m_getY2Axis();
        break;
    }
    return {ptr_x, ptr_y};
}

graphValue::graphValue(std::string name, std::shared_ptr<double> ptr_dataY, orientation orient):
    graphObject(true, false, false, false, true, true),
    w_data(ptr_dataY)
{
    m_data->m_setName(name);
    QIcon icon(":/images/object_value.png");
    m_data->m_setIcon(icon);
    m_data->m_setConstCurveOrientation(orient);

    s_data = w_data.lock();
}

graphValue::graphValue(const graphValue &oldGraphObject):
    graphObject(oldGraphObject)
{
    w_data = oldGraphObject.w_data;
    s_data = std::make_shared<double>(*oldGraphObject.s_data);
//    m_orientation = oldGraphObject.m_orientation;
}

void graphValue::m_drawItself(QPainter *painter, widGraph *ptr_graph)
{
    painter->save();
    // Get appropriate axes
        auto [ptr_x, ptr_y] = m_getAppropriateAxes(ptr_graph);
    // Draw curve
        auto [curveColor, curveWidth, curveStyleIndex, curveEnabled] = m_data->m_getStyleOfCurve();
        if (curveEnabled) {
            QPainterPath pathCurve = m_getCurvePainterPath(ptr_x, ptr_y);
            painter->setPen(QPen(curveColor, curveWidth,
                            dataGraphObject::getPenStyleFromIndex(curveStyleIndex)));
            painter->drawPath(pathCurve);
        }
        painter->restore();
}

double graphValue::m_getMinX()
{
    auto [orient] = m_data->m_getConstCurveOrientation();
    switch (orient) {
        case orientation::VERTICAL:
            return *s_data;
        case orientation::HORIZONTAL:
            return graphObject::m_getMinX();
    }
}

double graphValue::m_getMaxX()
{
    auto [orient] = m_data->m_getConstCurveOrientation();
    switch (orient) {
        case orientation::VERTICAL:
            return *s_data;
        case orientation::HORIZONTAL:
            return graphObject::m_getMinX();
    }
}

double graphValue::m_getMinY()
{
    auto [orient] = m_data->m_getConstCurveOrientation();
    switch (orient) {
        case orientation::VERTICAL:
            return graphObject::m_getMinX();
        case orientation::HORIZONTAL:
            return *s_data;
    }
}

double graphValue::m_getMaxY()
{
    auto [orient] = m_data->m_getConstCurveOrientation();
    switch (orient) {
        case orientation::VERTICAL:
            return graphObject::m_getMinX();
        case orientation::HORIZONTAL:
            return *s_data;
    }
}

double graphValue::m_getAvgY()
{
    auto [orient] = m_data->m_getConstCurveOrientation();
    switch (orient) {
        case orientation::VERTICAL:
            return graphObject::m_getMinX();
        case orientation::HORIZONTAL:
            return *s_data;
    }
}

int graphValue::m_getNValues()
{
    return 1;
}

QPainterPath graphValue::m_getCurvePainterPath(widGraphAxis *ptr_x, widGraphAxis *ptr_y)
{
    QPainterPath pathCurve;
    auto [orient] = m_data->m_getConstCurveOrientation();
    switch (orient) {
        case orientation::VERTICAL:
            {
            // Move to the first point
                double minY = ptr_y->m_getData().lock()->m_min;
                double maxY = ptr_y->m_getData().lock()->m_max;
                pathCurve.moveTo(ptr_x->m_getDrawAreaPositionFromValue(*s_data),
                                 ptr_y->m_getDrawAreaPositionFromValue(minY));
            // All points
                pathCurve.lineTo(ptr_x->m_getDrawAreaPositionFromValue(*s_data),
                                 ptr_y->m_getDrawAreaPositionFromValue(maxY));
            }
            break;
        case orientation::HORIZONTAL:
            {
            // Move to the first point
                double minX = ptr_x->m_getData().lock()->m_min;
                double maxX = ptr_x->m_getData().lock()->m_max;
                pathCurve.moveTo(ptr_x->m_getDrawAreaPositionFromValue(minX),
                            ptr_y->m_getDrawAreaPositionFromValue(*s_data));
            // All points
                pathCurve.lineTo(ptr_x->m_getDrawAreaPositionFromValue(maxX),
                            ptr_y->m_getDrawAreaPositionFromValue(*s_data));
            }
            break;
    }
    return pathCurve;
}

dataGraphObject::dataGraphObject(
    bool hasCurve, bool hasPoints, bool hasArea, bool hasColumns,
    bool hasLegend, bool hasOrientation):
    m_hasCurve(hasCurve), m_hasPoints(hasPoints), m_hasArea(hasArea), m_hasColumns(hasColumns),
    m_hasLegend(hasLegend), m_hasOrientation(hasOrientation)
{
}

dataGraphObject::dataGraphObject(std::ifstream &instream):
    m_hasCurve(true), m_hasPoints(true), m_hasArea(true), m_hasColumns(true), m_hasLegend(true)
{
    // Axis
 //       m_prefferedYAxis = readInt(instream);
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
  //      writeInt(outstream, m_prefferedYAxis);
    // Name
        writeString(outstream, m_name);
    // Color
        writeInt(outstream, m_curveR);
        writeInt(outstream, m_curveG);
        writeInt(outstream, m_curveB);
        writeInt(outstream, m_curveA);

}

void dataGraphObject::m_setName(const std::string name)
{
    m_name = name;
    if (!m_legendOverwrite)
        m_legendText = name;
}

void dataGraphObject::m_setIcon(const QIcon &icon)
{
    m_icon = icon;
}

void dataGraphObject::m_setCurveColor(const QColor &color)
{
    m_curveR = color.red();
    m_curveG = color.green();
    m_curveB = color.blue();
    m_curveA = color.alpha();
}

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

void dataGraphObject::m_setAreaColor(const QColor &color)
{
    m_areaR = color.red();
    m_areaG = color.green();
    m_areaB = color.blue();
    m_areaA = color.alpha();
}

void dataGraphObject::m_setStyleOfPoints(QColor color, int penWidth, double shapeSize, int styleIndex, bool show)
{
    m_setPointsColor(color);
    m_setPointsWidth(penWidth);
    m_setPointsShapeSize(shapeSize);
    m_setPointsStyleIndex(styleIndex);
    m_setShowPoints(show);
}

void dataGraphObject::m_setStyleOfArea(QColor color, int styleIndex, bool show)
{
    m_setAreaColor(color);
    m_setAreaStyleIndex(styleIndex);
    m_setShowArea(show);
}

void dataGraphObject::m_setStyleOfColumn(int columnWidth, bool show)
{
    m_setColumnsWidth(columnWidth);
    m_setShowColumns(show);
}

void dataGraphObject::m_setStyleOfLegend(bool overwrite, const std::string &text, bool show)
{
    m_setLegendOverwrite(overwrite);
    m_setLegendText(text);
    m_setShowLegend(show);
}

void dataGraphObject::m_setOperation(bool toBeDeleted)
{
    m_setToBeDeleted(toBeDeleted);
}

void dataGraphObject::m_setConstCurveOrientation(orientation orient)
{
    m_setOrientation(orient);
}

std::tuple<QColor, int, int, int, bool> dataGraphObject::m_getStyleOfPoints()
{
    return {m_getPointsColor(), m_pointsWidth, m_pointsShapeSize, m_pointsStyleIndex, m_showPoints};
}

std::tuple<QColor, int, int, bool> dataGraphObject::m_getStyleOfCurve()
{
    return {m_getCurveColor(), m_curveWidth, m_curveStyleIndex, m_showCurve};
}

std::tuple<QColor, int, bool> dataGraphObject::m_getStyleOfArea()
{
    return {m_getAreaColor(), m_areaStyleIndex, m_showArea};
}

std::tuple<int, bool> dataGraphObject::m_getStyleOfColumns()
{
    return {m_columnWidth, m_showColumn};
}

std::tuple<bool, const std::string &, bool> dataGraphObject::m_getStyleOfLegend()
{
    return {m_legendOverwrite, m_legendText, m_showLegend};
}

std::tuple<bool> dataGraphObject::m_getOperation()
{
    return {m_toBeDeleted};
}

std::tuple<orientation> dataGraphObject::m_getConstCurveOrientation()
{
    return {m_orientation};
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

Qt::BrushStyle dataGraphObject::getAreaStyleFromIndex(int index)
{
    switch (index) {
    case 0:
        return Qt::NoBrush;
        break;
    case 1:
        return Qt::SolidPattern;
        break;
    case 2:
        return Qt::Dense1Pattern;
        break;
    case 3:
        return Qt::Dense2Pattern;
        break;
    case 4:
        return Qt::Dense3Pattern;
        break;
    case 5:
        return Qt::Dense4Pattern;
        break;
    case 6:
        return Qt::Dense5Pattern;
        break;
    case 7:
        return Qt::Dense6Pattern;
        break;
    case 8:
        return Qt::Dense7Pattern;
        break;
    case 9:
        return Qt::HorPattern;
        break;
    case 10:
        return Qt::VerPattern;
        break;
    case 11:
        return Qt::CrossPattern;
        break;
    case 12:
        return Qt::BDiagPattern;
        break;
    case 13:
        return Qt::FDiagPattern;
        break;
    case 14:
        return Qt::DiagCrossPattern;
        break;
    default:
        return Qt::SolidPattern;
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
    case 6:
        return pointsShapes::TRIANGLE_REV;
        break;
    default:
        return pointsShapes::CROSS;
        break;
    }
}

graphColumn::graphColumn(std::string name, std::shared_ptr<std::vector<double> > ptr_dataY):
    graphObject(true, false, true, true, true, false),
    w_dataY(ptr_dataY)
{
    m_data->m_setName(name);
    QIcon icon(":/images/object_columns.png");
    m_data->m_setIcon(icon);
    m_data->m_setStyleOfColumn(30, true);

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
}

graphColumn::graphColumn(const graphColumn &oldGraphObject):
    graphObject(oldGraphObject)
{
    w_dataX = oldGraphObject.w_dataX;
    w_dataY = oldGraphObject.w_dataY;
    s_dataX = std::make_shared<std::vector<double>>(*oldGraphObject.s_dataX);
    s_dataY = std::make_shared<std::vector<double>>(*oldGraphObject.s_dataY);
}
void graphColumn::m_drawItself(QPainter *painter, widGraph *ptr_graph)
{
    painter->save();
    // Get appropriate axes
        auto [ptr_x, ptr_y] = m_getAppropriateAxes(ptr_graph);
        auto [columnsWidth, columnsEnabled] = m_data->m_getStyleOfColumns();
        if (columnsEnabled && columnsWidth > 0) {
            // Draw columns area
            auto [areaColor, areaStyleIndex, areaEnabled] = m_data->m_getStyleOfArea();
            if (areaEnabled) {
                QPainterPath pathColumns = m_getColumnPainterPath(ptr_x, ptr_y, columnsWidth);
                pathColumns.setFillRule(Qt::WindingFill);
                painter->fillPath(pathColumns, QBrush(areaColor, dataGraphObject::getAreaStyleFromIndex(areaStyleIndex)));
            }
            // Draw column border
            auto [curveColor, curveWidth, curveStyleIndex, curveEnabled] = m_data->m_getStyleOfCurve();
            if (curveEnabled) {
                QPainterPath pathCurve = m_getColumnBorderPainterPath(ptr_x, ptr_y, columnsWidth);
                painter->setPen(QPen(curveColor, curveWidth,
                                     dataGraphObject::getPenStyleFromIndex(curveStyleIndex)));
                painter->drawPath(pathCurve);
            }
        }
        painter->restore();
}

double graphColumn::m_getMinX()
{
    auto min_it = std::min_element(std::begin(*s_dataX), std::end(*s_dataX));
    auto max_it = std::max_element(std::begin(*s_dataX), std::end(*s_dataX));
    return *min_it -  0.05 *abs(*max_it - *min_it);;
}

double graphColumn::m_getMaxX()
{
    auto min_it = std::min_element(std::begin(*s_dataX), std::end(*s_dataX));
    auto max_it = std::max_element(std::begin(*s_dataX), std::end(*s_dataX));
    return *max_it + 0.05 *abs(*max_it - *min_it);
}

double graphColumn::m_getMinY()
{
    s_dataY = w_dataY.lock();
    auto it = std::min_element(std::begin(*s_dataY), std::end(*s_dataY));
    return *it;
}

double graphColumn::m_getMaxY()
{
    s_dataY = w_dataY.lock();
    auto it = std::max_element(std::begin(*s_dataY), std::end(*s_dataY));
    return *it;
}

double graphColumn::m_getAvgY()
{
    s_dataY = w_dataY.lock();
    double sum = std::accumulate(std::begin(*s_dataY), std::end(*s_dataY), 0.0);
    double m =  sum / s_dataY->size();
    return m;
}

int graphColumn::m_getNValues()
{
    s_dataY = w_dataY.lock();
    return s_dataY->size();
}

QPainterPath graphColumn::m_getColumnPainterPath(widGraphAxis *ptr_x, widGraphAxis *ptr_y, double columnWidth)
{
    double widthHalf = columnWidth/2;
    QPainterPath pathColumn;
    // All points
        for (auto itX = s_dataX->begin(), itY = s_dataY->begin();
            itX < s_dataX->end() || itY < s_dataY->end();
             itX++, itY++) {
            QRect rect(QPoint(ptr_x->m_getDrawAreaPositionFromValue(*itX) - widthHalf,
                               ptr_y->m_getDrawAreaPositionFromValue(0)),
                       QPoint(ptr_x->m_getDrawAreaPositionFromValue(*itX) + widthHalf,
                               ptr_y->m_getDrawAreaPositionFromValue(*itY)));
            pathColumn.addRect(rect);
        }
        return pathColumn;
}

QPainterPath graphColumn::m_getColumnBorderPainterPath(widGraphAxis *ptr_x, widGraphAxis *ptr_y, double columnWidth)
{
    double widthHalf = columnWidth/2;
 //   double widthHalf = 4;
    QPainterPath pathColumnBorder;
    // All points
        for (auto itX = s_dataX->begin(), itY = s_dataY->begin();
            itX < s_dataX->end() || itY < s_dataY->end();
             itX++, itY++) {
            QRect rect(QPoint(ptr_x->m_getDrawAreaPositionFromValue(*itX) - widthHalf,
                               ptr_y->m_getDrawAreaPositionFromValue(0)),
                       QPoint(ptr_x->m_getDrawAreaPositionFromValue(*itX) + widthHalf,
                               ptr_y->m_getDrawAreaPositionFromValue(0 + *itY)));
            pathColumnBorder.addRect(rect);
        }
        return pathColumnBorder;
}
