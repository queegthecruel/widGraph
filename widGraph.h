#ifndef WIDGRAPH_H
#define WIDGRAPH_H

#include <QWidget>
#include <QGridLayout>
#include "dataGraph.h"
#include "objectsGraph.h"

// Graph widgets
class widGraph;
class painterAntiAl;
class dialogAxis;

class widGraphElement: public QWidget
{
public:
    widGraphElement(widGraph *graph):
        ptr_graph(graph)
    {}
    void paintEvent(QPaintEvent *event) override = 0;
    virtual void m_setDimensions() {}
    virtual dataElement& m_getData() = 0;
protected:
    widGraph *ptr_graph;
};
class widGraphDrawArea: public widGraphElement
{
public:
    widGraphDrawArea(widGraph *graph);
    void paintEvent(QPaintEvent *event) override;
    virtual dataElement& m_getData() override;

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
    virtual dataElement& m_getData() override;
};

class widGraphAxis: public widGraphElement
{
public:
    widGraphAxis(widGraph *graph);
    virtual double m_getDrawAreaPositionFromValue(double value) = 0;
    virtual void m_setAutoAxis() = 0;
    void paintEvent(QPaintEvent */*event*/) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    virtual dataAxis& m_getData() override = 0;
    static double m_supCalculateNiceNumbers(float range, bool round);
    static std::tuple<double, double, double> m_calculateNiceMaxMin(double min, double max);
private:
    virtual double m_getPositionFromValue(double value) = 0;
    virtual void m_drawLine(painterAntiAl &painter) = 0;
    virtual void m_drawTicks(painterAntiAl &painter) = 0;
    virtual void m_drawNumbers(painterAntiAl &painter) = 0;
    virtual void m_drawText(painterAntiAl &painter) = 0;
protected:
    const double m_tickLength = 10,
                            m_spaceTicksToNumbers = 5,
                            m_spaceNumbersToText = 3,
                            m_spaceBorder = 5;
    const double m_rowSpacing = 1.0;
    std::unique_ptr<dialogAxis> m_dialog;
};

class widGraphXAxis: public widGraphAxis
{
public:
    widGraphXAxis(widGraph *graph);
    virtual double m_getDrawAreaPositionFromValue(double value) override;
    virtual void m_setAutoAxis() override;
    virtual void m_setDimensions() override;
    virtual dataAxis& m_getData() override;
private:
    virtual double m_getPositionFromValue(double value) override;
    virtual void m_drawLine(painterAntiAl &painter) override;
    virtual void m_drawTicks(painterAntiAl &painter) override;
    virtual void m_drawNumbers(painterAntiAl &painter) override;
    virtual void m_drawText(painterAntiAl &painter) override;

    double m_getTicksStart();
    double m_getTicksEnd();
    double m_getNumbersStart();
    double m_getNumberEnds();
    double m_getTextStart();
    double m_getTextEnds();
    double m_getEndFromTop();
protected:
};

class widGraphY1Axis: public widGraphAxis
{
public:
    widGraphY1Axis(widGraph *graph);
    virtual double m_getDrawAreaPositionFromValue(double value) override;
    virtual void m_setAutoAxis() override;
    virtual void m_setDimensions() override;
    virtual dataAxis& m_getData() override;
private:
    virtual double m_getPositionFromValue(double value) override;
    virtual void m_drawLine(painterAntiAl &painter) override;
    virtual void m_drawTicks(painterAntiAl &painter) override;
    virtual void m_drawNumbers(painterAntiAl &painter) override;
    virtual void m_drawText(painterAntiAl &painter) override;

    double m_getTicksStart();
    double m_getTicksEnd();
    double m_getNumbersStart();
    double m_getNumberEnds();
    double m_getTextStart();
    double m_getTextEnds();
};

class widGraphY2Axis: public widGraphAxis
{
public:
    widGraphY2Axis(widGraph *graph);
    virtual double m_getDrawAreaPositionFromValue(double value) override;
    virtual void m_setAutoAxis() override;
    virtual void m_setDimensions() override;
    virtual dataAxis& m_getData() override;
private:
    virtual double m_getPositionFromValue(double value) override;
    virtual void m_drawLine(painterAntiAl &painter) override;
    virtual void m_drawTicks(painterAntiAl &painter) override;
    virtual void m_drawNumbers(painterAntiAl &painter) override;
    virtual void m_drawText(painterAntiAl &painter) override;

    double m_getTicksStart();
    double m_getTicksEnd();
    double m_getNumbersStart();
    double m_getNumberEnds();
    double m_getTextStart();
    double m_getTextEnds();
};

class widGraphLegend: public widGraphElement
{
public:
    widGraphLegend(widGraph *graph);
    void paintEvent(QPaintEvent *event) override;
    virtual void m_setDimensions() override;
    virtual dataElement& m_getData() override;
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
