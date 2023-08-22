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

