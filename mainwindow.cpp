#include "mainwindow.h"
#include "widGraph.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent)
{
    resize(800,600);
    setStyleSheet("background: white;");

    m_widGraph = new widGraph();
    m_widGraph->m_getData().lock()->m_X.m_text = "X-axis";
    m_widGraph->m_getData().lock()->m_X.m_unit = "km";

    // Dataset 1
        auto numbers1 = std::vector<double>{4, 4, 4, 4, 4, 4};

        m_data1 = std::make_shared<std::vector<double>>(numbers1);
        m_widGraph->m_addObject(std::make_shared<graphCurve>(m_data1));
        m_widGraph->m_setCurveStyle(-1, 255, 0, 0);
        m_widGraph->m_setCurveName(-1, "Curve 1");
        (*m_data1)[2] = 12;
    // Dataset 2
        auto numbers2x = std::vector<double>{0, 1, 2, 7, 8, 9};
        auto numbers2y = std::vector<double>{1, 1, 1, 88, 3, 2};
        m_data2x = std::make_shared<std::vector<double>>(numbers2x);
        m_data2y = std::make_shared<std::vector<double>>(numbers2y);
        m_widGraph->m_addObject(std::make_shared<graphCurve>(m_data2x, m_data2y));
        m_widGraph->m_setCurveStyle(-1, 0, 255, 0, 1);
        m_widGraph->m_setCurveName(-1, "Curve 2");

    // Reload graph
        m_widGraph->m_loadValues();
    setCentralWidget(m_widGraph);
}

