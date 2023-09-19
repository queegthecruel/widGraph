#ifndef WIDGRAPH_H
#define WIDGRAPH_H

#include "dataGraph.h"
#include "objectsGraph.h"
#include "dialogGraph.h"
#include <QWidget>
#include <QGridLayout>
#include <QMouseEvent>

// Graph widgets
class widGraph;
class painterAntiAl;

class widGraphElement: public QWidget
{
public:
    widGraphElement(widGraph *graph, int elementNumber):
        ptr_graph(graph), m_elementNumber(elementNumber)
    {}
    void paintEvent(QPaintEvent *event) override = 0;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void m_setDimensions() = 0;
protected:
    widGraph *ptr_graph;
    const int m_elementNumber;
};

class widGraphDrawArea: public widGraphElement
{
public:
    widGraphDrawArea(widGraph *graph);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    virtual void m_setDimensions() override {};
    void m_setAxisMinMax(double startX, double endX,
        double startY1, double endY1, double startY2, double endY2);
private:
    void m_drawHorGrid(painterAntiAl &painter);
    void m_drawVertGrid(painterAntiAl &painter);
protected:
// For zoom
    bool m_isMouseMoving = false;
    QPoint m_startingPoint;
};

class widGraphButton: public widGraphElement
{
public:
    widGraphButton(widGraph *graph, const QString &tooltip);
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void m_setDimensions() override;
private:
    virtual void m_onClick() = 0;
    virtual void m_drawInside(painterAntiAl &painter) = 0;
    void m_drawBorder(painterAntiAl &painter);
protected:
    QSizeF m_size = QSize(20,20);
    bool m_isChecked = false;
    bool m_isCheckable = false;
};

class widGraphButtonAutoAxes: public widGraphButton
{
public:
    widGraphButtonAutoAxes(widGraph *graph);
    virtual void m_onClick() override;
    virtual void m_drawInside(painterAntiAl &painter) override;
};

class widGraphButtonScreenshot: public widGraphButton
{
public:
    widGraphButtonScreenshot(widGraph *graph);
    virtual void m_onClick() override;
    virtual void m_drawInside(painterAntiAl &painter) override;
};

class widGraphButtonShowGrid: public widGraphButton
{
public:
    widGraphButtonShowGrid(widGraph *graph);
    virtual void m_onClick() override;
    virtual void m_drawInside(painterAntiAl &painter) override;
};

class widGraphButtonZoom: public widGraphButton
{
public:
    widGraphButtonZoom(widGraph *graph);
    virtual void m_onClick() override;
    virtual void m_drawInside(painterAntiAl &painter) override;
};

class widGraphTitleText: public widGraphElement
{
public:
    widGraphTitleText(widGraph *graph);
    void paintEvent(QPaintEvent *event) override ;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void m_setDimensions() override;
};

class widGraphTitle: public widGraphElement
{
public:
    widGraphTitle(widGraph *graph);
    void paintEvent(QPaintEvent *event) override;;
    virtual void m_setDimensions() override;
protected:
    const double m_rowSpacing = 1.0;
    const double m_spaceAbove = 5, m_spaceBelow = 5;
    HBoxLayout *m_layBackground;
    widGraphTitleText *m_text;
    widGraphButtonAutoAxes *m_butAuto;
    widGraphButtonShowGrid *m_butShowGrid;
    widGraphButtonZoom *m_butZoom;
    widGraphButtonScreenshot *m_butScreenshot;
};

class widGraphAxis: public widGraphElement
{
public:
    widGraphAxis(widGraph *graph, int elementNumber);
    ~widGraphAxis() = default;
    virtual double m_getDrawAreaPositionFromValue(double value) = 0;
    virtual double m_getValueFromDrawAreaPosition(double position) = 0;
    virtual std::tuple<double, double> m_getMinAndMax() = 0;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent */*event*/) override;
    virtual std::weak_ptr<dataAxis> m_getData() = 0;
    static double m_supCalculateNiceNumbers(float range, bool round);
    static std::tuple<double, double, double> m_calculateNiceMaxMin(double min, double max);
    void m_setAxis();
    void m_setAxisMinMax(double start, double end);
private:
    virtual double m_getPositionFromValue(double value) = 0;
    virtual double m_getValueFromPosition(double position) = 0;
    virtual std::tuple<double, double> m_getStartAndEndFromMouse(QPointF start, QPointF end) = 0;
    virtual void m_drawLine(painterAntiAl &painter) = 0;
    virtual void m_drawTicks(painterAntiAl &painter) = 0;
    virtual void m_drawNumbers(painterAntiAl &painter) = 0;
    virtual void m_drawText(painterAntiAl &painter) = 0;
    virtual void m_drawZoomCursor(painterAntiAl &painter) = 0;
