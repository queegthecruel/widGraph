#ifndef OBJECTSGRAPH_H
#define OBJECTSGRAPH_H

#include "graph_global.h"
#include <QPainter>
#include <QPainterPath>
#include <QDebug>

// Graph objects
class widGraph;
class widGraphAxis;

class dataGraphObject
{
public:
    dataGraphObject();
    dataGraphObject(std::ifstream &instream);
    void m_saveToFile(std::ofstream &outstream);
    inline QColor m_getColor()
        {return QColor(m_R, m_G, m_B, m_A);}
    void m_setColor(const QColor &color)
    {m_R = color.red();
     m_G = color.green();
     m_B = color.blue();
     m_A = color.alpha();}
    inline const std::string &m_getName()
        {return m_name;}
    inline void m_setName(const std::string name)
        {m_name = name;}
    inline int m_getPrefferedYAxis()
        {return m_prefferedYAxis;}
    inline void m_setPrefferedAxis(int axisIndex)
        {m_prefferedYAxis = axisIndex;}
protected:
    int m_prefferedYAxis = 0;
    std::string m_name;
    int m_R = 0, m_G = 0, m_B = 0, m_A = 0;
};


class graphObjects
{
public:
    graphObjects();
    virtual void m_drawItself(QPainter *painter, widGraph *ptr_graph) = 0;
    virtual double m_getMinX() {return 0;}
    virtual double m_getMaxX() {return 0;}
    virtual double m_getMinY() {return 0;}
    virtual double m_getMaxY() {return 0;}
    void m_setData(std::shared_ptr<dataGraphObject> data)
        {m_data = data;};
    int m_getPrefferedYAxis();
    inline std::weak_ptr<dataGraphObject> m_getData()
        {return m_data;}
    static QPainterPath m_createPoint(QPointF point = QPoint(0,0), double width = 10);
    static std::shared_ptr<graphObjects> m_createGraphObject(int type);
protected:
    std::tuple<widGraphAxis *, widGraphAxis *> m_getAppropriateAxes(widGraph *ptr_graph);
protected:
    enum type {CURVE, YVALUE};
    std::shared_ptr<dataGraphObject> m_data;
};

class WIDGRAPH_EXPORT graphCurve: public graphObjects
{
public:
    graphCurve(std::shared_ptr<std::vector<double>> ptr_dataY);
    graphCurve(std::shared_ptr<std::vector<double>> ptr_dataX,
               std::shared_ptr<std::vector<double>> ptr_dataY);
    ~graphCurve() = default;
    virtual void m_drawItself(QPainter *painter, widGraph *ptr_graph) override;
    virtual double m_getMinX() override;
    virtual double m_getMaxX() override;
    virtual double m_getMinY() override;
    virtual double m_getMaxY() override;
private:
    QPainterPath m_getCurvePainterPath(widGraphAxis* ptr_x, widGraphAxis* ptr_y);
    QPainterPath m_getPointsPainterPath(widGraphAxis* ptr_x, widGraphAxis* ptr_y);
protected:
    std::weak_ptr<std::vector<double>> w_dataX, w_dataY;
    std::shared_ptr<std::vector<double>> s_dataX, s_dataY;
};

class WIDGRAPH_EXPORT graphYValue: public graphObjects
{
public:
    graphYValue(std::shared_ptr<double> ptr_dataY);
    ~graphYValue() = default;
    virtual void m_drawItself(QPainter *painter, widGraph *ptr_graph) override;
    virtual double m_getMinY() override
        {return *s_dataY;}
    virtual double m_getMaxY() override
        {return *s_dataY;}
private:
    QPainterPath m_getCurvePainterPath(widGraphAxis* ptr_x, widGraphAxis* ptr_y);
protected:
    std::weak_ptr<double> w_dataY;
    std::shared_ptr<double> s_dataY;
};


#endif // OBJECTSGRAPH_H
