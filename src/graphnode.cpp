#include "graphnode.h"
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include "graphscene.h"
#include <QtWidgets>
#include "graphslot.h"
#include "graphlink.h"
#include "graphnodefield.h"
#include <set>

GraphNode::GraphNode(GraphScene* scene, const QString& name, int id)
    :mScene(scene)
    ,mId(id)
{
    setBrush(QBrush(QColor(200,200,200)));
    setRect(0, 0, 100, 20);

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    QGraphicsTextItem* label = mScene->addText(name);
    label->setPos(5, (rect().height() - label->boundingRect().height()) * 0.5);
    label->setDefaultTextColor(QColor(0,0,0));
    label->setParentItem(this);
}

GraphNode::~GraphNode()
{
}

void GraphNode::addField(bool isOutput, const QString& name, bool isMultipleInput, GraphNodeFieldType type)
{
    GraphNodeField* field = new GraphNodeField(mScene, this, name, isMultipleInput, isOutput, type);
    field->setParentItem(this);
    mFields.push_back(field);

    qreal y = rect().height();
    foreach (GraphNodeField* f, mFields)
    {
        f->setPos(0, y);
        y += f->rect().height();
    }
    update();
}

QVariant GraphNode::itemChange( GraphicsItemChange change, const QVariant &value )
{
    if (change == QGraphicsItem::ItemPositionHasChanged)
    {
        foreach (GraphNodeField* f, mFields)
        {
            f->updateLinks();
        }
        mScene->OnNodePositionChanged(this);
    }
    else if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
        mScene->OnNodeSelectionChanged(this);
    }
    return value;
}


bool GraphNode::hasParent(GraphNode* node)
{
    std::set<GraphNode*> inNodes;
    foreach (GraphNodeField* f, mFields)
    {
        if (f->slot()->isOutput())
        {
            continue;
        }
        foreach (GraphLink* link, f->slot()->links())
        {
            GraphNode* p = link->beginSlot()->node();
            if (p == node)
            {
                return true;
            }
            inNodes.insert(p);
        }
    }

    for (std::set<GraphNode*>::iterator it = inNodes.begin();
        it != inNodes.end(); ++it)
    {
        if ((*it)->hasParent(node))
        {
            return true;
        }
    }
    return false;
}

GraphNodeField* GraphNode::field(const QString& name) const
{
    for (int i = 0; i < mFields.size(); ++i)
    {
        if (mFields[i]->name() == name)
        {
            return mFields[i];
        }
    }
    return NULL;
}
