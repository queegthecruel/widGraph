#include "dataGraph.h"

dataElement::dataElement() {}

dataElement::dataElement(std::ifstream &) {}

void dataElement::m_saveToFile(std::ofstream &)
{
}

dataTitle::dataTitle()
{

}

dataAxis::dataAxis()
{

}

dataAxis::dataAxis(std::ifstream &instream):
    dataElement(instream)
{
    m_fontText = readDouble(instream);
    m_fontNumbers = readDouble(instream);
}

void dataAxis::m_saveToFile(std::ofstream &outstream)
{
    dataElement::m_saveToFile(outstream);
    writeDouble(outstream, m_fontText);
    writeDouble(outstream, m_fontNumbers);
}

void dataAxis::m_setMinMaxStep(double min, double max, double step)
{
    m_min = min;
    m_max = max;
    m_step = step;
}

void dataAxis::m_setStep(double step)
{
    m_step = step;
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

int dataLegend::m_getNFinalColumns()
{
    if (m_arrangeToAxes)
        return 2;
    else
        return m_nColumns;
}

dataGraph::dataGraph()
{
    m_title = std::make_shared<dataTitle>();
    m_X = std::make_shared<dataAxisX>();
    m_Y1 = std::make_shared<dataAxisY1>();
    m_Y2 = std::make_shared<dataAxisY2>();
    m_legend = std::make_shared<dataLegend>();
    m_drawArea = std::make_shared<dataDrawArea>();
    m_control = std::make_shared<dataControl>();
}

dataGraph::dataGraph(std::ifstream &instream)
{
    m_title = std::make_shared<dataTitle>(instream);
    m_X = std::make_shared<dataAxisX>(instream);
    m_Y1 = std::make_shared<dataAxisY1>(instream);
    m_Y2 = std::make_shared<dataAxisY2>(instream);
    m_legend = std::make_shared<dataLegend>(instream);
    m_drawArea = std::make_shared<dataDrawArea>(instream);
    m_control = std::make_shared<dataControl>(instream);
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

void dataGraph::m_addObject(std::shared_ptr<graphObject> ptr_object)
{
    m_vectorOfObjects.push_back(ptr_object);
}

void dataGraph::m_removeAllObjects()
{
    for (int i = m_vectorOfObjects.size() - 1; i >= 0; --i)
        m_vectorOfObjects.erase(m_vectorOfObjects.begin() + i);
}

void dataGraph::m_removeObject(int curveIndex)
{
    if (curveIndex == -1)
        curveIndex = m_vectorOfObjects.size() - 1;
    for (int i = m_vectorOfObjects.size() - 1; i >= 0; --i)
        if(i == curveIndex)
            m_vectorOfObjects.erase(m_vectorOfObjects.begin() + i);
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

void dataControl::m_setMove(bool status)
{
    m_setNoZoomNoMove();
    m_move = status;
}

void dataControl::m_setZoom(bool status)
{
    m_setNoZoomNoMove();
    m_zoom = status;
}

void dataControl::m_setNoZoomNoMove()
{
    m_move = false;
    m_zoom = false;
}

