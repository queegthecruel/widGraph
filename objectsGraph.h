#ifndef OBJECTSGRAPH_H
#define OBJECTSGRAPH_H

#include "graph_global.h"
#include <QPainter>
#include <QPainterPath>
#include <QDebug>

// Graph objects
class widGraph;
class widGraphAxis;

enum class pointsShapes {NONE, POINT, CROSS, SQUARE, CIRCLE, TRIANGLE, TRIANGLE_REV};
enum class yAxisPosition {LEFT, RIGHT};
enum class orientation {HORIZONTAL, VERTICAL};
class dataGraphObject
{
public:
    dataGraphObject(bool hasCurve, bool hasPoints, bool hasArea, bool hasColumns, bool hasLegend, bool hasOrientation);
    dataGraphObject(const dataGraphObject & oldData) = default;
    dataGraphObject(std::ifstream &instream);
    ~dataGraphObject() = default;
    dataGraphObject& operator=(const dataGraphObject & oldData) = default;
    void m_saveToFile(std::ofstream &outstream);
    inline const std::string &m_getName()
        {return m_name;}
    void m_setName(const std::string name);
    inline enum yAxisPosition m_getPrefferedYAxis()
        {return m_prefferedYAxis;}
    inline void m_setYAxis(enum yAxisPosition axis)
        {m_prefferedYAxis = axis;}
    inline bool m_getHasCurve()
        {return m_hasCurve;}
    inline bool m_getHasPoints()
        {return m_hasPoints;}
    inline bool m_getHasArea()
        {return m_hasArea;}
    inline bool m_getHasColumn()
        {return m_hasColumns;}
    inline bool m_getHasLegend()
        {return m_hasLegend;}
    inline bool m_getHasOrientation()
        {return m_hasOrientation;}
    // Curve
        void m_setStyleOfCurve(QColor color, int width, int styleIndex, bool show = true);
        std::tuple<QColor, int, int, bool> m_getStyleOfCurve();
        static Qt::PenStyle getPenStyleFromIndex(int index);
    // Points
        void m_setStyleOfPoints(QColor color, int penWidth, double shapeSize, int styleIndex, bool show = true);
        std::tuple<QColor, int, int, int, bool> m_getStyleOfPoints();
        static pointsShapes getShapeStyleFromIndex(int index);
    // Area
        void m_setStyleOfArea(QColor color, int styleIndex, bool show = true);
        std::tuple<QColor, int, bool> m_getStyleOfArea();
        static Qt::BrushStyle getAreaStyleFromIndex(int index);
    // Column
        void m_setStyleOfColumn(int columnWidth, bool show = true);
        std::tuple<int, bool> m_getStyleOfColumns();
    // Legend
        void m_setStyleOfLegend(bool overwrite, const std::string &text, bool show = true);
        std::tuple<bool, const std::string &, bool> m_getStyleOfLegend();
    // Operation
        void m_setOperation(bool toBeDeleted);
        std::tuple<bool> m_getOperation();
    // Horizontal/vertical const curve
        void m_setConstCurveOrientation(enum orientation orient);
        std::tuple<enum orientation> m_getConstCurveOrientation();
protected:
    // Curve
        inline QColor m_getCurveColor()
            {return QColor(m_curveR, m_curveG, m_curveB, m_curveA);}
        void m_setCurveColor(const QColor &color);
        inline void m_setCurveWidth(double newWidth)
            {m_curveWidth = newWidth;}
        inline void m_setCurveStyleIndex(int newStyle)
            {m_curveStyleIndex = newStyle;}
        inline void m_setShowCurve(bool show)
            {m_showCurve = show;}
    // Points
        inline QColor m_getPointsColor()
            {return QColor(m_pointsR, m_pointsG, m_pointsB, m_pointsA);}
        void m_setPointsColor(const QColor &color);
        inline void m_setPointsWidth(double newWidth)
            {m_pointsWidth = newWidth;}
        inline void m_setPointsShapeSize(double newShapeSize)
            {m_pointsShapeSize = newShapeSize;}
        inline void m_setPointsStyleIndex(int newStyle)
            {m_pointsStyleIndex = newStyle;}
        inline void m_setShowPoints(bool show)
            {m_showPoints = show;}
    // Area
        inline QColor m_getAreaColor()
            {return QColor(m_areaR, m_areaG, m_areaB, m_areaA);}
        void m_setAreaColor(const QColor &color);
        inline void m_setAreaStyleIndex(int newStyle)
            {m_areaStyleIndex = newStyle;}
        inline void m_setShowArea(bool show)
            {m_showArea = show;}
    // Column
        inline void m_setColumnsWidth(double newWidth)
            {m_columnWidth = newWidth;}
        inline void m_setShowColumns(bool show)
            {m_showColumn = show;}
    // Legend
        inline void m_setLegendText(const std::string &text)
            {m_legendText = text;}
        inline void m_setLegendOverwrite(bool overwrite)
            {m_legendOverwrite = overwrite;}
        inline void m_setShowLegend(bool show)
            {m_showLegend = show;}
    // Operation
        inline void m_setToBeDeleted(bool toBeDeleted)
            {m_toBeDeleted = toBeDeleted;}
    // Horizontal/vertical const curve
        inline void m_setOrientation(enum orientation orient)
            {m_orientation = orient;}
public:
    bool m_hasCurve, m_hasPoints, m_hasArea, m_hasColumns, m_hasLegend, m_hasOrientation;
protected:
    yAxisPosition m_prefferedYAxis = yAxisPosition::LEFT;
    std::string m_name;
    // Curve
        bool m_showCurve = true;
        int m_curveR = 0, m_curveG = 0, m_curveB = 0, m_curveA = 255;
        double m_curveWidth = 3;
        int m_curveStyleIndex = 1;
    // Points
        bool m_showPoints = true;
        int m_pointsR = 0, m_pointsG = 0, m_pointsB = 0, m_pointsA = 255;
        double m_pointsWidth = 3, m_pointsShapeSize = 10;
        int m_pointsStyleIndex = 2;
    // Area
        bool m_showArea = false;
        int m_areaR = 0, m_areaG = 0, m_areaB = 0, m_areaA = 255;
        int m_areaStyleIndex = 13;
    // Column
        bool m_showColumn = false;
        int m_columnWidth = 30;
    // Legend
        bool m_showLegend = true;
        bool m_legendOverwrite = false;
        std::string m_legendText;
    // Operation
        bool m_toBeDeleted = false;
    // Horizontal/vertical const curve
        enum orientation m_orientation = orientation::HORIZONTAL;
};

