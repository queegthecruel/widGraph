#ifndef SUPWIDGETS_H
#define SUPWIDGETS_H

#include "widPretty.h"
#include "widGraph.h"

#include <vector>
#include <memory>

class graphObject;
class WIDGRAPH_EXPORT dataStructure
{
public:
    dataStructure();
    dataStructure(const std::string &name);
    inline std::vector<std::shared_ptr<graphObject>> m_getVGraphObjects()
        {return m_vGraphObjects;}
    inline std::vector<std::shared_ptr<dataStructure>> m_getVDataStructures()
        {return m_vDataStructures;}
    inline const std::string &m_getName()
        {return m_name;}
private:
    std::string m_name;
    std::shared_ptr<std::vector<double>> m_curveXData;

    std::vector<std::shared_ptr<std::vector<double>>> m_vData;
    std::vector<std::shared_ptr<graphObject>> m_vGraphObjects;

    std::vector<std::shared_ptr<dataStructure>> m_vDataStructures;
public:
    // Test
        std::shared_ptr<double> m_dataTestValue;
        std::shared_ptr<graphObject> m_graphObjectsTestValue;
        std::shared_ptr<std::vector<double>> m_dataTestColumn;
        std::shared_ptr<graphObject> m_graphObjectsTestColumn;
};

class treeWidgetDraggable;
class listGraphItems;
class previewGraph: public QFrame
{
    Q_OBJECT
public:
    previewGraph(treeWidgetDraggable *ptr_list);
private:
    void m_setGraphParams();
    virtual QSize sizeHint() const;
public slots:
    void m_slotUpdateCurve();
private:
    treeWidgetDraggable *ptr_listWithCurves;
    label *m_labMin, *m_labMax, *m_labAvg, *m_labValues;
    widGraph *m_widGraph;
};

class WIDGRAPH_EXPORT listGraphItems: public QWidget
{
public:
   listGraphItems();
   void m_addDataStructure(std::shared_ptr<dataStructure> ptr_dataStruct);
   treeWidgetDraggable *m_getWidTree()
        {return m_tree;}
private:
   treeWidgetDraggable *m_tree;
   previewGraph *m_preview;
};

class treeWidgetDraggableItemAbstract;
class treeWidgetDraggableItem;
class WIDGRAPH_EXPORT treeWidgetDraggable: public QTreeWidget
{
public:
    treeWidgetDraggable();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void m_addDataStructure(std::shared_ptr<dataStructure> ptr_dataStruct);
    treeWidgetDraggableItem *m_getSelectedItem();
private:
    QVector<std::weak_ptr<dataStructure>> m_vPtrData;
    treeWidgetDraggableItem *ptr_dragged = nullptr;
};

class treeWidgetDraggableTopItem;
class treeWidgetDraggableItemAbstract: public QTreeWidgetItem
{
public:
    treeWidgetDraggableItemAbstract(treeWidgetDraggable *parent);
    treeWidgetDraggableItemAbstract(treeWidgetDraggableTopItem *parent);
    virtual void m_loadValues() = 0;
};

class WIDGRAPH_EXPORT treeWidgetDraggableItem: public treeWidgetDraggableItemAbstract
{
public:
    treeWidgetDraggableItem(std::weak_ptr<graphObject> ptr_curve, treeWidgetDraggable *parent);
    treeWidgetDraggableItem(std::weak_ptr<graphObject> ptr_curve, treeWidgetDraggableTopItem *parent);
    virtual void m_loadValues() override;
    std::weak_ptr<graphObject> m_getGraphObject()
        {return ptr_object;}
private:
    std::weak_ptr<graphObject> ptr_object;
};

class WIDGRAPH_EXPORT treeWidgetDraggableTopItem: public treeWidgetDraggableItemAbstract
{
public:
    treeWidgetDraggableTopItem(std::shared_ptr<dataStructure> ptr_dataStruct,
                               treeWidgetDraggable *parent);
    treeWidgetDraggableTopItem(std::shared_ptr<dataStructure> ptr_dataStruct,
                               treeWidgetDraggableTopItem *parent);
    virtual void m_loadValues() override;
private:
    void m_init(std::shared_ptr<dataStructure> ptr_dataStruct);
private:
    std::weak_ptr<dataStructure> ptr_dataStructure;
    std::vector<treeWidgetDraggableItemAbstract*> m_vChildren;
};

#endif // SUPWIDGETS_H
