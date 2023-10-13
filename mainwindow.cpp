#include "mainwindow.h"
#include "widGraph.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent)
{
    resize(800,600);
    setStyleSheet("background: white;");

    m_widGraph = new widGraph();
    auto ptr_x = m_widGraph->m_getData().lock()->m_X;
    ptr_x->m_text = "X-axis";
    ptr_x->m_unit = "km";
    ptr_x->m_fontNumbers = 17;
    ptr_x->m_fontText = 17;

    auto ptr_y1 = m_widGraph->m_getData().lock()->m_Y1;
    ptr_y1->m_text = "Y-axis 1";
    ptr_y1->m_unit = "km/h";
    ptr_y1->m_fontNumbers = 17;
    ptr_y1->m_fontText = 17;

    auto ptr_y2 = m_widGraph->m_getData().lock()->m_Y2;
    ptr_y2->m_text = "Y-axis 2";
    ptr_y2->m_unit = "kN";
    ptr_y2->m_fontNumbers = 17;
    ptr_y2->m_fontText = 17;

    // Dataset 1
        auto numbers1 = std::vector<double>{4, 4, 12, 4, 4, 4};
        m_data1 = std::make_shared<std::vector<double>>(numbers1);
        m_widGraph->m_addObject(std::make_shared<graphCurve>(m_data1));
        m_widGraph->m_setCurveStyle(-1, QColor(255,0,0,128));
        m_widGraph->m_setCurveName(-1, "Curve 1");
    // Dataset 2
        auto numbers2x = std::vector<double>{0, 1, 2, 7, 8, 9};
        auto numbers2y = std::vector<double>{1, 1, 1, 88, 3, 2};
        m_data2x = std::make_shared<std::vector<double>>(numbers2x);
        m_data2y = std::make_shared<std::vector<double>>(numbers2y);
        m_widGraph->m_addObject(std::make_shared<graphCurve>(m_data2x, m_data2y));
        m_widGraph->m_setCurveStyle(-1, Qt::green, 1);
        m_widGraph->m_setCurveName(-1, "Curve 2");
    // Dataset 3
        double numbers3 = 15;
        m_data3 = std::make_shared<double>(numbers3);
        m_widGraph->m_addObject(std::make_shared<graphYValue>(m_data3));
        m_widGraph->m_setCurveStyle(-1, Qt::blue, 0);
        m_widGraph->m_setCurveName(-1, "Curve 3");

    // Reload graph
        m_widGraph->m_loadValues();
    setCentralWidget(m_widGraph);
}

