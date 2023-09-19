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
    dataElement() {}
    dataElement(std::ifstream &instream) {}
    ~dataElement() = default;
    void m_saveToFile(std::ofstream &outstream)
    {
    }
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
    void m_setStep(double step);

    bool m_autoAxis = true, m_autoStep  = true;
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

    int m_width = -1;
    int m_height = -1;
    int m_fontText = 15;
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
    void m_setMove(bool status);
    void m_setZoom(bool status);
};

class graphObjects;
struct dataGraph
{
    dataGraph();
    dataGraph(std::ifstream &instream);
    dataGraph(const dataGraph&) = delete;
    dataGraph& operator=(const dataGraph& oldData);
    ~dataGraph() = default;
    void m_saveToFile(std::ofstream &outstream);

    std::shared_ptr<dataTitle> m_title;
    std::shared_ptr<dataAxisX> m_X;
    std::shared_ptr<dataAxisY1> m_Y1;
    std::shared_ptr<dataAxisY2> m_Y2;
    std::shared_ptr<dataLegend> m_legend;
    std::shared_ptr<dataDrawArea> m_drawArea;
    std::shared_ptr<dataControl> m_control;
    std::vector<std::shared_ptr<graphObjects>> m_vectorOfObjects;
    enum GRAPH_SETTINGS {TITLE, X, Y1, Y2, LEGEND, DRAWAREA};
};


struct dataGraphObject
{
    int m_prefferedYAxis = 0;
    int m_R = 0, m_G = 0, m_B = 0;
    std::string m_name;
};



#endif // DATAGRAPH_H
