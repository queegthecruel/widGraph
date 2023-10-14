#ifndef WIDGRAPH_H
#define WIDGRAPH_H

#include "graph_global.h"
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
    bool m_cancelOperation();
private:
    void m_moveByMouse();
    void m_zoomByMouse();
    void m_drawHorGrid(painterAntiAl &painter);
    void m_drawVertGrid(painterAntiAl &painter);
    void m_drawMove(painterAntiAl &painter);
    void m_drawSelectionRectangle(painterAntiAl &painter);
    void m_drawBorder(painterAntiAl &painter);
    void m_drawGrid(painterAntiAl &painter);
    void m_drawGraphObjects(painterAntiAl &painter);
protected:
    bool m_isMouseZooming = false;
    bool m_isMouseMoving = false;
    QPoint m_startingPoint;
};

class widGraphButton: public widGraphElement
{
public:
    widGraphButton(widGraph *graph, const QImage &icon, const QImage &iconActive, const QString &tooltip);
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void m_setDimensions() override;
    virtual void m_loadValues() {}
private:
    virtual void m_onClick() = 0;
    virtual void m_drawInside(painterAntiAl &painter);
    void m_drawBorder(painterAntiAl &painter);
protected:
    QSizeF m_size = QSize(20,20);
    bool m_isChecked = false;
    bool m_isCheckable = false;
    QImage m_icon, m_iconActive;
};

class widGraphButtonAutoAxes: public widGraphButton
{
public:
    widGraphButtonAutoAxes(widGraph *graph);
    virtual void m_onClick() override;
};

class widGraphButtonScreenshot: public widGraphButton
{
public:
    widGraphButtonScreenshot(widGraph *graph);
    virtual void m_onClick() override;
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
    virtual void m_loadValues() override;
    virtual void m_onClick() override;
};

class widGraphButtonMove: public widGraphButton
{
public:
    widGraphButtonMove(widGraph *graph);
    virtual void m_loadValues() override;
    virtual void m_onClick() override;
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
    QHBoxLayout *m_layBackground;
    widGraphTitleText *m_text;
    widGraphButtonAutoAxes *m_butAuto;
    widGraphButtonShowGrid *m_butShowGrid;
    widGraphButtonZoom *m_butZoom;
    widGraphButtonMove *m_butMove;
    widGraphButtonScreenshot *m_butScreenshot;
};

class widGraphAxis: public widGraphElement
{
public:
    widGraphAxis(widGraph *graph, int elementNumber);
    ~widGraphAxis() = default;
    virtual double m_getDrawAreaPositionFromValue(double value) = 0;
    virtual double m_getValueFromDrawAreaPosition(double position) = 0;
    virtual std::tuple<double, double> m_getMinAndMaxOfObjects() = 0;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent */*event*/) override;
    virtual std::weak_ptr<dataAxis> m_getData() = 0;
    static double m_supCalculateNiceNumbers(float range, bool round);
    static std::tuple<double, double, double> m_calculateNiceMaxMin(double min, double max);
    void m_setAxis();
    void m_setAxisMinMax(double start, double end);
    bool m_cancelOperation();
    static std::vector<double> m_getTicksPosition(double min, double max, double step);
private:
    virtual double m_getPositionFromValue(double value) = 0;
    virtual double m_getValueFromPosition(double position) = 0;
    virtual std::tuple<double, double> m_getStartAndEndFromMouse(QPointF start, QPointF end) = 0;
    virtual void m_drawLine(painterAntiAl &painter) = 0;
    virtual void m_drawTicks(painterAntiAl &painter) = 0;
    virtual void m_drawNumbers(painterAntiAl &painter) = 0;
    virtual void m_drawText(painterAntiAl &painter) = 0;
    virtual void m_drawZoomCursor(painterAntiAl &painter) = 0;
    virtual void m_drawMoveCursor(painterAntiAl &painter) = 0;
    void m_zoomByMouse();
    void m_moveByMouse();
protected:
    virtual double m_getTicksStart() = 0;
    virtual double m_getTicksEnd() = 0;
    virtual double m_getNumbersStart() = 0;
    virtual double m_getNumbersEnd() = 0;
    virtual double m_getTextStart() = 0;
    virtual double m_getTextEnds() = 0;
    virtual void m_supDrawNumber(painterAntiAl &painter, double location, double length) = 0;
    virtual void m_supDrawTick(painterAntiAl &painter, double location) = 0;
public:
    static bool m_supDistanceForActionIsSufficient(const QPoint &x1, const QPoint &x2);
protected:
// For positions
    const double m_tickLength = 10,
                 m_spaceTicksToNumbers = 5,
                 m_spaceNumbersToText = 3,
                 m_spaceBorder = 5;
    const double m_rowSpacing = 1.0;
// For zoom
    bool m_isMouseZooming = false;
    bool m_isMouseMoving = false;
    QPoint m_startingPoint;
    const double m_zoomCursorWidth = 20;
};

class widGraphXAxes: public widGraphAxis
{
public:
    widGraphXAxes(widGraph *graph, int elementNumber):
        widGraphAxis(graph, elementNumber)
    {}
protected:
    virtual void m_supDrawNumber(painterAntiAl &painter, double location, double length) override;
    virtual void m_supDrawTick(painterAntiAl &painter, double location) override;
    virtual double m_getPositionFromValue(double value) override;
};

