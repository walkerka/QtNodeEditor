#include "graphscene.h"
#include "graphnode.h"
#include "graphlink.h"
#include "graphslot.h"
#include "graphnodefield.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

GraphScene::GraphScene(QObject *parent) 
    :QGraphicsScene(parent)
    ,mNextNodeId(1)
    ,mNextLinkId(1)
{
}

GraphNode* GraphScene::CreateNode(const QString& name)
{
    GraphNode* node = new GraphNode(this, name, mNextNodeId++);
    addItem(node);
    mNodes.push_back(node);
    emit nodeAdded(node->id());
    return node;
}

void GraphScene::RemoveNode(GraphNode* node)
{
    for (size_t i = 0; i < mNodes.size(); ++i)
    {
        if (mNodes[i] == node)
        {
            RemoveLinksOfNode(node);
            int id = node->id();
            std::vector<GraphNode*>::iterator it = mNodes.begin();
            it += i;
            mNodes.erase(it);
            removeItem(node);
            delete node;
            emit nodeRemoved(id);
            break;
        }
    }
}

void GraphScene::RemoveLinksOfNode(GraphNode* node)
{
    foreach(GraphNodeField* field, node->fields())
    {
        foreach(GraphLink* link, field->slot()->links())
        {
            RemoveLink(link);
        }
    }
}

GraphLink* GraphScene::AddLink(GraphSlot* fromSlot, GraphSlot* toSlot, int index, bool enableSignal)
{
    if (!toSlot->isMultipleInput())
    {
        for (size_t i = 0; i < mLinks.size(); ++i)
        {
            if (mLinks[i]->endSlot() == toSlot)
            {
                RemoveLink(mLinks[i]);
                break;
            }
        }
    }

    GraphLink* link = new GraphLink(this, fromSlot, toSlot, mNextNodeId++);
    addItem(link);
    link->setZValue(-1);
    link->updateShape();
    fromSlot->addLink(link);
    toSlot->addLink(link, index);
    mLinks.push_back(link);
    if (enableSignal)
    {
        emit linkAdded(link->id(), fromSlot->node()->id(), fromSlot->fieldName(), toSlot->node()->id(), toSlot->fieldName(), index);
    }
    return link;
}

GraphLink* GraphScene::AddLink(GraphNode* fromNode, const QString& fromName, GraphNode* toNode, const QString& toName, int index)
{
    GraphNodeField* fromField = fromNode->field(fromName);
    if (!fromField)
    {
        return NULL;
    }
    foreach (GraphNodeField* field, toNode->fields())
    {
        if (field->name() == toName)
        {
            return AddLink(fromField->slot(), field->slot(), index, false);
        }
    }
    return NULL;
}

void GraphScene::RemoveLink(GraphLink* link)
{
    for (size_t i = 0; i < mLinks.size(); ++i)
    {
        if (mLinks[i] == link)
        {
            int id = link->id();
            int fromId = link->beginSlot()->node()->id();
            int toId = link->endSlot()->node()->id();
            QString fromName = link->beginSlot()->fieldName();
            QString toName = link->endSlot()->fieldName();

            std::vector<GraphLink*>::iterator it = mLinks.begin();
            it += i;
            link->beginSlot()->removeLink(link);
            link->endSlot()->removeLink(link);
            mLinks.erase(it);
            removeItem(link);
            delete link;
            emit linkRemoved(id, fromId, fromName, toId, toName, i);
            break;
        }
    }
}

void GraphScene::Clear()
{
    for (size_t i = 0; i < mLinks.size(); ++i)
    {
        GraphLink* link = mLinks[i];
        removeItem(link);
        delete link;
    }

    for (size_t i = 0; i < mNodes.size(); ++i)
    {
        GraphNode* node = mNodes[i];
        removeItem(node);
        delete node;
    }
    mLinks.clear();
    mNodes.clear();
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}

void GraphScene::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
    {
        const QList<QGraphicsItem*>& sel = selectedItems();
        if (sel.empty())
        {
            return;
        }

        QGraphicsItem* item = sel.front();
        GraphType type = (GraphType)item->type();
        if (type == GraphTypeNode)
        {
            RemoveNode((GraphNode*)item);
        }
        else if (type == GraphTypeLink)
        {
            RemoveLink((GraphLink*)item);
        }
    }
}

int GraphScene::AddNode(const QString& name)
{
    GraphNode* node = CreateNode(name);
    return node->id();
}

void GraphScene::RemoveNode(int id)
{
    for (size_t i = 0; i < mNodes.size(); ++i)
    {
        if (mNodes[i]->id() == id)
        {
            RemoveNode(mNodes[i]);
            break;
        }
    }
}

void GraphScene::SetNodePosition(int id, const QPointF& position)
{
    GraphNode* node = GetNode(id);
    if (node)
    {
        node->setPos(position);
    }
}

void GraphScene::AddNodeField(int id, bool isOutput, const QString& name, bool isMultipleInput, GraphNodeFieldType type)
{
    GraphNode* node = GetNode(id);
    if (node)
    {
        node->addField(isOutput, name, isMultipleInput, type);
    }
}

int GraphScene::AddLink(int fromNode, const QString& fromName, int toNode, const QString& toName, int index)
{
    GraphNode* from = GetNode(fromNode);
    GraphNode* to = GetNode(toNode);

    if (from == NULL || to == NULL)
    {
        return 0;
    }

    GraphLink* link = AddLink(from, fromName, to, toName, index);
    return link->id();
}

void GraphScene::RemoveLink(int fromNode, const QString& /*fromName*/, int toNode, const QString& toName, int index)
{
    GraphNode* from = GetNode(fromNode);
    GraphNode* to = GetNode(toNode);

    if (from == NULL || to == NULL)
    {
        return;
    }

    GraphNodeField* f = to->field(toName);
    if (f == NULL || f->slot()->linkCount() <= index)
    {
        return;
    }

    GraphLink* l = f->slot()->links()[index];
    if (l->beginSlot()->node() == from)
    {
        RemoveLink(l);
    }
}

GraphNode* GraphScene::GetNode(int id)
{
    for (size_t i = 0; i < mNodes.size(); ++i)
    {
        if (mNodes[i]->id() == id)
        {
            return mNodes[i];
        }
    }
    return NULL;
}

void GraphScene::OnNodePositionChanged(GraphNode* node)
{
    emit nodeMoved(node->id(), node->pos());
}

void GraphScene::OnNodeSelectionChanged(GraphNode* node)
{
    emit nodeSelected(node->id());
}
