#ifndef DATAGRAPH_H
#define DATAGRAPH_H

#include <memory>
#include <string>
#include <vector>
#include <fstream>

std::string readString(std::ifstream &instream);
void writeString(std::ofstream &outstream, const std::string& value);
double readDouble(std::ifstream &instream);
void writeDouble(std::ofstream &outstream, const double& value);
int readInt(std::ifstream &instream);
void writeInt(std::ofstream &outstream, const int& value);
bool readBool(std::ifstream &instream);
void writeBool(std::ofstream &outstream, const bool& value);

// Graph data
struct dataElement
{
    dataElement();
    dataElement(std::ifstream &/*instream*/);
    void m_saveToFile(std::ofstream &/*outstream*/);
    void m_setVisible(bool status, int manualSize = -1)
    {
        if (status) {
            m_show = true;
            m_manualSize = false;
            if (manualSize != -1) {
                m_manualSize = true;
                m_manualSizeValue = manualSize;
            }
        }
        else {
            m_show = true;
            m_manualSize = true;
            m_manualSizeValue = 0;
        }
    }

    bool m_show = true;
    bool m_manualSize = false;
    int m_manualSizeValue = 20;
};

struct dataTitle: public dataElement
{
    dataTitle();
    dataTitle(std::ifstream &instream):
        dataElement(instream)
    {

    }
    ~dataTitle() = default;
    void m_saveToFile(std::ofstream &outstream)
    {
        dataElement::m_saveToFile(outstream);
    }
    std::string title = "Graph title";
    int m_width = -1;
    int m_height = 50;
    std::string m_text = "widGraph";
    double m_fontText = 15;
};

struct dataAxis: public dataElement
{
    dataAxis();
    dataAxis(std::ifstream &instream);
    ~dataAxis() = default;
    void m_saveToFile(std::ofstream &outstream);
    void m_setMinMaxStep(double min, double max, double step);
    inline std::tuple<double, double, double> m_getMinMaxStep()
        {return {m_min, m_max, m_step};}
    void m_setStep(double step);

    bool m_autoAxis = true, m_manualStep  = true;
    double m_min = 0, m_max = 10, m_step = 1;
    int m_width = -1;
    int m_height = -1;
    double m_fontText = 15, m_fontNumbers = 15;
    std::string m_text, m_unit;
};

struct dataAxisX: public dataAxis
{
    dataAxisX();
    dataAxisX(std::ifstream &instream):
        dataAxis(instream) {}
    ~dataAxisX() = default;
    void m_saveToFile(std::ofstream &outstream)
    {
        dataAxis::m_saveToFile(outstream);
    }
};

struct dataAxisY1: public dataAxis
{
    dataAxisY1();
    dataAxisY1(std::ifstream &instream):
        dataAxis(instream) {}
    ~dataAxisY1() = default;
    void m_saveToFile(std::ofstream &outstream)
    {
        dataAxis::m_saveToFile(outstream);
    }
};

struct dataAxisY2: public dataAxis
{
    dataAxisY2();
    dataAxisY2(std::ifstream &instream):
        dataAxis(instream) {}
    ~dataAxisY2() = default;
    void m_saveToFile(std::ofstream &outstream)
    {
        dataAxis::m_saveToFile(outstream);
    }
};

struct dataLegend: public dataElement
{
    dataLegend();
    dataLegend(std::ifstream &instream):
        dataElement(instream) {}
    ~dataLegend() = default;
    void m_saveToFile(std::ofstream &outstream)
    {
        dataElement::m_saveToFile(outstream);
    }
    int m_getNFinalColumns();

    int m_width = -1, m_height = -1;
    int m_fontText = 15;
    int m_nColumns = 1;
    bool m_showTopLine = true;
    bool m_arrangeToAxes = true;
};

struct dataDrawArea: public dataElement
{
    dataDrawArea() {}
    dataDrawArea(std::ifstream &instream):
        dataElement(instream) {}
    ~dataDrawArea() = default;
    void m_saveToFile(std::ofstream &outstream)
    {
        dataElement::m_saveToFile(outstream);
    }
    bool m_showGrid = true;
    bool m_showYAxesAtX = true, m_showXAxesAtY1 = true, m_showXAxesAtY2 = true;
};

struct dataControl: public dataElement
{
    dataControl() {}
    dataControl(std::ifstream &instream):
        dataElement(instream) {}
    ~dataControl() = default;
    void m_saveToFile(std::ofstream &outstream)
    {
        dataElement::m_saveToFile(outstream);
    }
    bool m_zoom = false, m_move = false;
    bool m_allowDialog = true;
    bool m_allowDrop = false;

    void m_setMove(bool status);
    void m_setZoom(bool status);
    void m_setNoZoomNoMove();
};

class graphObject;
struct dataGraph
{
    dataGraph();
    dataGraph(std::ifstream &instream);
    dataGraph(const dataGraph&) = delete;
    dataGraph& operator=(const dataGraph& oldData) = delete;
    ~dataGraph() = default;
    void m_saveToFile(std::ofstream &outstream);
    void m_addObject(std::shared_ptr<graphObject> ptr_object);
    void m_removeAllObjects();
    void m_removeObject(int curveIndex);

    std::shared_ptr<dataTitle> m_title;
    std::shared_ptr<dataAxisX> m_X;
    std::shared_ptr<dataAxisY1> m_Y1;
    std::shared_ptr<dataAxisY2> m_Y2;
    std::shared_ptr<dataLegend> m_legend;
    std::shared_ptr<dataDrawArea> m_drawArea;
    std::shared_ptr<dataControl> m_control;

    std::vector<std::shared_ptr<graphObject>> m_vectorOfObjects;
};



#endif // DATAGRAPH_H
