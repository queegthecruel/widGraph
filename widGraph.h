#ifndef WIDGRAPH_H
#define WIDGRAPH_H

#include <QWidget>
#include <QGridLayout>
#include "dataGraph.h"
#include "objectsGraph.h"

// Graph widgets
class widGraph;
class painterAntiAl;

class widGraphElement: public QWidget
{
public:
    widGraphElement(widGraph *graph):
        ptr_graph(graph)
    {}
    void paintEvent(QPaintEvent *event) override = 0;
    virtual void m_setDimensions() {}

protected:
    widGraph *ptr_graph;
};
class widGraphDrawArea: public widGraphElement
{
public:
    widGraphDrawArea(widGraph *graph);
    void paintEvent(QPaintEvent *event) override;
private:
    void m_drawHorGrid(painterAntiAl &painter);
    void m_drawVertGrid(painterAntiAl &painter);
};

class widGraphTitle: public widGraphElement
{
public:
    widGraphTitle(widGraph *graph);
    void paintEvent(QPaintEvent *event) override;
    virtual void m_setDimensions() override;
};

class widGraphAxis: public widGraphElement
{
public:
    widGraphAxis(widGraph *graph): widGraphElement(graph)
    {}
    virtual double m_getDrawAreaPositionFromValue(const double &value) = 0;
    virtual void m_setAutoAxis() = 0;
    void paintEvent(QPaintEvent */*event*/) override;
    static double m_supCalculateNiceNumbers(float range, bool round);
    static std::tuple<double, double, double> m_calculateNiceMaxMin(double min, double max);
private:
    virtual double m_getPositionFromValue(const double &value) = 0;
    virtual void m_drawLine(painterAntiAl &painter) {};
    virtual void m_drawTicks(painterAntiAl &painter) {};
    virtual void m_drawNumbers(painterAntiAl &painter) {};
    virtual void m_drawText(painterAntiAl &painter) {};
};

class widGraphXAxis: public widGraphAxis
{
public:
    widGraphXAxis(widGraph *graph);
    virtual double m_getDrawAreaPositionFromValue(const double &value) override;
    virtual void m_setAutoAxis() override;
    virtual void m_setDimensions() override;
private:
    virtual double m_getPositionFromValue(const double &value) override;
    virtual void m_drawLine(painterAntiAl &painter) override;
    virtual void m_drawTicks(painterAntiAl &painter) override;
    virtual void m_drawNumbers(painterAntiAl &painter) override;
    virtual void m_drawText(painterAntiAl &painter) override;

    double m_getTicksEndFromTop();
    double m_getNumbersStartFromTop();
    double m_getNumberEndsFromTop();
    double m_getTextStartFromTop();
    double m_getTextEndsFromTop();
    double m_getEndFromTop();

protected:
    const double m_tickLength = 10;
};

class widGraphY1Axis: public widGraphAxis
{
public:
    widGraphY1Axis(widGraph *graph);
    virtual double m_getDrawAreaPositionFromValue(const double &value) override;
    virtual void m_setAutoAxis() override;
    virtual void m_setDimensions() override;
private:
    virtual double m_getPositionFromValue(const double &value) override;
    virtual void m_drawLine(painterAntiAl &painter) override;
    virtual void m_drawTicks(painterAntiAl &painter) override;
    virtual void m_drawNumbers(painterAntiAl &painter) override;
};

class widGraphY2Axis: public widGraphAxis
{
public:
    widGraphY2Axis(widGraph *graph);
    virtual double m_getDrawAreaPositionFromValue(const double &value) override;
    virtual void m_setAutoAxis() override;
    virtual void m_setDimensions() override;
private:
    virtual double m_getPositionFromValue(const double &value) override;
    virtual void m_drawLine(painterAntiAl &painter) override;
    virtual void m_drawTicks(painterAntiAl &painter) override;
    virtual void m_drawNumbers(painterAntiAl &painter) override;
};

class widGraphLegend: public widGraphElement
{
public:
    widGraphLegend(widGraph *graph);
    void paintEvent(QPaintEvent *event) override;
    virtual void m_setDimensions() override;
private:
    void m_drawTopLine(painterAntiAl &painter);
    void m_drawTexts(painterAntiAl &painter);
};

class widGraph: public QWidget
{
public:
    widGraph();
    std::weak_ptr<dataGraph> m_getData()
        {return m_data;}
    widGraphXAxis* m_getXAxis()
        {return m_widX;}
    widGraphY1Axis* m_getY1Axis()
        {return m_widY1;}
    widGraphY2Axis* m_getY2Axis()
        {return m_widY2;}
    void m_addObject(std::shared_ptr<graphObjects> ptr_object);
    void m_loadValues();
    void m_setCurveStyle(int curveIndex, int R, int G, int B, int axis = 0);
    void m_setCurveName(int curveIndex, const std::string& name);
protected:
    // Data
        std::shared_ptr<dataGraph> m_data;
    // Graph elements
        widGraphDrawArea *m_widArea;
        widGraphTitle *m_widTitle;
        widGraphXAxis *m_widX;
        widGraphY1Axis *m_widY1;
        widGraphY2Axis *m_widY2;
        widGraphLegend *m_widLegend;
};





#endif // WIDGRAPH_H
