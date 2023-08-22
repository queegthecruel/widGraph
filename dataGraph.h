#ifndef DATAGRAPH_H
#define DATAGRAPH_H

#include <memory>
#include <string>
#include <vector>

// Graph data
struct dataElement {};

struct dataTitle: public dataElement
{
    dataTitle();
    ~dataTitle() = default;
    std::string title = "Graph title";
    int m_width = -1;
    int m_height = 50;
};

struct dataAxis: public dataElement
{
    dataAxis();
    ~dataAxis() = default;

    double m_min = 0, m_max = 10, m_step = 1;
    int m_width = -1;
    int m_height = -1;
    double m_fontText = 15, m_fontNumbers = 15;
    std::string m_text, m_unit;
};

struct dataAxisX: public dataAxis
{
    dataAxisX();
    ~dataAxisX() = default;
};

struct dataAxisY1: public dataAxis
{
    dataAxisY1();
    ~dataAxisY1() = default;
};

struct dataAxisY2: public dataAxis
{
    dataAxisY2();
    ~dataAxisY2() = default;
};

struct dataLegend: public dataElement
{
    dataLegend();
    ~dataLegend() = default;

    int m_width = -1;
    int m_height = -1;
    int m_fontHeight = 15;
};

struct dataDrawArea: public dataElement
{
    dataDrawArea() {}
    ~dataDrawArea() = default;
};

class graphObjects;
struct dataGraph
{
    dataGraph();
    dataGraph(const dataGraph&) = delete;
    dataGraph& operator=(const dataGraph&) = delete;
    ~dataGraph() = default;

    std::shared_ptr<dataTitle> m_title;
    std::shared_ptr<dataAxisX> m_X;
    std::shared_ptr<dataAxisY1> m_Y1;
    std::shared_ptr<dataAxisY2> m_Y2;
    std::shared_ptr<dataLegend> m_legend;
    std::shared_ptr<dataDrawArea> m_drawArea;
    std::vector<std::shared_ptr<graphObjects>> m_vectorOfObjects;
};


struct dataGraphObject
{
    int m_prefferedYAxis = 0;
    int m_R = 0, m_G = 0, m_B = 0;
    std::string m_name;
};



#endif // DATAGRAPH_H
