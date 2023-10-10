#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

class widGraph;
class graphCurve;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

protected:
    std::shared_ptr<std::vector<double>> m_data1;
    std::shared_ptr<std::vector<double>> m_data2x, m_data2y;
    std::shared_ptr<double> m_data3;

    widGraph *m_widGraph;
};
#endif // MAINWINDOW_H
