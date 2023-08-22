#include "dataGraph.h"

dataTitle::dataTitle()
{

}

dataAxis::dataAxis()
{

}

dataAxisX::dataAxisX()
{
    m_min = 0;
    m_max = 10;
    m_step = 1;
    m_fontNumbers = 15;
    m_fontText = 15;
}

dataAxisY1::dataAxisY1()
{
    m_width = 50;
    m_min = 0;
    m_max = 5;
    m_step = 1;
}

dataAxisY2::dataAxisY2()
{
    m_width = 50;
}

dataLegend::dataLegend()
{
    m_height = 50;
}

dataGraph::dataGraph()
{
     m_title = std::make_shared<dataTitle>();
     m_X = std::make_shared<dataAxisX>();
     m_Y1 = std::make_shared<dataAxisY1>();
     m_Y2 = std::make_shared<dataAxisY2>();
     m_legend = std::make_shared<dataLegend>();
     m_drawArea = std::make_shared<dataDrawArea>();
}

dataGraph::dataGraph(std::ifstream &instream)
{
     m_title = std::make_shared<dataTitle>(instream);
     m_X = std::make_shared<dataAxisX>(instream);
     m_Y1 = std::make_shared<dataAxisY1>(instream);
     m_Y2 = std::make_shared<dataAxisY2>(instream);
     m_legend = std::make_shared<dataLegend>(instream);
     m_drawArea = std::make_shared<dataDrawArea>(instream);
}

dataGraph &dataGraph::operator=(const dataGraph &oldData)
{
    *m_title = *oldData.m_title;
    *m_X = *oldData.m_X;
    *m_Y1 = *oldData.m_Y1;
    *m_Y2 = *oldData.m_Y2;
    *m_legend = *oldData.m_legend;
    *m_drawArea = *oldData.m_drawArea;

    m_vectorOfObjects.clear();
    for(const auto &var: oldData.m_vectorOfObjects)
        m_vectorOfObjects.push_back(var);
    return *this;
}

void dataGraph::m_saveToFile(std::ofstream &outstream)
{
    m_title->m_saveToFile(outstream);
    m_X->m_saveToFile(outstream);
    m_Y1->m_saveToFile(outstream);
    m_Y2->m_saveToFile(outstream);
    m_legend->m_saveToFile(outstream);
    m_drawArea->m_saveToFile(outstream);
}


bool readBool(std::ifstream &instream)
{
    bool value;
    instream >> value;
    return value;
}

int readInt(std::ifstream &instream)
{
    int value;
    instream >> value;
    return value;
}

double readDouble(std::ifstream &instream)
{
    double value;
    instream >> value;
    return value;
}

std::string readString(std::ifstream &instream)
{
    std::string value;
    instream >> value;
    return value;
}

void writeBool(std::ofstream &outstream, const bool &value)
{
    outstream << value;
}

void writeInt(std::ofstream &outstream, const int &value)
{
    outstream << value;
}

void writeDouble(std::ofstream &outstream, const double &value)
{
    outstream << value;
}

void writeString(std::ofstream &outstream, const std::string &value)
{
    outstream << value;
}
