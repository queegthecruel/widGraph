#include "supWidgets.h"
#include <QDrag>

treeWidgetDraggable::treeWidgetDraggable()
{
    setColumnCount(3);
    setHeaderLabels({"Item", "Unit", "Value"});
    setColumnWidth(0, 125);
    setColumnWidth(1, 50);
    setColumnWidth(2, 75);
    setIndentation(8);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(InternalMove);
    setDragEnabled(false);
    // setFocusPolicy(Qt::NoFocus);
    setSizePolicy(sizePolicy().horizontalPolicy(), QSizePolicy::Ignored);
}

void treeWidgetDraggable::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);
    auto *ptr_child = dynamic_cast<treeWidgetDraggableItem*>(itemAt(event->pos()));
    if (ptr_child)
        ptr_dragged = ptr_child;
    else
        ptr_dragged = nullptr;
}

void treeWidgetDraggable::mouseReleaseEvent(QMouseEvent *event)
{
    QTreeWidget::mouseReleaseEvent(event);
    ptr_dragged = nullptr;
}

void treeWidgetDraggable::mouseMoveEvent(QMouseEvent *event)
{
    QTreeWidget::mouseMoveEvent(event);
    if (! ptr_dragged) return;
    auto ptr_object = ptr_dragged->m_getGraphObject();
    auto *mimeData = new MimeDataWithGraphObject(ptr_object);

    QByteArray itemData;
    mimeData->setData("widGraph/curve", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->exec(Qt::LinkAction);
}

void treeWidgetDraggable::m_addDataStructure(std::shared_ptr<dataStructure> ptr_dataStruct)
{
    m_vPtrData.push_back(ptr_dataStruct);
    auto *item = new treeWidgetDraggableTopItem(ptr_dataStruct, this);
    item->m_loadValues();
}

treeWidgetDraggableItem *treeWidgetDraggable::m_getSelectedItem()
{
    auto *ptr_child = dynamic_cast<treeWidgetDraggableItem*>(currentItem());
    return ptr_child;
}

treeWidgetDraggableItem::treeWidgetDraggableItem(
    std::weak_ptr<graphObject> ptr_graphObject,
    treeWidgetDraggable *parent):
    treeWidgetDraggableItemAbstract(parent),
    ptr_object(ptr_graphObject)
{}

treeWidgetDraggableItem::treeWidgetDraggableItem(
    std::weak_ptr<graphObject> ptr_graphObject,
    treeWidgetDraggableTopItem *parent):
    treeWidgetDraggableItemAbstract(parent),
    ptr_object(ptr_graphObject)
{}

void treeWidgetDraggableItem::m_loadValues()
{
    // Icon
        QIcon icon;
        if (auto curve = ptr_object.lock())
            icon = curve->m_getIcon();
        setIcon(0, icon);
    // Name
        QString name;
        if (auto curve = ptr_object.lock())
            name = curve->m_getName().c_str();
        setText(0, name);
        setToolTip(0, name);
    // Name
        QString unit = "-";
        setText(1, unit);
        setToolTip(1, unit);
    // Info
        QString info;
        if (auto curve = ptr_object.lock())
            info = curve->m_getInfo().c_str();
        setText(2, info);
        setToolTip(2, info);
}

treeWidgetDraggableItemAbstract::treeWidgetDraggableItemAbstract(treeWidgetDraggable *parent):
    QTreeWidgetItem(parent) {}

treeWidgetDraggableItemAbstract::treeWidgetDraggableItemAbstract(treeWidgetDraggableTopItem *parent):
    QTreeWidgetItem(parent) {}

treeWidgetDraggableTopItem::treeWidgetDraggableTopItem(
    std::shared_ptr<dataStructure> ptr_dataStruct,
    treeWidgetDraggable *parent):
    treeWidgetDraggableItemAbstract(parent),
    ptr_dataStructure(ptr_dataStruct)
{
    m_init(ptr_dataStruct);
}

treeWidgetDraggableTopItem::treeWidgetDraggableTopItem(
    std::shared_ptr<dataStructure> ptr_dataStruct,
    treeWidgetDraggableTopItem *parent):
    treeWidgetDraggableItemAbstract(parent),
    ptr_dataStructure(ptr_dataStruct)
{
    m_init(ptr_dataStruct);
}

void treeWidgetDraggableTopItem::m_init(std::shared_ptr<dataStructure> ptr_dataStruct)
{
    // Curves
        auto vCurves = ptr_dataStruct->m_getVGraphObjects();
        for (auto &var: vCurves) {
            treeWidgetDraggableItem *item = new treeWidgetDraggableItem(var, this);
            m_vChildren.push_back(item);
        }
    // Structures
        auto vDataStructures = ptr_dataStruct->m_getVDataStructures();
        for (auto &var: vDataStructures) {
            treeWidgetDraggableTopItem *item = new treeWidgetDraggableTopItem(var, this);
            m_vChildren.push_back(item);
        }
}

void treeWidgetDraggableTopItem::m_loadValues()
{
    // Name
        QString name = "Unknown structure";
        if (auto data = ptr_dataStructure.lock())
            name = data->m_getName().c_str();
        setText(0, name);
        QFont fontBold;
        fontBold.setBold(true);
        setFont(0, fontBold);
    // Children
        for (auto &ptr_children: m_vChildren)
            ptr_children->m_loadValues();
}

listGraphItems::listGraphItems()
{
    m_tree = new treeWidgetDraggable();
    m_preview = new previewGraph(m_tree);
    connect(m_tree, &QTreeWidget::itemSelectionChanged,
            m_preview, &previewGraph::m_slotUpdateCurve);
    QSplitter *widSplitter = new QSplitter(Qt::Vertical);
    widSplitter->setStyleSheet(""
                  "QSplitter::handle{"
                    "background: darkgray;"
                    "height: 3px;"
                  "}"
                  "QSplitter {"
                    "border: 2px solid gray;"
                  "}"
                  "QLabel {"
                    "background: lightgray;"
                  "}"
                  );
    widSplitter->addWidget(m_tree);
    widSplitter->addWidget(m_preview);
    VBoxLayout *layBackground = new VBoxLayout(this);
    layBackground->addWidget(widSplitter);
}

void listGraphItems::m_addDataStructure(std::shared_ptr<dataStructure> ptr_dataStruct)
{
    m_tree->m_addDataStructure(ptr_dataStruct);
}

previewGraph::previewGraph(treeWidgetDraggable *ptr_list):
    ptr_listWithCurves(ptr_list)
{
    // Title
        labPreview = new label("", 12, true);
        labPreview->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    // Info
        m_labValues = new label("", "Number of values");
        m_labMin = new label("", "Minimum value");
        m_labMax = new label("", "Maximum value");
        m_labAvg = new label("", "Average value");
        auto *widHor = new widHorizontal({m_labValues, m_labMin, m_labMax, m_labAvg});
        widHor->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    // Preview
        m_widGraph = new widGraph();
        m_widGraph->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
        m_setGraphParams();
    // Layout
        VBoxLayout *layBackground = new VBoxLayout(this);
        layBackground->addWidget(labPreview);
        layBackground->addWidget(widHor);
        layBackground->addWidget(m_widGraph);
        setMinimumHeight(200);
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    // Update preview - show default values
        m_slotUpdateCurve();
}

void previewGraph::m_setGraphParams()
{
    auto ptr_data = m_widGraph->m_getData().lock();
    ptr_data->m_X->m_setVisible(false);
    ptr_data->m_Y1->m_setVisible(false);
    ptr_data->m_Y2->m_setVisible(false);
    ptr_data->m_title->m_setVisible(false);
    ptr_data->m_legend->m_setVisible(false);
    ptr_data->m_control->m_allowDialog = false;
    m_widGraph->m_loadValues();
}

QSize previewGraph::sizeHint() const
{
    int w = QWidget::sizeHint().width();
    int h = 100;
    return QSize(w,h);
}

void previewGraph::m_slotUpdateCurve()
{
    m_widGraph->m_removeAllObjects();
    QString title = "Summary and preview";

    double values = qInf();
    double min = qInf();
    double max = qInf();
    double avg = qInf();
    auto ptr_item = ptr_listWithCurves->m_getSelectedItem();
    if (ptr_item) {
            auto ptr_object = ptr_item->m_getGraphObject().lock();
        // Title
            title = ptr_object->m_getName().c_str();
        // Info
            min = ptr_object->m_getMinY();
            max = ptr_object->m_getMaxY();
            avg = ptr_object->m_getAvgY();
            values = ptr_object->m_getNValues();
        // Preview
            if (auto ptr_curve = std::dynamic_pointer_cast<graphCurve>(ptr_object))
                m_widGraph->m_addObject(ptr_curve);
            else if (auto ptr_value = std::dynamic_pointer_cast<graphValue>(ptr_object))
                m_widGraph->m_addObject(ptr_value);
            else if (auto ptr_column = std::dynamic_pointer_cast<graphColumn>(ptr_object))
                m_widGraph->m_addObject(ptr_column);
    }
    labPreview->setText(title);
    m_labValues->setText("Vals: " + QString::number(values) + " ");
    m_labMax->setText("Max: " + QString::number(max) + " ");
    m_labMin->setText("Min: " + QString::number(min) + " ");
    m_labAvg->setText("Avg: " + QString::number(avg) + " ");
    m_widGraph->m_loadValues();
 }


dataStructure::dataStructure(const std::string &name):
    m_name(name)
{
    auto numbersX = std::vector<double>{0,1,2,3,4,5};
    m_curveXData = std::make_shared<std::vector<double>>(numbersX);

    auto numbers1 = std::vector<double>{0,2,2,2,1,0};
    auto curve1Data = std::make_shared<std::vector<double>>(numbers1);
    m_vData.push_back(curve1Data);
    auto curve = std::make_shared<graphCurve>("Curve 1", m_curveXData, curve1Data);
    m_vGraphObjects.push_back(curve);

    auto numbers2 = std::vector<double>{10,2,5,30,20,18};
    auto curve2Data = std::make_shared<std::vector<double>>(numbers2);
    m_vData.push_back(curve2Data);
    auto curve2 = std::make_shared<graphCurve>("Curve 2", m_curveXData, curve2Data);
    m_vGraphObjects.push_back(curve2);

    auto numbers3 = std::vector<double>{10,20,30,40,50,10};
    auto curve3Data = std::make_shared<std::vector<double>>(numbers3);
    m_vData.push_back(curve3Data);
    auto curve3 = std::make_shared<graphCurve>("Curve 3", m_curveXData, curve3Data);
    m_vGraphObjects.push_back(curve3);
}

dataStructure::dataStructure():
    m_name("Base")
{
    auto numbersX = std::vector<double>{0,1,2,3,4,5};
    m_curveXData = std::make_shared<std::vector<double>>(numbersX);

    auto numbers1 = std::vector<double>{0,10,2,30,4,50};
    auto curve1Data = std::make_shared<std::vector<double>>(numbers1);
    m_vData.push_back(curve1Data);
    auto curve = std::make_shared<graphCurve>("Curve 1", m_curveXData, curve1Data);
    m_vGraphObjects.push_back(curve);

    auto numbers2 = std::vector<double>{10,10,5,18,8,20};
    auto curve2Data = std::make_shared<std::vector<double>>(numbers2);
    m_vData.push_back(curve2Data);
    auto curve2 = std::make_shared<graphCurve>("Curve 2", m_curveXData, curve2Data);
    m_vGraphObjects.push_back(curve2);

    auto numbers3 = std::vector<double>{12,3,2,6,8,10};
    auto curve3Data = std::make_shared<std::vector<double>>(numbers3);
    m_vData.push_back(curve3Data);
    auto curve3 = std::make_shared<graphCurve>("Curve 3", m_curveXData, curve3Data);
    m_vGraphObjects.push_back(curve3);

    auto structure1 = std::make_shared<dataStructure>("XXX 1");
    m_vDataStructures.push_back(structure1);
    auto structure2 = std::make_shared<dataStructure>("XXX 2");
    m_vDataStructures.push_back(structure2);

    //Test
    m_dataTestValue = std::make_shared<double>(10);
    m_graphObjectsTestValue = std::make_shared<graphValue>("Value 1", m_dataTestValue, orientation::HORIZONTAL);
    auto numColumns = std::vector<double>{20,42,80,13,26,52};
    m_dataTestColumn = std::make_shared<std::vector<double>>(numColumns);
    m_graphObjectsTestColumn = std::make_shared<graphColumn>("Column 1", m_dataTestColumn);
  }