class widGraphYAxes: public widGraphAxis
{
public:
    widGraphYAxes(widGraph *graph, int elementNumber, int position):
        widGraphAxis(graph, elementNumber), m_axisPosition(position)
    {}
    virtual double m_getDrawAreaPositionFromValue(double value) override;
    virtual double m_getValueFromDrawAreaPosition(double position) override;
    virtual std::tuple<double, double> m_getMinAndMaxOfObjects() override;
protected:
    virtual void m_drawMoveCursor(painterAntiAl &painter) override;
    virtual void m_drawZoomCursor(painterAntiAl &painter) override;
    virtual double m_getPositionFromValue(double value) override;
    virtual double m_getValueFromPosition(double position) override;
    virtual std::tuple<double, double> m_getStartAndEndFromMouse(QPointF start, QPointF end) override;
    virtual void m_supDrawNumber(painterAntiAl &painter, double location, double length) override;
    virtual void m_supDrawTick(painterAntiAl &painter, double location) override;
    virtual void m_drawTicks(painterAntiAl &painter) override;
    virtual void m_drawNumbers(painterAntiAl &painter) override;
    virtual void m_drawText(painterAntiAl &painter) override;
protected:
    const int m_axisPosition;
};

class widGraphXAxis: public widGraphXAxes
{
public:
    widGraphXAxis(widGraph *graph);
    ~widGraphXAxis() = default;
    virtual double m_getDrawAreaPositionFromValue(double value) override;
    virtual double m_getValueFromDrawAreaPosition(double position) override;
    virtual std::tuple<double, double> m_getMinAndMaxOfObjects() override;
    virtual void m_setDimensions() override;
    virtual std::weak_ptr<dataAxis> m_getData() override;
private:
    virtual double m_getValueFromPosition(double position) override;
    virtual std::tuple<double, double> m_getStartAndEndFromMouse(QPointF start, QPointF end) override;
    virtual void m_drawLine(painterAntiAl &painter) override;
    virtual void m_drawTicks(painterAntiAl &painter) override;
    virtual void m_drawNumbers(painterAntiAl &painter) override;
    virtual void m_drawText(painterAntiAl &painter) override;
    virtual void m_drawZoomCursor(painterAntiAl &painter) override;
    virtual void m_drawMoveCursor(painterAntiAl &painter) override;
protected:
    virtual double m_getTicksStart() override;
    virtual double m_getTicksEnd() override;
    virtual double m_getNumbersStart() override;
    virtual double m_getNumbersEnd() override;
    virtual double m_getTextStart() override;
    virtual double m_getTextEnds() override;
    double m_getEndFromTop();
protected:
};

class widGraphY1Axis: public widGraphYAxes
{
public:
    widGraphY1Axis(widGraph *graph);
    virtual void m_setDimensions() override;
    virtual std::weak_ptr<dataAxis> m_getData() override;
private:
    virtual void m_drawLine(painterAntiAl &painter) override;
protected:
    virtual double m_getTicksStart() override;
    virtual double m_getTicksEnd() override;
    virtual double m_getNumbersStart() override;
    virtual double m_getNumbersEnd() override;
    virtual double m_getTextStart() override;
    virtual double m_getTextEnds() override;
protected:

};

class widGraphY2Axis: public widGraphYAxes
{
public:
    widGraphY2Axis(widGraph *graph);
    ~widGraphY2Axis() = default;
    virtual void m_setDimensions() override;
    virtual std::weak_ptr<dataAxis> m_getData() override;
private:
    virtual void m_drawLine(painterAntiAl &painter) override;
protected:
    virtual double m_getTicksStart() override;
    virtual double m_getTicksEnd() override;
    virtual double m_getNumbersStart() override;
    virtual double m_getNumbersEnd() override;
    virtual double m_getTextStart() override;
    virtual double m_getTextEnds() override;
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

class WIDGRAPH_EXPORT widGraph: public QWidget
{
    Q_OBJECT
public:
    widGraph();
    widGraph(const widGraph&) = delete;
    widGraph& operator=(const widGraph&) = delete;
    void keyPressEvent(QKeyEvent *event) override;
    std::weak_ptr<dataGraph> m_getData()
        {return m_data;}
    inline void m_setData(std::shared_ptr<dataGraph> newData)
            {m_data = newData;}
    widGraphXAxis* m_getXAxis()
        {return m_widX;}
    widGraphY1Axis* m_getY1Axis()
        {return m_widY1;}
    widGraphY2Axis* m_getY2Axis()
        {return m_widY2;}
    void m_addObject(std::shared_ptr<graphObjects> ptr_object);
    void m_loadValues();
    void m_setCurveStyle(int curveIndex, QColor color, int axis = 0);
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

class graphLayout: public QGridLayout
{
public:
    graphLayout(QWidget *parent);
    ~graphLayout() = default;
};

class painterAntiAl: public QPainter
{
public:
    painterAntiAl(QPaintDevice *device);
};




#endif // WIDGRAPH_H
