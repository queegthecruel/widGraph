#ifndef OBJECTSGRAPH_H
#define OBJECTSGRAPH_H

#include <QPainter>
#include <QPainterPath>
#include <QDebug>

// Graph objects
class widGraph;
class dataGraphObject;
class graphObjects
{
public:
    graphObjects();
    virtual void m_drawItself(QPainter *painter, widGraph *ptr_graph) = 0;
    virtual double m_getMinX() {return 0;}
    virtual double m_getMaxX() {return 0;}
    virtual double m_getMinY() {return 0;}
    virtual double m_getMaxY() {return 0;}
    int m_getPrefferedYAxis();
    inline std::weak_ptr<dataGraphObject> m_getData()
        {return m_data;}
protected:
    std::shared_ptr<dataGraphObject> m_data;
};

class graphCurve: public graphObjects
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
protected:
    std::weak_ptr<std::vector<double>> w_dataX, w_dataY;
    std::shared_ptr<std::vector<double>> s_dataX, s_dataY;
};


#endif // OBJECTSGRAPH_H
