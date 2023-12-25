#ifndef WIDGRAPH_H
#define WIDGRAPH_H

#include "graph_global.h"
#include "dataGraph.h"
#include "objectsGraph.h"
#include "dialogGraph.h"
#include <QWidget>
#include <QGridLayout>
#include <QMouseEvent>
#include <QTimer>
#include <QMimeData>


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
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void m_setDimensions() = 0;
protected:
    widGraph *ptr_graph;
};

class widGraphDrawArea: public widGraphElement
{
public:
    widGraphDrawArea(widGraph *graph);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    virtual void m_setDimensions() override {}
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
    Q_OBJECT
public:
    widGraphButton(widGraph *graph, const QImage &icon, const QImage &iconActive, const QString &tooltip);
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void m_setDimensions() override;
    virtual void m_loadValues() {}
    inline void m_show()
        {m_isVisible = true;}
    inline void m_hide()
        {m_isVisible = false;}
public slots:
    void m_slotAnimationTimer();
private:
    virtual void m_onClick() = 0;
    virtual void m_doAnimation();
    virtual void m_drawInside(painterAntiAl &painter);
    void m_drawBorder(painterAntiAl &painter);
protected:
    QSizeF m_size = QSize(20,20);
    bool m_isChecked = false;
    bool m_isCheckable = false;
    bool m_isAnimation = false;
    bool m_isVisible = false;
    QImage m_icon, m_iconActive;
    QTimer *m_timerAnimation;
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

class widGraphButtonZoomIn: public widGraphButton
{
public:
    widGraphButtonZoomIn(widGraph *graph);
    virtual void m_loadValues() override;
    virtual void m_onClick() override;
};

class widGraphButtonZoomOut: public widGraphButton
{
public:
    widGraphButtonZoomOut(widGraph *graph);
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
    void paintEvent(QPaintEvent *event) override;
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void m_setDimensions() override;
    void m_showButtons();
    void m_hideButtons();
protected:
    void m_setButtonsDimensions();
    void m_loadButtonsValues();
    void m_drawLine(painterAntiAl &painter);
protected:
    const double m_rowSpacing = 1.0;
    const double m_spaceAbove = 5, m_spaceBelow = 5;
    QHBoxLayout *m_layBackground;
    widGraphTitleText *m_text;
    widGraphButtonAutoAxes *m_butAuto;
    widGraphButtonZoomIn *m_butZoomIn;
    widGraphButtonZoomOut *m_butZoomOut;
    widGraphButtonMove *m_butMove;
    widGraphButtonScreenshot *m_butScreenshot;
};

class widGraphAxis: public widGraphElement
{
public:
    widGraphAxis(widGraph *graph);
    ~widGraphAxis() = default;
    virtual double m_getDrawAreaPositionFromValue(double value) = 0;
    virtual double m_getValueFromDrawAreaPosition(double position) = 0;
    virtual std::tuple<double, double> m_getMinAndMaxOfObjects() = 0;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent */*event*/) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    virtual std::weak_ptr<dataAxis> m_getData() = 0;
    static double m_supCalculateNiceNumbers(float range, bool round);
    static std::tuple<double, double, double> m_calculateNiceMaxMin(double min, double max);
    void m_setAxis();
    void m_setAxisMinMax(double start, double end);
    bool m_cancelOperation();
    static std::vector<double> m_getTicksPosition(double min, double max, double step);
    void m_markForDrop(bool status);
    void m_unmarkForDrop();
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
    virtual bool m_dropCurve(std::weak_ptr<graphObject> ptr_curve)
        {return false;};
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
    widGraphXAxes(widGraph *graph):
        widGraphAxis(graph)
    {}
protected:
    virtual void m_supDrawNumber(painterAntiAl &painter, double location, double length) override;
    virtual void m_supDrawTick(painterAntiAl &painter, double location) override;
    virtual double m_getPositionFromValue(double value) override;
};

class widGraphYAxes: public widGraphAxis
{
public:
    widGraphYAxes(widGraph *graph, enum yAxisPosition position):
        widGraphAxis(graph), m_axisPosition(position)
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
    const enum yAxisPosition m_axisPosition;
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
    virtual bool m_dropCurve(std::weak_ptr<graphObject> ptr_object) override;
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
    virtual bool m_dropCurve(std::weak_ptr<graphObject> ptr_object) override;
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
    virtual bool m_dropCurve(std::weak_ptr<graphObject> ptr_curve) override;
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
    int m_getNCurvesWithLegend(int yAxis = -1);
    int m_getNRows();
    void m_drawTopLine(painterAntiAl &painter);
    void m_drawTexts(painterAntiAl &painter);
    void m_supDrawText(painterAntiAl &painter,
        std::shared_ptr<dataGraphObject> ptr_curveData, QRect rect, Qt::AlignmentFlag option);
    QVector<int> m_getTextLeftPositions(int nColumns);
};

class WIDGRAPH_EXPORT widGraph: public QWidget
{
    Q_OBJECT
public:
    widGraph();
    ~widGraph();
    widGraph(const widGraph&) = delete;
    widGraph& operator=(const widGraph&) = delete;
    void keyPressEvent(QKeyEvent *event) override;
    void hideEvent(QHideEvent *event) override;

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
    void m_addObject(std::shared_ptr<graphCurve> ptr_object);
    void m_addObject(std::shared_ptr<graphValue> ptr_object);
    void m_addObject(std::shared_ptr<graphColumn> ptr_object);
    void m_removeAllObjects();
    void m_removeObject(int curveIndex);
    void m_loadValues();
    void m_setCurveStyle(int curveIndex, QColor color, int curveWidth = 3, int curveStyleIndex = 1, bool showCurve = true);
    void m_setPointsStyle(int curveIndex, QColor color, int penPointsWidth = 3, int pointsShapeSize = 10, int pointsStyleIndex = 1, bool showPoints = true);
    void m_setAreaStyle(int curveIndex, QColor color, int areaStyleIndex = 1, bool showArea = true);
    void m_setColumnsStyle(int curveIndex, int columnWidth = 30, bool showColumn = true);
    void m_setCurveAxis(int curveIndex, yAxisPosition axis);
    void m_setConstCurveOrientation(int curveIndex, enum orientation orient);
    void m_setCurveName(int curveIndex, const std::string& name);
    void m_openDialog();
    void m_takeScreenshot();
    void m_zoomOut();
    static QString removeTrailingZeros(double number);
protected slots:
    void m_slotDialogClosed(int status);
private:
    std::shared_ptr<dataGraphObject> m_getObjectFromIndex(int curveIndex);
signals:
    void m_signalClose();
    void m_signalHide();
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
        dialogGraph* m_dialog = nullptr;
};

class graphLayout: public QGridLayout
{
public:
    graphLayout();
    ~graphLayout() = default;
};

class painterAntiAl: public QPainter
{
public:
    painterAntiAl(QPaintDevice *device);
};

class WIDGRAPH_EXPORT MimeDataWithGraphObject: public QMimeData
{
public:
    MimeDataWithGraphObject(std::weak_ptr<graphObject> ptr_curve):
        QMimeData(), ptr_object(ptr_curve)
    {}
    std::weak_ptr<graphObject> m_getGraphObject() const
        {return ptr_object;}
private:
    std::weak_ptr<graphObject> ptr_object;
};


#endif // WIDGRAPH_H