public:
    static bool m_supDistanceForZoomIsSufficient(const QPoint &x1, const QPoint &x2);
protected:
// For positions
    const double m_tickLength = 10,
                 m_spaceTicksToNumbers = 5,
                 m_spaceNumbersToText = 3,
                 m_spaceBorder = 5;
    const double m_rowSpacing = 1.0;

// For zoom
    bool m_isMouseMoving = false;
    QPoint m_startingPoint;
    const double m_zoomCursorWidth = 20;
};

class widGraphXAxis: public widGraphAxis
{
public:
    widGraphXAxis(widGraph *graph);
    ~widGraphXAxis() = default;
    virtual double m_getDrawAreaPositionFromValue(double value) override;
    virtual double m_getValueFromDrawAreaPosition(double position) override;
    virtual std::tuple<double, double> m_getMinAndMax() override;
    virtual void m_setDimensions() override;
    virtual std::weak_ptr<dataAxis> m_getData() override;
private:
    virtual double m_getPositionFromValue(double value) override;
    virtual double m_getValueFromPosition(double position) override;
    virtual std::tuple<double, double> m_getStartAndEndFromMouse(QPointF start, QPointF end) override;
    virtual void m_drawLine(painterAntiAl &painter) override;
    virtual void m_drawTicks(painterAntiAl &painter) override;
    virtual void m_drawNumbers(painterAntiAl &painter) override;
    virtual void m_drawText(painterAntiAl &painter) override;
    virtual void m_drawZoomCursor(painterAntiAl &painter) override;

    double m_getTicksStart();
    double m_getTicksEnd();
    double m_getNumbersStart();
    double m_getNumbersEnd();
    double m_getTextStart();
    double m_getTextEnds();
    double m_getEndFromTop();
protected:
};

class widGraphY1Axis: public widGraphAxis
{
public:
    widGraphY1Axis(widGraph *graph);
    ~widGraphY1Axis() = default;
    virtual double m_getDrawAreaPositionFromValue(double value) override;
    virtual double m_getValueFromDrawAreaPosition(double position) override;
    virtual std::tuple<double, double> m_getMinAndMax() override;
    virtual void m_setDimensions() override;
    virtual std::weak_ptr<dataAxis> m_getData() override;
private:
    virtual double m_getPositionFromValue(double value) override;
    virtual double m_getValueFromPosition(double position) override;
    virtual std::tuple<double, double> m_getStartAndEndFromMouse(QPointF start, QPointF end) override;
    virtual void m_drawLine(painterAntiAl &painter) override;
    virtual void m_drawTicks(painterAntiAl &painter) override;
    virtual void m_drawNumbers(painterAntiAl &painter) override;
    virtual void m_drawText(painterAntiAl &painter) override;
    virtual void m_drawZoomCursor(painterAntiAl &painter) override;

    double m_getTicksStart();
    double m_getTicksEnd();
    double m_getNumbersStart();
    double m_getNumbersEnd();
    double m_getTextStart();
    double m_getTextEnds();
protected:

};

class widGraphY2Axis: public widGraphAxis
{
public:
    widGraphY2Axis(widGraph *graph);
    ~widGraphY2Axis() = default;
    virtual double m_getDrawAreaPositionFromValue(double value) override;
    virtual double m_getValueFromDrawAreaPosition(double position) override;
    virtual std::tuple<double, double> m_getMinAndMax() override;
    virtual void m_setDimensions() override;
    virtual std::weak_ptr<dataAxis> m_getData() override;
private:
    virtual double m_getPositionFromValue(double value) override;
    virtual double m_getValueFromPosition(double position) override;
    virtual std::tuple<double, double> m_getStartAndEndFromMouse(QPointF start, QPointF end) override;
    virtual void m_drawLine(painterAntiAl &painter) override;
    virtual void m_drawTicks(painterAntiAl &painter) override;
    virtual void m_drawNumbers(painterAntiAl &painter) override;
    virtual void m_drawText(painterAntiAl &painter) override;
    virtual void m_drawZoomCursor(painterAntiAl &painter) override;

    double m_getTicksStart();
    double m_getTicksEnd();
    double m_getNumbersStart();
    double m_getNumbersEnd();
    double m_getTextStart();
    double m_getTextEnds();
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
    Q_OBJECT
public:
    widGraph();
    widGraph(const widGraph&) = delete;
    widGraph& operator=(const widGraph&) = delete;
    std::weak_ptr<dataGraph> m_getData()
        {return m_data;}
    inline void m_setData(std::shared_ptr<dataGraph> newData)
            {*m_data = *newData;}
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
    void m_openDialog(int tabIndex = 0);
    void m_takeScreenshot();
public slots:
    void m_slotDialogClosed(int status);
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
    // Dialog
        std::unique_ptr<dialogGraph> m_dialog;
};





#endif // WIDGRAPH_H
