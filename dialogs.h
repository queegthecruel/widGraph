#ifndef DIALOGS_H
#define DIALOGS_H

#include "widOthers.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>

class dialogs: public QDialog
{
public:
    dialogs();
    ~dialogs() = default;
};

class dataAxis;
class widGraph;
class dialogAxis: public dialogs
{
    Q_OBJECT
public:
    dialogAxis(widGraph *graph, dataAxis &data);
    virtual ~dialogAxis() = default;
    void m_loadValues();
    void m_saveValues();
public slots:
    void m_slotLoad();
    void m_slotSave();
protected:
    widGraph *ptr_graph;
    dataAxis& ptr_data;

    QPushButton *m_butLoad, *m_butSave;
    QVBoxLayout *m_layBackground;
    treeWidget *m_tree;
    lineEdit *m_editFontSizeNumbers;
};

#endif // DIALOGS_H