class graphObject
{
public:
    graphObject(bool hasCurve, bool hasPoints, bool hasArea, bool hasColumns,
        bool hasLegend, bool hasOrientation);
    graphObject(const graphObject& oldGraphObject);
    virtual void m_drawItself(QPainter *painter, widGraph *ptr_graph) = 0;
    virtual double m_getMinX() {return 0;}
    virtual double m_getMaxX() {return 0;}
    virtual double m_getMinY() {return 0;}
    virtual double m_getMaxY() {return 0;}
    virtual double m_getAvgY() {return 0;}
    virtual int m_getNValues() {return 0;}
    void m_setData(std::shared_ptr<dataGraphObject> data)
        {*m_data = *data;}
    enum yAxisPosition m_getPrefferedYAxis();
    inline std::weak_ptr<dataGraphObject> m_getData()
        {return m_data;}
    static QPainterPath m_createPoint(QPointF point = QPoint(0,0),
        double shapeSize = 10, pointsShapes style = pointsShapes::CROSS);
    static std::shared_ptr<graphObject> m_createGraphObject(int type);
protected:
    std::tuple<widGraphAxis *, widGraphAxis *> m_getAppropriateAxes(widGraph *ptr_graph);
protected:
    enum type {CURVE, YVALUE};
    std::shared_ptr<dataGraphObject> m_data;
};

class WIDGRAPH_EXPORT graphCurve: public graphObject
{
public:
    graphCurve(std::string name,
               std::shared_ptr<std::vector<double>> ptr_dataY);
    graphCurve(std::string name,
               std::shared_ptr<std::vector<double>> ptr_dataX,
               std::shared_ptr<std::vector<double>> ptr_dataY);
    graphCurve(const graphCurve& oldGraphObject);
    virtual void m_drawItself(QPainter *painter, widGraph *ptr_graph) override;
    virtual double m_getMinX() override;
    virtual double m_getMaxX() override;
    virtual double m_getMinY() override;
    virtual double m_getMaxY() override;
    virtual double m_getAvgY() override;
    virtual int m_getNValues() override;
private:
    QPainterPath m_getCurvePainterPath(widGraphAxis* ptr_x, widGraphAxis* ptr_y);
    QPainterPath m_getPointsPainterPath(widGraphAxis* ptr_x, widGraphAxis* ptr_y, pointsShapes style, double shapeSize);
    QPainterPath m_getAreaPainterPath(widGraphAxis* ptr_x, widGraphAxis* ptr_y);
protected:
    std::weak_ptr<std::vector<double>> w_dataX, w_dataY;
    std::shared_ptr<std::vector<double>> s_dataX, s_dataY;
};

class WIDGRAPH_EXPORT graphValue: public graphObject
{
public:
    graphValue(std::string name, std::shared_ptr<double> ptr_dataY, enum orientation orient = orientation::HORIZONTAL);
    graphValue(const graphValue& oldGraphObject);
    virtual void m_drawItself(QPainter *painter, widGraph *ptr_graph) override;
    virtual double m_getMinX() override;
    virtual double m_getMaxX() override;
    virtual double m_getMinY() override;
    virtual double m_getMaxY() override;
    virtual double m_getAvgY() override;
    virtual int m_getNValues() override;
private:
    QPainterPath m_getCurvePainterPath(widGraphAxis* ptr_x, widGraphAxis* ptr_y);
protected:
    std::weak_ptr<double> w_data;
    std::shared_ptr<double> s_data;
};

class WIDGRAPH_EXPORT graphColumn: public graphObject
{
public:
    graphColumn(std::string name, std::shared_ptr<std::vector<double>> ptr_dataY);
    graphColumn(const graphColumn& oldGraphObject);
    ~graphColumn() = default;
    virtual void m_drawItself(QPainter *painter, widGraph *ptr_graph) override;
    virtual double m_getMinX() override;
    virtual double m_getMaxX() override;
    virtual double m_getMinY() override;
    virtual double m_getMaxY() override;
    virtual double m_getAvgY() override;
    virtual int m_getNValues() override;
private:
    QPainterPath m_getColumnPainterPath(widGraphAxis* ptr_x, widGraphAxis* ptr_y, double columnWidth);
    QPainterPath m_getColumnBorderPainterPath(widGraphAxis* ptr_x, widGraphAxis* ptr_y, double columnWidth);
protected:
    std::weak_ptr<std::vector<double>> w_dataX, w_dataY;
    std::shared_ptr<std::vector<double>> s_dataX, s_dataY;
};
#endif // OBJECTSGRAPH_H
